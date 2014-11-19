// PlayVideo_ffmpeg_sdl.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "WaveFile.h"

#define OUTPUT_WAVE 0

HANDLE g_decodeThread = NULL;
HANDLE g_playVideoThread = NULL;
HANDLE g_hInstanceThread = NULL;
bool g_bStop = true;
bool g_bEndProcess = false;
BOOL g_bInit = FALSE;

int g_width = 1024;
int g_height = 576;
int g_cusWidth = 0;
int g_cusHeight = 0;
int g_nMaxAudioChn = 0;
bool g_nSwitchChn = false;
float g_fMultiSpeed = 1;
int g_nCurTime = 0;
SDL_Rect g_dstRc;

AVCodecContext* g_pAudioCodecCtx;

BOOL g_bPauseVideo = FALSE;
HANDLE g_hPauseWorkThread;
HANDLE g_hPauseMainThread;
HANDLE g_hCloseProcess;

CRITICAL_SECTION g_csResizeWnd;


CString g_csFileName = _T("");

SDL_Window* g_pWin = NULL;
SDL_Surface* g_pScreen = NULL;
SDL_Renderer* g_pRender = NULL;
SDL_Texture* g_pTexture = NULL;

uint64_t g_seek_pos = 0;
int g_audioChn = 0;
enum emSeekType
{
	emSecond,
	emMinute,
	emForward_10s,
	emBack_10s
};

emSeekType g_emSeekType;
BOOL g_bSeek = FALSE;


#define DEQUESIZE 10

SDL_Rect g_texture_dimensions;

uint8_t audio_buf[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2];

CRITICAL_SECTION g_cs;

using namespace std;

class CAudioAdjust
{
public:
	explicit CAudioAdjust(UINT32 dwChannels = 2,const INT64 nSampleCount = 1920,const INT64 nBlockAlign = 4,const INT64 nSize = 50)
		:m_dwChannels(dwChannels),m_nSampleCount(nSampleCount),m_nBlockAlign(nBlockAlign),m_nSize(nSize),m_pBuff(NULL)
	{
		m_nTotalSizeInByte = m_nSampleCount * m_nBlockAlign * m_nSize;
		m_pBuff = new BYTE[m_nSampleCount*m_nBlockAlign*m_nSize];
		m_nCurSamples = 0;
		m_nCurBytes = 0;
	}
	~CAudioAdjust()
	{
		m_nCurSamples = 0;
		delete []m_pBuff;
		m_pBuff = NULL;
		m_nTotalSizeInByte = 0;
	}

	//加入采样
	BOOL Incorporate(const BYTE *pBuf,const INT64 nSampleCount);

	BOOL Incorporate1(const BYTE *pBuf,const INT64 nByteNum);

	//是否能取出完整采样
	BOOL CanExtract();
	BOOL IsEnough(int num);

	BOOL IsFull();
	//取出完整采样--以nSampleCount为准
	//成功返回TRUE,失败返回FALSE
	BOOL Extract(BYTE **ppBuf);

	BOOL Extract(BYTE **ppBuf, int len);

	void Reset();


public:
	const UINT32 m_dwChannels;
	const INT64 m_nSampleCount;
	const INT64 m_nBlockAlign;
	const INT64 m_nSize;


protected:

	INT64	m_nCurSamples;
	INT64	m_nTotalSizeInByte;
	INT64   m_nCurBytes;
	BYTE	*m_pBuff;
};

BOOL CAudioAdjust::Incorporate(const BYTE *pBuf,const INT64 nSampleCount)
{
	if ((nSampleCount+m_nCurSamples) * m_nBlockAlign > m_nTotalSizeInByte)
	{
		return FALSE;
	}

	memcpy(m_pBuff + m_nCurSamples * m_nBlockAlign, pBuf, nSampleCount * m_nBlockAlign);
	m_nCurSamples += nSampleCount;

	return TRUE;
}

BOOL CAudioAdjust::Incorporate1(const BYTE *pBuf,const INT64 nByteNum)
{
	if(nByteNum+m_nCurBytes > m_nTotalSizeInByte)
	{
		return FALSE;
	}
	memcpy(m_pBuff + m_nCurBytes, pBuf, nByteNum);
	m_nCurBytes += nByteNum;
}

BOOL CAudioAdjust::CanExtract()
{
	if (m_nCurSamples < m_nSampleCount)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOL CAudioAdjust::IsEnough(int num)
{
	if(num >= m_nCurBytes)
	{
		return FALSE;
	}
	return TRUE;
}
BOOL CAudioAdjust::Extract(BYTE **ppBuf)
{
	if (m_nCurSamples < m_nSampleCount)
	{
		return FALSE;
	}
	if(m_dwChannels >= 2)
	{
		memcpy(*ppBuf, m_pBuff, m_nSampleCount * m_nBlockAlign);
	}
	else if(m_dwChannels == 1)
	{
		for(int i=0;i<m_nSampleCount;i++)
		{
			memcpy(*ppBuf+i*m_nBlockAlign*2, m_pBuff+i*m_nBlockAlign, m_nBlockAlign);
			memcpy(*ppBuf+(i*2+1)*m_nBlockAlign, m_pBuff+i*m_nBlockAlign, m_nBlockAlign);

		}
	}
	memcpy(m_pBuff, m_pBuff + m_nSampleCount * m_nBlockAlign ,m_nSampleCount * m_nBlockAlign * (m_nSize-1));

	m_nCurSamples -= m_nSampleCount;

	return TRUE;
}

BOOL CAudioAdjust::Extract(BYTE **ppBuf, int len)
{
	memcpy(*ppBuf, m_pBuff, len);
	memcpy(m_pBuff, m_pBuff + len , m_nTotalSizeInByte - len);
	m_nCurBytes -= len;
	return TRUE;
}

BOOL CAudioAdjust::IsFull()
{
	if(m_nCurBytes >= m_nTotalSizeInByte)
		return TRUE;
	return FALSE;
}

void CAudioAdjust::Reset()
{
	m_nCurBytes = 0;
}

CWaveFile* g_fWav;
BOOL InitWavFile( CString strWavFile )
{
	WAVEFORMATEX  wfxInput;

	ZeroMemory( &wfxInput, sizeof(wfxInput));
	wfxInput.wFormatTag = WAVE_FORMAT_PCM;
	wfxInput.nSamplesPerSec = 48000;
	wfxInput.wBitsPerSample = 16;
	wfxInput.nChannels = 2;
	wfxInput.nBlockAlign = 
		wfxInput.nChannels * (wfxInput.wBitsPerSample / 8);
	wfxInput.nAvgBytesPerSec = 
		wfxInput.nBlockAlign * wfxInput.nSamplesPerSec;

	g_fWav = new CWaveFile;
	HRESULT hr = g_fWav->Open( (LPTSTR)(LPCTSTR)strWavFile, &wfxInput, WAVEFILE_WRITE );
	if( S_OK != hr )
	{
		g_fWav->Close();
		return FALSE;
	}

	return TRUE;
}

BOOL WriteWavFile( UINT8 *pAudioBuffer, UINT32 dwAudioDataSize, UINT32 *pWroteSize )
{
	if( NULL == g_fWav )
		return FALSE;

	HRESULT hr = g_fWav->Write( dwAudioDataSize, (BYTE*)pAudioBuffer, pWroteSize );
	if( S_OK != hr )
	{
		*pWroteSize = 0;
		return FALSE;
	}

	return  TRUE;
}

BOOL CloseWavFile()
{
	if( NULL == g_fWav )
		return FALSE;

	g_fWav->Close();
	return TRUE;
}

struct VideoFrame
{
	uint8_t* out_buffer;
	AVFrame* pFrame;
};

class VideoArryCirculator
{
public:
	

	VideoArryCirculator( int size = DEQUESIZE, int videoWidth = 1920, int videoHeight = 1080, AVPixelFormat videoFmt = PIX_FMT_YUV420P);
	~VideoArryCirculator();

	BOOL GetIdleFrame(VideoFrame** pFrame);
	BOOL GetDataFrame(VideoFrame** pFrame);
	void PutIdleFrame(VideoFrame* pFrame);
	void PutDataFrame(VideoFrame* pFrame);
	void ResetVideoDeque();
private:
	void CreateVideoDeque();
	void ReleaseVideoDeque();
	
private:
	CRITICAL_SECTION m_csDataLock;
	CRITICAL_SECTION m_csIdleLock;
	int m_size;
	int m_videoWidth;
	int m_videoHeight;
	AVPixelFormat m_videoFmt;
	
	VideoFrame* m_arryFrames;
	std::deque<VideoFrame*> m_idleVideoDeque;
	std::deque<VideoFrame*> m_useVideoDeque;
};

VideoArryCirculator::VideoArryCirculator( int size, int videoWidth , int videoHeight, AVPixelFormat videoFmt )
{
	m_size = size;
	m_videoWidth = videoWidth;
	m_videoHeight = videoHeight;
	m_videoFmt = videoFmt;
	CreateVideoDeque();
	InitializeCriticalSection(&m_csDataLock);
	InitializeCriticalSection(&m_csIdleLock);
}

VideoArryCirculator::~VideoArryCirculator()
{
	ReleaseVideoDeque();
	DeleteCriticalSection(&m_csDataLock);
	DeleteCriticalSection(&m_csIdleLock);
}

BOOL VideoArryCirculator::GetIdleFrame(VideoFrame** pFrame)
{
	BOOL bRet = FALSE;
	EnterCriticalSection(&m_csIdleLock);
	if(m_idleVideoDeque.size() > 0)
	{
		*pFrame = m_idleVideoDeque.front();
		m_idleVideoDeque.pop_front();
		bRet = TRUE;
	}
	LeaveCriticalSection(&m_csIdleLock);
	return bRet;
}

BOOL VideoArryCirculator::GetDataFrame(VideoFrame** pFrame)
{
	BOOL bRet = FALSE;
	EnterCriticalSection(&m_csDataLock);
	if(m_useVideoDeque.size() > 0)
	{
		*pFrame = m_useVideoDeque.front();
		m_useVideoDeque.pop_front();
		bRet = TRUE;
	}
	LeaveCriticalSection(&m_csDataLock);
	return bRet;
}

void VideoArryCirculator::PutIdleFrame(VideoFrame* pFrame)
{
	EnterCriticalSection(&m_csIdleLock);
	m_idleVideoDeque.push_back(pFrame);
	LeaveCriticalSection(&m_csIdleLock);
}

void VideoArryCirculator::PutDataFrame(VideoFrame* pFrame)
{
	EnterCriticalSection(&m_csDataLock);
	m_useVideoDeque.push_back(pFrame);
	LeaveCriticalSection(&m_csDataLock);
}

void VideoArryCirculator::CreateVideoDeque()
{
	if(m_size <= 0)
		return;
	m_arryFrames = new VideoFrame[m_size];
	for (int i = 0; i < m_size; i++)
	{
		m_arryFrames[i].pFrame = avcodec_alloc_frame();
		m_arryFrames[i].out_buffer = new uint8_t[avpicture_get_size(m_videoFmt, m_videoWidth, m_videoHeight)];
		avpicture_fill((AVPicture*)m_arryFrames[i].pFrame, m_arryFrames[i].out_buffer, m_videoFmt, m_videoWidth, m_videoHeight);
		m_idleVideoDeque.push_back(&m_arryFrames[i]);
	}
}
void VideoArryCirculator::ReleaseVideoDeque()
{
	if(m_size <= 0)
		return;
	EnterCriticalSection(&m_csIdleLock);
	EnterCriticalSection(&m_csDataLock);
	m_idleVideoDeque.clear();
	m_useVideoDeque.clear();
	for(int i = 0; i < DEQUESIZE; i++)
	{
		if(m_arryFrames[i].pFrame)
			avcodec_free_frame(&m_arryFrames[i].pFrame);
		if(m_arryFrames[i].out_buffer)
			delete[] m_arryFrames[i].out_buffer;
	}
	LeaveCriticalSection(&m_csDataLock);
	LeaveCriticalSection(&m_csIdleLock);
}

void VideoArryCirculator::ResetVideoDeque()
{
	m_useVideoDeque.clear();
	m_idleVideoDeque.clear();
	for (int i = 0; i < DEQUESIZE; i++)
	{
		m_idleVideoDeque.push_back(&m_arryFrames[i]);
	}
}

VideoArryCirculator* g_pVideoCirc;



CAudioAdjust g_audioBuffer(2, 1024);

void EndCurrentPlay()
{
	g_bStop = true;
	SDL_Event event;
	event.type = SDL_QUIT;
	SDL_PushEvent(&event);
}


void mixaudio(void *unused, Uint8 *stream, int len)
{
	//SDL_LockAudio();
	if(len <= 0)
		return;

	/*int audio_len=4608;
	len=(len>audio_len?audio_len:len);*/
	while(!g_bStop)
	{
		if(g_audioBuffer.IsFull())
		{
			printf("audio buffer is full!\n");
		}
		if(g_audioBuffer.IsEnough(len))
		{
			EnterCriticalSection(&g_cs);
			g_audioBuffer.Extract(&stream, len);

#if OUTPUT_WAVE
			UINT32 pOut;
			WriteWavFile(stream, len, &pOut);
#endif

			LeaveCriticalSection(&g_cs);
			break;
		}
		else
		{
			//Sleep(2);
		}
	}
	//SDL_UnlockAudio();
}

bool isAllDigit(const string& str)
{
	int i ;
	for(i = 0; i != str.length(); i++)
	{
		if(!isdigit(str[i]))
		{
			return false;
		}
	}
	return true;
}
#if _UNICODE
#define std_tcin std::wcin
#define std_tstring std::wstring
#else
#define std_tcin std::cin
#define std_tstring std::string
#endif


DWORD WINAPI DecodeProc(LPVOID pParam)
{
	AVFormatContext	*pFormatCtx;
	int				i, videoindex;
	AVCodecContext	*pCodecCtx;
	AVCodec			*pCodec;
	USES_CONVERSION;
	std::string strVideoPath = T2A(g_csFileName);
	g_bInit = TRUE;
	av_register_all();//注册组件
	//avformat_network_init();//支持网络流
	pFormatCtx = avformat_alloc_context();//初始化AVFormatContext
	if(avformat_open_input(&pFormatCtx,strVideoPath.c_str(),NULL,NULL)!=0){//打开文件
		printf("无法打开文件\n");
		EndCurrentPlay();
		return -1;
	}
	if(av_find_stream_info(pFormatCtx)<0)//查找流信息
	{
		printf("Couldn't find stream information.\n");
		EndCurrentPlay();
		return -1;
	}
	videoindex=-1;
	int audioIndex[2] = {-1, -1};
	g_nMaxAudioChn = 0;
	for(i=0; i<pFormatCtx->nb_streams; i++) //获取视频流ID
	{
		if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO)
		{
			videoindex=i;
		}
		if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO)
		{
			audioIndex[g_nMaxAudioChn] = i;
			g_nMaxAudioChn++;
		}
	}
		if(videoindex==-1)
		{
			printf("Didn't find a video stream.\n");
			return -1;
		}
		if(audioIndex[0] == -1)
		{
			printf("Didn't find a audio stream.\n");
			return -1;
		}
		pCodecCtx=pFormatCtx->streams[videoindex]->codec;
		pCodec=avcodec_find_decoder(pCodecCtx->codec_id);//查找解码器
		if(pCodec==NULL)
		{
			printf("Codec not found.\n");
			return -1;
		}
		if(avcodec_open(pCodecCtx, pCodec)<0)//打开解码器
		{
			printf("Could not open codec.\n");
			return -1;
		}

		AVCodec* pAudioCodecDecoder1 = NULL;
		AVCodec* pAudioCodecDecoder2 = NULL;

		AVCodecContext	*pAudioCodecCtx1 = pFormatCtx->streams[audioIndex[0]]->codec;
		pAudioCodecDecoder1 = avcodec_find_decoder(pAudioCodecCtx1->codec_id);
		if(!pAudioCodecDecoder1)
		{
			printf("Audio codec not found.\n");
		}
		g_pAudioCodecCtx = pAudioCodecCtx1;
		int aret = avcodec_open2(pAudioCodecCtx1, pAudioCodecDecoder1, NULL);
		if(aret<0)//打开解码器
		{
			char errbuf[128];
			const char *errbuf_ptr = errbuf;
			if (av_strerror(aret, errbuf, sizeof(errbuf)) < 0)
				errbuf_ptr = strerror(AVUNERROR(aret));
			fprintf(stderr, "Could not open '%s'\n", errbuf_ptr);
			printf("Could not open audio codec.\n");
		}
		AVFrame* pAudioFrame = avcodec_alloc_frame();
		AVFifoBuffer* _pfifo;
		_pfifo = av_fifo_alloc(AVCODEC_MAX_AUDIO_FRAME_SIZE* pAudioCodecCtx1->channels);
		UINT8* pSamplesbuffer = (UINT8*)av_malloc(1920 * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16) * pAudioCodecCtx1->channels*4);
		AVCodecContext	*pAudioCodecCtx2 = NULL;
		if(g_nMaxAudioChn > 1)
		{
			pAudioCodecCtx2 = pFormatCtx->streams[audioIndex[1]]->codec;
			pAudioCodecDecoder2 = avcodec_find_decoder(pAudioCodecCtx2->codec_id);
			if(!pAudioCodecDecoder2)
			{
				printf("Audio codec not found.\n");
			}
			aret = avcodec_open2(pAudioCodecCtx2, pAudioCodecDecoder2, NULL);
			if(aret<0)//打开解码器
			{
				char errbuf[128];
				const char *errbuf_ptr = errbuf;
				if (av_strerror(aret, errbuf, sizeof(errbuf)) < 0)
					errbuf_ptr = strerror(AVUNERROR(aret));
				fprintf(stderr, "Could not open '%s'\n", errbuf_ptr);
				printf("Could not open audio codec.\n");
			}
		}
		


		AVFrame	*pFrame;
		pFrame=avcodec_alloc_frame();//存储解码后AVFrame
		
		g_width = pCodecCtx->width;
		g_height = pCodecCtx->height;

		g_dstRc.x = 0;
		g_dstRc.y = 0;
		g_dstRc.w = g_width;
		g_dstRc.h = g_height;

		SetEvent(g_hPauseMainThread);//激活主线程
		WaitForSingleObject(g_hPauseWorkThread, INFINITE); //等待主线程初始化完成SDL

		/*SDL_Overlay *bmp; 
		bmp = SDL_CreateYUVOverlay(pCodecCtx->width, pCodecCtx->height,SDL_YV12_OVERLAY, screen);*/ 
		SDL_Rect rect;
		//-----------------------------
		int ret, got_picture;
		int y_size = pCodecCtx->width * pCodecCtx->height;

		AVPacket *packet=(AVPacket *)malloc(sizeof(AVPacket));//存储解码前数据包AVPacket
		av_new_packet(packet, y_size);
		//输出一下信息-----------------------------
		printf("文件信息-----------------------------------------\n");
		av_dump_format(pFormatCtx,0,strVideoPath.c_str(),0);
		printf("-------------------------------------------------\n");
		//------------------------------

		

		SwrContext* swrCxt =  swr_alloc_set_opts(NULL, pAudioCodecCtx1->channel_layout, AV_SAMPLE_FMT_S16P, pAudioCodecCtx1->sample_rate, pAudioCodecCtx1->channel_layout, pAudioCodecCtx1->sample_fmt, 
			pAudioCodecCtx1->sample_rate, 0, 0);

		swr_init(swrCxt);

		

		uint8_t* audioSwrBuf = new uint8_t[AVCODEC_MAX_AUDIO_FRAME_SIZE*3/2];
		memset(audioSwrBuf, 0, AVCODEC_MAX_AUDIO_FRAME_SIZE*3*sizeof(uint8_t)/2);
		int audioSwrLen = AVCODEC_MAX_AUDIO_FRAME_SIZE*3/2;
#if OUTPUT_WAVE
		InitWavFile(_T("wave.wav"));
#endif
		
		SwsContext *img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
		if(!img_convert_ctx)
		{
			printf("sws_getContext failed!");
		}
		//SDL_PauseAudio(0);
		while(av_read_frame(pFormatCtx, packet)>=0)//循环获取压缩数据包AVPacket
		{
			if(packet->stream_index==videoindex)
			{
				ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);//解码。输入为AVPacket，输出为AVFrame
				if(ret < 0)
				{
					printf("解码错误\n");
					break;
				}
				if(got_picture)
				{
					//像素格式转换。pFrame转换为pFrameYUV。
					 
					VideoFrame* pVideoFrame = NULL;
					if(g_pVideoCirc && g_pVideoCirc->GetIdleFrame(&pVideoFrame))
					{
						AVFrame* pFrameYUV = pVideoFrame->pFrame;
						sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
						g_pVideoCirc->PutDataFrame(pVideoFrame);
					}
					
					//延时40ms
					int delayTime = g_fMultiSpeed* 40;
					SDL_Delay(delayTime);
					//------------SDL-----------
				}
			}
			else if(packet->stream_index == audioIndex[0] || packet->stream_index == audioIndex[1])
			{
				if(g_audioChn == 0 &&  packet->stream_index == audioIndex[1]
				|| g_audioChn == 1 &&  packet->stream_index == audioIndex[0])
				{
					av_free_packet(packet);
					continue;
				}
				AVCodecContext* pAudioCodecCtx = NULL;

				if(g_nSwitchChn)
				{
					EnterCriticalSection(&g_cs);
					g_audioBuffer.Reset();
					LeaveCriticalSection(&g_cs);
					av_fifo_reset(_pfifo);
					g_nSwitchChn = false;
				}
				if(g_audioChn == 0)
				{
					pAudioCodecCtx = pAudioCodecCtx1;
				}
				else if(g_audioChn == 1)
				{
					pAudioCodecCtx = pAudioCodecCtx2;
				}
				else
				{
					if(pAudioCodecCtx == NULL)
					{
						pAudioCodecCtx = pAudioCodecCtx1;
					}
				}


				int planar;
				int bFinished = 0;
				int len = avcodec_decode_audio4( pAudioCodecCtx, pAudioFrame, &bFinished, packet );
				
				if( bFinished )
				{
					int size = 0;
					int alen = av_samples_get_buffer_size(pAudioFrame->linesize, pAudioCodecCtx->channels, pAudioFrame->nb_samples, pAudioCodecCtx->sample_fmt, 0);
					planar = av_sample_fmt_is_planar(pAudioCodecCtx->sample_fmt);

					if(pAudioCodecCtx->sample_fmt != AV_SAMPLE_FMT_S16P && pAudioCodecCtx->sample_fmt != AV_SAMPLE_FMT_S16)
					{
						audioSwrLen = AVCODEC_MAX_AUDIO_FRAME_SIZE*3/2/pAudioCodecCtx->channels/av_get_bytes_per_sample(AV_SAMPLE_FMT_S16P);
						int len=swr_convert(swrCxt, &audioSwrBuf, audioSwrLen, (const uint8_t**)(pAudioFrame->extended_data), pAudioFrame->nb_samples);	
						audioSwrLen=len*pAudioCodecCtx->channels*av_get_bytes_per_sample(AV_SAMPLE_FMT_S16P); 
						alen = audioSwrLen;
						for(int i = 0; i < pAudioCodecCtx->channels; i++)
						{
							memcpy(pAudioFrame->data[i], audioSwrBuf, alen);
						}	
					}
					else
					{
						audioSwrLen = pAudioFrame->linesize[0];
					}
					uint16_t *out16=NULL;
					if (planar && pAudioCodecCtx->channels>1 ) 
					{
						int k, j;
						out16 = (uint16_t *)audio_buf;
						for (k = 0; k < pAudioCodecCtx->channels;k++) {
							for (i=0, j=0; i<audioSwrLen; i+=2,++j) {
								out16[k%2 + 2*j] = pAudioFrame->data[k][i] & 0xff;
								if ((i+1)>=audioSwrLen)
								{
									fprintf(stderr, "linesize[0]是奇数, pFrame->linesize[0]=%d\n",
										audioSwrLen);
									break;
								}
								out16[ 2*j + k%2] |= ((pAudioFrame->data[k][i+1]) & 0xff) << 8;
							}
						}

						/*short *sample_buffer = (short *)audioSwrBuf;
						for (int i = 0; i < alen/2; i++)

						{
						out16[i*2] = (char)(sample_buffer[i/2] & 0xFF);
						out16[i*2+1] = (char)((sample_buffer[i/2] >>8) & 0xFF);		
						}*/

						av_fifo_generic_write(_pfifo, out16, alen,NULL);
					} 
					else
					{
						av_fifo_generic_write(_pfifo, pAudioFrame->data[0], alen,NULL);
					}



					
					while( (size = av_fifo_size(_pfifo)) >= alen )
					{
						av_fifo_generic_read(_pfifo, pSamplesbuffer, alen,NULL);
						
						EnterCriticalSection(&g_cs);
						g_audioBuffer.Incorporate1((UINT8*)pSamplesbuffer, alen);
						LeaveCriticalSection(&g_cs);
					}
				}
			}

			int curTimePos = packet->pts;
			g_nCurTime = curTimePos*pFormatCtx->streams[videoindex]->time_base.num/pFormatCtx->streams[videoindex]->time_base.den;
			av_free_packet(packet);

			if(g_bStop)
				break;

			if(g_bPauseVideo)
			{
				HANDLE hArry[2] = {0};
				hArry[0] = g_hCloseProcess;
				hArry[1] = g_hPauseWorkThread;
				SDL_PauseAudio(1);
				WaitForMultipleObjects( 2, hArry, FALSE, INFINITE );
				SDL_PauseAudio(0);
			}
			uint64_t time;
			if(g_bSeek)
			{
				switch (g_emSeekType)
				{
				case emSecond:
					g_seek_pos = g_seek_pos*AV_TIME_BASE;
					break;
				case emMinute:
					g_seek_pos = g_seek_pos*AV_TIME_BASE*60;
					break;
				case emForward_10s:
					time = curTimePos*pFormatCtx->streams[videoindex]->time_base.num/pFormatCtx->streams[videoindex]->time_base.den + g_seek_pos;
					g_seek_pos = time*AV_TIME_BASE;
					if(g_seek_pos > pFormatCtx->duration)
					{
						g_seek_pos = pFormatCtx->duration;
					}
					break;
				case emBack_10s:
					time = curTimePos*pFormatCtx->streams[videoindex]->time_base.num/pFormatCtx->streams[videoindex]->time_base.den - g_seek_pos;
					g_seek_pos = time > 0 ? time*AV_TIME_BASE : 0;
					break;
				}
				EnterCriticalSection(&g_cs);
				g_audioBuffer.Reset();
				LeaveCriticalSection(&g_cs);
				av_fifo_reset(_pfifo);
				av_seek_frame(pFormatCtx, -1, g_seek_pos,AVSEEK_FLAG_BACKWARD);
				g_bSeek = false;
			}
			
		}
		//SDL_PauseAudio(1);

		swr_free(&swrCxt);
		sws_freeContext(img_convert_ctx);


#if OUTPUT_WAVE
		CloseWavFile();
#endif
		avcodec_free_frame(&pFrame);
		//avcodec_close(pCodecCtx);
		//avcodec_close(pAudioCodecCtx);
		av_fifo_free(_pfifo);
		avcodec_free_frame(&pAudioFrame);
		avformat_close_input(&pFormatCtx);
		av_free(pSamplesbuffer);

		delete[] audioSwrBuf;
}

DWORD WINAPI PlayVideoProc(LPVOID pParam)
{
	while (!g_bStop)
	{
		VideoFrame* pVideoFrame = NULL;
		if(g_pVideoCirc && g_pVideoCirc->GetDataFrame(&pVideoFrame))
		{
			AVFrame* pFrameYUV = pVideoFrame->pFrame;

			EnterCriticalSection(&g_csResizeWnd);
			SDL_Rect dstRect;
			dstRect = g_dstRc;
			SDL_UpdateYUVTexture(g_pTexture, &g_texture_dimensions, pFrameYUV->data[0], pFrameYUV->linesize[0],
				pFrameYUV->data[1], pFrameYUV->linesize[1], pFrameYUV->data[2], pFrameYUV->linesize[2]);

			SDL_RenderClear(g_pRender);
			SDL_RenderCopy(g_pRender, g_pTexture, &g_texture_dimensions, &dstRect);
			
			
			SDL_RenderPresent(g_pRender);

			LeaveCriticalSection(&g_csResizeWnd);

			g_pVideoCirc->PutIdleFrame(pVideoFrame);
		}
		else
		{
			//printf("Get Video Data Frame Failed!\n");
			Sleep(40);
		}
		
	}
	return 0;
}

bool InitSDL()
{
	//char sdl_var[128];  
	//sprintf(sdl_var, "SDL_WINDOWID=0x%lx", GetModuleHandle(NULL) );//主窗口句柄 
	//putenv(sdl_var);

	//------------SDL初始化--------
	//SDL_SetModuleHandle(GetModuleHandle(NULL));
	if(SDL_Init(SDL_INIT_VIDEO /*| SDL_INIT_AUDIO*//* | SDL_INIT_TIMER*/)) {  
		printf( "Could not initialize SDL - %s\n", SDL_GetError()); 
		return -1;
	} 

	if(SDL_VideoInit(NULL) == -1) {
		printf("Could not initialize SDL video./n");
		return false;
	}

	int width = g_cusWidth > 0 ? g_cusWidth : g_width;
	int height = g_cusHeight > 0 ? g_cusHeight : g_height;


	SDL_PixelFormat* pixformat = NULL;
	g_pWin = SDL_CreateWindow("SDL_WIN", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, /*SDL_WINDOW_RESIZABLE |*/ SDL_WINDOW_SHOWN);
	int xPos = (GetSystemMetrics(SM_CXSCREEN) - width)/2; 

	int yPos = (GetSystemMetrics(SM_CYSCREEN) - height)/2;
	SDL_SetWindowPosition(g_pWin, xPos, yPos);
	SDL_SetWindowTitle(g_pWin, "Player");


	

	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear"); 
	//SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");


	SDL_SetWindowBordered(g_pWin, SDL_TRUE);

	g_pScreen = SDL_GetWindowSurface( g_pWin );
	if(!g_pScreen)
		printf("SDL_GetWindowSurface Failed!\n");

	g_pRender = SDL_CreateRenderer(g_pWin, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	//g_pRender = SDL_CreateSoftwareRenderer(g_pScreen);
	if(!g_pRender)
		printf("SDL_CreateRenderer Failed!\n");

	g_pTexture = SDL_CreateTexture(g_pRender, SDL_PIXELFORMAT_YV12,
		SDL_TEXTUREACCESS_STREAMING, g_width, g_height);
	if(!g_pTexture)
		printf("SDL_CreateTextureFromSurface Failed!\n");

	Uint32 pixelFormat = 0;
	int access = 0;
	SDL_QueryTexture(g_pTexture,(Uint32 *)&pixelFormat,(int *)&access,&g_texture_dimensions.w,&g_texture_dimensions.h);




	int hTemp = width*g_height/g_width;
	int wTemp = height*g_width/g_height;

	if(hTemp > height)
	{
		g_dstRc.x = (width - wTemp)/2;
		g_dstRc.y = 0;
		g_dstRc.w = wTemp;
		g_dstRc.h = height;
	}
	else
	{
		g_dstRc.x = 0;
		g_dstRc.y = (height-hTemp)/2;
		g_dstRc.w = width;
		g_dstRc.h = hTemp;
	}

	SDL_AudioSpec wanted_spec, spec;
	wanted_spec.format = AUDIO_S16SYS;// 具体含义请查看“SDL宏定义”部分
	wanted_spec.freq = g_pAudioCodecCtx->sample_rate; 
	wanted_spec.channels = g_pAudioCodecCtx->channels;
	//wanted_spec.silence = 0; // 0指示静音
	wanted_spec.size = (AVCODEC_MAX_AUDIO_FRAME_SIZE * 3)/2;
	wanted_spec.samples = 1024; // 自定义SDL缓冲区大小
	wanted_spec.callback = mixaudio; // 音频解码的关键回调函数
	wanted_spec.userdata = NULL; // 传给上面回调函数的外带数据

	g_audioBuffer.Reset();

	if(SDL_OpenAudio(&wanted_spec, NULL) < 0)
	{
		printf("SDL_OpenAudio Failed!\n");
	}

	SDL_PauseAudio(0);
	return true;
}

void UninitSDL()
{
	if(g_pTexture)
		SDL_DestroyTexture(g_pTexture);
	if(g_pRender)
		SDL_DestroyRenderer(g_pRender);
	if(g_pScreen)
		SDL_FreeSurface( g_pScreen );
	if(g_pWin)
		SDL_DestroyWindow( g_pWin );

	SDL_AudioStatus stc = SDL_GetAudioStatus();
	if(stc == SDL_AUDIO_PLAYING || stc == SDL_AUDIO_PAUSED)
	{
		SDL_PauseAudio(1);
		SDL_CloseAudio();
	}

	SDL_Quit();
}

int PlayVA()
{
	InitializeCriticalSection(&g_cs);
	InitializeCriticalSection(&g_csResizeWnd);

	g_hPauseWorkThread = CreateEvent(NULL, FALSE, FALSE, NULL);
	g_hPauseMainThread = CreateEvent(NULL, FALSE, FALSE, NULL);
	g_hCloseProcess = CreateEvent(NULL, FALSE, FALSE, NULL);

	g_bStop = false;

	g_decodeThread = ::CreateThread(NULL, 0, DecodeProc, 0, 0, NULL);

	WaitForSingleObject(g_hPauseMainThread, INFINITE); //等待工作线程获取视频宽高信息


	g_pVideoCirc = new VideoArryCirculator(10, g_width, g_height);

	g_playVideoThread = ::CreateThread(NULL, 0, PlayVideoProc, 0, 0, NULL);

	int nRenderDrivers = SDL_GetNumRenderDrivers(); 
	int i = 0; 
	for (; i < nRenderDrivers; i++) { 
		SDL_RendererInfo info; 
		SDL_GetRenderDriverInfo(i, &info);    //d3d 
		printf("******%d-----info name: %s ******\n", i, info.name); 
		printf("%s max_texture_height: %d\n", info.name, info.max_texture_height); 
		printf("%s max_texture_width: %d\n", info.name, info.max_texture_width); 

	} 

	if(!InitSDL())
		return -1;

	SetEvent(g_hPauseWorkThread);//激活工作线程

	SDL_Event event;
	while (!g_bStop)  //处理SDL主界面的消息循环
	{
		if (SDL_WaitEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				g_bStop = true;
			}
			else if(event.type == SDL_MOUSEBUTTONUP
				|| event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
			{
				g_bPauseVideo = !g_bPauseVideo;
				if(!g_bPauseVideo)
				{
					SetEvent(g_hPauseWorkThread);
				}
			}
			if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_LEFT)
			{
				g_seek_pos = 10;
				g_emSeekType = emBack_10s;
				g_bSeek = TRUE;
			}
			else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RIGHT)
			{
				g_seek_pos = 10;
				g_emSeekType = emForward_10s;
				g_bSeek = TRUE;
			}
			else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN)
			{
				SDL_SetWindowFullscreen(g_pWin, SDL_WINDOW_FULLSCREEN_DESKTOP);
			}
			else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			{
				SDL_SetWindowFullscreen(g_pWin, SDL_WINDOW_SHOWN);
			}
			else if(event.type == SDL_WINDOWEVENT)
			{
				if (event.window.event == SDL_WINDOWEVENT_RESIZED) 
				{
					EnterCriticalSection(&g_csResizeWnd);
					int width = event.window.data1;
					int height = event.window.data2;

					int hTemp = width*g_height/g_width;
					int wTemp = height*g_width/g_height;

					if(hTemp > height)
					{
						g_dstRc.x = (width - wTemp)/2;
						g_dstRc.y = 0;
						g_dstRc.w = wTemp;
						g_dstRc.h = height;
						g_cusWidth = wTemp;
						g_cusHeight = height;
					}
					else
					{
						g_dstRc.x = 0;
						g_dstRc.y = (height-hTemp)/2;
						g_dstRc.w = width;
						g_dstRc.h = hTemp;
						g_cusWidth = width;
						g_cusHeight = hTemp;
					}
					/*UninitSDL();
					InitSDL();*/
					SDL_RenderSetViewport(g_pRender, &g_dstRc);
					LeaveCriticalSection(&g_csResizeWnd);
				}
			}
		}
		else
		{
			Sleep(100);
		}
	}

	SetEvent(g_hCloseProcess);
	WaitForSingleObject(g_decodeThread, INFINITE);
	CloseHandle(g_decodeThread);
	
	UninitSDL();

	DeleteCriticalSection(&g_cs);
	DeleteCriticalSection(&g_csResizeWnd);

	WaitForSingleObject(g_playVideoThread, INFINITE);
	CloseHandle(g_playVideoThread);
	if(g_pVideoCirc)
		delete g_pVideoCirc;
}

DWORD WINAPI newInstanceProc(LPVOID pParam)
{
	PlayVA();
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	
	while(!g_bEndProcess)
	{
		std::string msg;
		//std::cin >> msg;
		std::getline(cin, msg);
		USES_CONVERSION;
		CString str = A2T(msg.c_str());
		if(str.Find(_T("-f ")) == 0 )
		{
			if(!g_bStop)
			{
				printf("Current move is not stop, please stop!\n");
				continue;
			}
			str = str.Right(str.GetLength()-3);
			int nPos = str.Find( _T(" "));
			if(nPos < 0)
			{
				g_csFileName = str;
			}
			else
			{
				g_csFileName = str.Left(nPos);
			}
			
			str = str.Right(str.GetLength()-nPos - 1);
			if(str.Find(_T("-w ")) == 0 )
			{
				str = str.Right(str.GetLength()-3);
				nPos = str.Find( _T(" "));
				CString csWidth = str.Left(nPos);
				if(csWidth == csWidth.SpanIncluding(_T("0123456789")))
				{
					g_cusWidth = _ttoi(csWidth);
					str = str.Right(str.GetLength()-nPos - 1);
					if(str.Find(_T("-h ")) == 0 )
					{
						CString csHeight = str.Right(str.GetLength()-3);
						if(csHeight == csHeight.SpanIncluding(_T("0123456789")))
						{
							g_cusHeight = _ttoi(csHeight);
						}
						else
						{
							g_cusWidth = 0;
							g_cusHeight = 0;
						}
					}
					else
					{
						g_cusWidth = 0;
						g_cusHeight = 0;
					}
				}
				else
				{
					g_cusWidth = 0;
					g_cusHeight = 0;
				}
			}
			else
			{
				g_cusWidth = 0;
				g_cusHeight = 0;
			}
			if(g_csFileName == _T("d"))
			{
				g_csFileName = _T("D:/TheCroods.mkv");
			}
			g_hInstanceThread = CreateThread(NULL, 0, newInstanceProc, 0, 0, 0);
		}
		else if(str.Find(_T("-m ")) == 0 )
		{
			str = str.Right(str.GetLength()-3);
			if(str == str.SpanIncluding(_T("0123456789")))
			{
				g_seek_pos = _ttoi(str);
				g_emSeekType = emMinute;
				g_bSeek = true;
			}
		}
		else if(str.Find(_T("-s ")) == 0 )
		{
			str = str.Right(str.GetLength()-3);
			if(str == str.SpanIncluding(_T("0123456789")))
			{
				g_seek_pos = _ttoi(str);
				g_emSeekType = emSecond;
				g_bSeek = true;
			}
		}
		else if(str.Find(_T("-c ")) == 0 )
		{
			str = str.Right(str.GetLength()-3);
			if(str == str.SpanIncluding(_T("0123456789")))
			{
				int num = _ttoi(str);
				if(num < g_nMaxAudioChn)
				{
					if(num != g_audioChn)
					{
						g_nSwitchChn = true;
						g_audioChn = num;
					}
				}
			}
		}
		else if(str.Find(_T("-b ")) == 0 )
		{
			str = str.Right(str.GetLength()-3);
			if(str == str.SpanIncluding(_T("0123456789")))
			{
				int num = _ttoi(str);
				if(num == 0)
				{
					SDL_SetWindowBordered(g_pWin, SDL_FALSE);
				}
				else if(num == 1)
				{
					SDL_SetWindowBordered(g_pWin, SDL_TRUE);
				}
			}
			
		}
		else if( 0 == strcmp(msg.c_str(), "quit"))
		{
			if(!g_bStop)
			{
				printf("Video is not stop! Please stop!\n");
				continue;
			}
			g_bEndProcess = true;
			WaitForSingleObject(g_hInstanceThread, INFINITE);
			CloseHandle(g_hInstanceThread);
			break;
		}
		else if( 0 == strcmp(msg.c_str(), "stop"))
		{
			EndCurrentPlay();
			WaitForSingleObject(g_hInstanceThread, INFINITE);
			CloseHandle(g_hInstanceThread);
		}
		else if( 0 == strcmp(msg.c_str(), "ctime"))
		{
			int m = g_nCurTime/60;
			int h = m/60;
			m = m%60;
			int s = g_nCurTime%60;
			char str[256] = {0};
			sprintf_s(str, 256, "%.2dh:%.2dm:%.2ds\n",h, m, s);
			printf(str);
		}
		else
		{
			printf("command is incrroct!\n");
		}
	}
	
	return 0;
}

