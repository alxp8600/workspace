
// FFMPEG_SDL_MFC_PLAYER_1Dlg.h : header file
//

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libavutil/avutil.h"
#include "libavutil/mathematics.h"
#include "inttypes.h"
#include "SDL.h"
#include "SDL_thread.h"
};
#include "afxcmn.h"
#include "afxwin.h"
#undef main

#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"avdevice.lib")
#pragma comment(lib,"avfilter.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"postproc.lib")
#pragma comment(lib,"swresample.lib")
#pragma comment(lib,"swscale.lib")

#pragma comment(lib,"SDL.lib")
//���ﲻ��������� ľ���� �����д���
//#pragma comment(lib,"SDLmain.lib")


#define SDL_AUDIO_BUFFER_SIZE 1024
#define VIDEO_PICTURE_QUEUE_SIZE 1
#define MAX_QUEUE_SIZE (15 * 1024 * 1024)
#define MIN_FRAMES      5
#define AV_NOSYNC_THRESHOLD 10.0
#define VIDEO_ID 100
#define AUDIO_ID 101
#define MFCMINWINDOWSIZE_WIDTH  600
#define MFCMINWINDOWSIZE_HEIGHT 300
#define BMPPATHNAME _T("..//BMP_RESOURCES/22.bmp")


//?? ��������п��Ż��ĵط�
//1:?? seek����ļ�����ʱ������Ż���video->clock�����ļ���ǰ���ŵ�ʱ�� ���ļ�����ʱ�䲻ƥ��
//2:?? ��ʱ2�����ϴ��ļ��Ῠס���ж� ���Ż����Ѿ����٣�
//3:?? ��ͣʱ�ı䴰�ڴ�С��ͼ�񲻻���Ÿı� 

typedef struct PacketQueue 
{
	AVPacketList *first_pkt, *last_pkt;
	int nb_packets;
	int size;
	SDL_mutex *mutex;
	SDL_cond *cond;
} PacketQueue;

typedef struct VideoPicture
{
	SDL_Overlay *bmp;
	int width, height; /* source height & width */
	int allocated;
	double pts;
} VideoPicture;


typedef struct StreamState_t 
{
	AVFormatContext *pFormatCtx;
	AVStream        *audio_st;
	AVStream        *video_st;
	PacketQueue     audioq;
	PacketQueue     videoq;
	uint8_t         audio_buf[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2];
	unsigned int    audio_buf_size;
	unsigned int    audio_buf_index;
	AVPacket        audio_pkt;
	uint8_t         *audio_pkt_data;
	int             audio_pkt_size;
	VideoPicture    pictq[VIDEO_PICTURE_QUEUE_SIZE];
	int             pictq_size, pictq_rindex, pictq_windex;
	SDL_mutex       *pictq_mutex;     //�������
	SDL_cond        *pictq_cond;      //��������
	SDL_Thread      *read_tid;        //���Ǵ��ļ���ȡ���ݷ�����е��߳�
	SDL_Thread      *video_tid;       //������Ƶ��ȡ��������߳�
	SDL_Thread      *refresh_tid;     //����ˢ���߳�
	double          audio_clock;      //��Ƶ��ʱ��� 
	double          video_clock;      //��Ƶ��ʱ���
	double          seek_time;        //Ҫ�ƶ���ʱ�䣨�룩
	int             seek_req;         //seek�ı�־ �Ƿ���Ҫseek
	int             seek_flags;       //seek�ķ�ʽ AVSEEK_FLAG_FRAME��
	int64_t         seek_pos;         //seek�����ʱ��
} StreamState;



// CFFMPEG_SDL_MFC_PLAYER_1Dlg dialog
class CFFMPEG_SDL_MFC_PLAYER_1Dlg : public CDialog
{
// Construction
public:
	CFFMPEG_SDL_MFC_PLAYER_1Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FFMPEG_SDL_MFC_PLAYER_1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	//MFCĬ�ϵ�

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	//�Լ���ӵ�

public:
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonPlayPause();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy); //�����ڴ�С����ʱ����
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI); //�����ǿ��ƴ�����Сֵ�ģ���������Сʱ�ᵼ�¿ؼ��ص�
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point); //˫��CSTATIC�ؼ�ȫ��
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar); //����������
	afx_msg void OnBnClickedCheckMute();     //���þ�����ѡ��
	//��ȡ�ؼ����ļ��е�λ��
	int m_IsAccess_control_position;        //�Ƿ��Ѿ���ȡ�˿ؼ��ĳ�ʼ��λ��

	int m_width_MAINFRAME;                  //�����ڵĿ�
	int m_height_BMAINFRAME;                //�����ڵĸ�

	CRect m_rect_BUTTON_LOAD;               //���ذ�ť��ʼ����λ��
	int m_width_BUTTON_LOAD;                //���ذ�ť�Ŀ�
	int m_height__BUTTON_LOAD;              //���ذ�ť�ĸ�

	CRect m_rect_BUTTON_OPEN;               //�򿪰�ť��ʼ����λ��
	int m_width_BUTTON_OPEN;                //�򿪰�ť�Ŀ�
	int m_height__BUTTON_OPEN;              //�򿪰�ť�ĸ�

	CRect m_rect_BUTTON_PLAY_PAUSE;         //������ͣ��ť��ʼ����λ��
	int m_width_BUTTON_PLAY_PAUSE;          //������ͣ��ť�Ŀ�
	int m_height__BUTTON_PLAY_PAUSE;        //������ͣ��ť�ĸ�

	CRect m_rect_BUTTON_STOP;               //ֹͣ��ť��ʼ����λ��
	int m_width_BUTTON_STOP;                //ֹͣ��ť�Ŀ�
	int m_height__BUTTON_STOP;              //ֹͣ��ť�ĸ�

	CRect m_rect_BUTTON_CANCEL;             //�˳���ť��ʼ����λ��
	int m_width_BUTTON_CANCEL;              //�˳���ť�Ŀ�
	int m_height__BUTTON_CANCEL;            //�˳���ť�ĸ�

	CRect m_rect_CHECK_MUTE;                //������ѡ���ʼ����λ��
	int m_width_CHECK_MUTE;                 //������ѡ��Ŀ�
	int m_height_CHECK_MUTE;                //������ѡ��ĸ�

	CRect m_rect_SLIDER_VOLUME;             //�������Ƴ�ʼ����λ��
	int m_width_SLIDER_VOLUME;              //�������ƵĿ�
	int m_height_SLIDER_VOLUME;             //�������Ƶĸ�

	CRect m_rect_LIST_CONTROL;              //��ʾ��Ƶ��Ϣ���б��س�ʼ����λ��
	int m_width_LIST_CONTROL;               //��ʾ��Ƶ��Ϣ���б��صĿ�
	int m_height_LIST_CONTROL;              //��ʾ��Ƶ��Ϣ���б��صĸ�

	CRect m_rect_CURRENT_TIME;              //��ǰ����ʱ���ʼ����λ��
	int m_width_CURRENT_TIME;               //��ǰ����ʱ��Ŀ�
	int m_height_CURRENT_TIME;              //��ǰ����ʱ��ĸ�

	CRect m_rect_ALL_TIME;                  //�ļ���ʱ����ʼ����λ��
	int m_width_ALL_TIME;                   //�ļ���ʱ���Ŀ�
	int m_height_ALL_TIME;                  //�ļ���ʱ���ĸ�

	CRect m_rect_SLIDER_PLAYL;              //��������ʼ����λ��
	int m_width_SLIDER_PLAYL;               //�������Ŀ�
	int m_height_SLIDER_PLAY;               //�������ĸ�

	CRect m_rect_PICURE_CONTROL;            //���������ʼ����λ��
	int m_width_PICURE_CONTROL;             //��������Ŀ�
	int m_height_PICURE_CONTROL;            //��������ĸ�

	//��ȡ�ؼ���ʼ��λ�� //ֻ����һ��
	int Access_control_position();      
	//�����������ĳ�ʼ�� �򿪱����� SDL��ʼ����,����Ĭ�ϴ���  //ֻ����һ��
	int InitProgram(); 
	//�����˳����������رտ����ٴ��ڵ�  //ֻ����һ��
	int UinitProgram();
	//��������Ҫ�ı��������
	int InitVariable();
	//��������Ҫ�ı���������
	int UinitVariable();
	//���ļ����ҵ������
	int Open_codec_context(int * stream_idx, AVFormatContext * fmt_ctx, enum AVMediaType type); 
	//���� ������Ƶ�ص���������Ƶ�߳�
	int stream_component_open(StreamState *is, int stream_index);
	//�ر���Ƶ �ر���Ƶ�豸
	int stream_component_close(StreamState *is, int stream_index); 
	//ʼ������
	void packet_queue_init(PacketQueue *q) ;   
	//��packet������У�����ֻ�����ĺ�ˢ��packet�жϣ�
	int packet_queue_put(PacketQueue *q, AVPacket *pkt);   
	//�Ӷ�������ȡ������
	int packet_queue_get(PacketQueue *q, AVPacket *pkt, int queue_type);
	//��ֹ������������
	void packet_queue_abort(PacketQueue *q);             
	//������������к���
	int packet_queue_put_private(PacketQueue *q, AVPacket *pkt);    
	//ˢ��packet�ĳ�ʼ��
	void packet_queue_start(PacketQueue *q); 
	//���ٶ��� 
	void packet_queue_destroy(PacketQueue *q);     
	//��ն���
	void packet_queue_flush(PacketQueue *q); 
	//ˢ�²�������Ƶ��
	void video_refresh(void *opaque);  
	//������Ƶ����
	int audio_decode_frame(StreamState *is, uint8_t *audio_buf, int buf_size,double *pts_ptr);  
	//��������������ӵ�VideoPicture�ṹ����
	int queue_picture(StreamState *is, AVFrame *pFrame, double pts); 
	//��ʾ��Ƶ
	void video_display(StreamState *is);  
	//�������SDL_Overlay yuv����
	void alloc_picture(void *userdata) ;
	//�����򿪴������ô�С
	int video_open(StreamState *is);                                         
public:
	StreamState  * m_streamstate;            //����Ƶȫ�ֽṹ��
	int m_fs_screen_width;                   //ȫ���Ŀ�
	int m_fs_screen_height;                  //ȫ���ĸ�
	AVPacket m_flush_pkt;                    //ˢ��packet
	CString m_sourceFile;                    //Ҫ�򿪵��ļ�·��
	double m_file_duration;                  //�ļ���ʱ��
	AVCodecContext * m_video_dec_ctx ;       //��Ƶ����context
	AVCodecContext * m_audio_dec_ctx ;       //��Ƶ����context
	AVCodec * m_pvideo_codec;                //��Ƶ������
	AVCodec * m_paudio_codec;                //��Ƶ������
	SDL_Surface * m_screen;                  //sdl ��Ļ��ʾ�ṹ��
	int m_is_full_screen;                    //�Ƿ�ȫ������
	int m_screen_width ;                     //�ı��С��Ŀ�͸�
	int m_screen_height;                     //�ı��С��Ŀ�͸�
	int m_Isstop;                            //�Ƿ�ֹͣ 0��ʾ���� 1��ʾֹͣ
	int m_pause_play;                        //0��ʾ��ͣ 1��ʾ����
	int m_slider_pos ;                       //���Ź�����Ҫ����λ��
	int m_stream_type;                       //�ļ����� 1������Ƶ��������2��ֻ������Ƶ 3��ֻ������Ƶ
	//video pram
	int m_video_stream_idx;                  //��Ƶ���ļ��е������
	double m_dbFrameRate;                    //��Ƶ֡��
	double m_video_duration;                 //��Ƶ��֡����ʱ�� �루s��
	int m_dwWidth;                           //��
	int m_dwHeight;                          //��
	AVCodecID m_video_codecID;               //��Ƶ��������
	AVPixelFormat m_video_pixelfromat;       //��Ƶyuv����
	char m_spspps[1000];                      //spspps_buf
	int m_spspps_size;                       //spspps_size
	//audio pram
	int m_audio_stream_idx;                  //��Ƶ���ļ��е������
	double m_audio_duration;                 //��Ƶ��֡����ʱ�� �루s��
	int m_dwChannelCount;                    //����
	int m_dwBitsPerSample;                   //����
	int m_dwFrequency;                       //������
	AVCodecID m_audio_codecID;               //��Ƶ��������
	int m_audio_frame_size;                  //һ֡���ݴ�С
	int m_IsEnd_audio;                       //��Ƶ�Ƿ񲥷���� 0 δ������� 1�������
	int m_Volume_pos;                        //�������Ʊ��� 0-128
public:
	CListCtrl m_listControl;                 //��ʾ��Ƶ��Ϣ���б��ؼ�����
	CSliderCtrl m_sliderPlay;                //����������
	CStatic m_picture_static;                //��������ָ��
	CRect m_Picture_rect;                    //��ʾͼ��������С
	CSliderCtrl m_sliderVolume;              //����������
};

int read_thread( LPVOID lpParam );           //���ļ���ȡ����Ƶ ����ˢ���߳�
int refresh_thread(LPVOID lpParam);          //ˢ���߳�
void audio_callback(void *userdata, unsigned char *stream, int len) ;   //��Ƶ�ص�����
int video_thread(void *arg);                                            //��Ƶ�����߳̽����������ݴ���VideoPicture�ṹ��