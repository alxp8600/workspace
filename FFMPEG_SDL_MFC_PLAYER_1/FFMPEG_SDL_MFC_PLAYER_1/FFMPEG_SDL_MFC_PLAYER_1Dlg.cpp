
// FFMPEG_SDL_MFC_PLAYER_1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "FFMPEG_SDL_MFC_PLAYER_1.h"
#include "FFMPEG_SDL_MFC_PLAYER_1Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFFMPEG_SDL_MFC_PLAYER_1Dlg dialog


CFFMPEG_SDL_MFC_PLAYER_1Dlg::CFFMPEG_SDL_MFC_PLAYER_1Dlg(CWnd* pParent /*=NULL*/)
: CDialog(CFFMPEG_SDL_MFC_PLAYER_1Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFFMPEG_SDL_MFC_PLAYER_1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CONTROL_INFO, m_listControl);
	DDX_Control(pDX, IDC_SLIDER_PLAY, m_sliderPlay);
	DDX_Control(pDX, IDC_PICURE_CONTROL, m_picture_static);
	DDX_Control(pDX, IDC_SLIDER_VOLUME, m_sliderVolume);
}

BEGIN_MESSAGE_MAP(CFFMPEG_SDL_MFC_PLAYER_1Dlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CFFMPEG_SDL_MFC_PLAYER_1Dlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CFFMPEG_SDL_MFC_PLAYER_1Dlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_PLAY_PAUSE, &CFFMPEG_SDL_MFC_PLAYER_1Dlg::OnBnClickedButtonPlayPause)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CFFMPEG_SDL_MFC_PLAYER_1Dlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDCANCEL, &CFFMPEG_SDL_MFC_PLAYER_1Dlg::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CHECK_MUTE, &CFFMPEG_SDL_MFC_PLAYER_1Dlg::OnBnClickedCheckMute)
END_MESSAGE_MAP()


// CFFMPEG_SDL_MFC_PLAYER_1Dlg message handlers

BOOL CFFMPEG_SDL_MFC_PLAYER_1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	//////////////////////////////////////////////////////////////////////////
	//设置列表框属性
	DWORD dwStyle = m_listControl.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl） 
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl） 
	dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	m_listControl.SetExtendedStyle(dwStyle); //设置扩展风格
	m_listControl.InsertColumn(0,_T("媒体信息"),LVCFMT_IMAGE|LVCFMT_LEFT);  //创建要显示的标题 必须有
	m_listControl.InsertColumn(0,_T("流类型"),LVCFMT_IMAGE|LVCFMT_LEFT);//创建要显示的标题 必须有
	m_listControl.SetColumnWidth(0,71);      //设置媒体信息的宽度
	m_listControl.SetColumnWidth(1,1000);    //设置媒体信息的宽度
	m_listControl.SetTextColor(RGB(0,0,255));//设计字体的颜色 蓝色
	//////////////////////////////////////////////////////////////////////////
	//初始化滚动条变量
	m_sliderPlay.SetPos(0);
	m_sliderPlay.SetPageSize(10);//滚动条每次seek的间隔 //10秒
	//////////////////////////////////////////////////////////////////////////
	//设置音量滚动条的变量
	m_sliderVolume.SetPos(0);
	m_sliderVolume.SetPageSize(10); 
	//////////////////////////////////////////////////////////////////////////
	//获取控件在对话框上的坐标
	m_IsAccess_control_position = Access_control_position();
	//////////////////////////////////////////////////////////////////////////
	//初始化控件状态
	GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PLAY_PAUSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	m_sliderPlay.EnableWindow(FALSE);
	GetDlgItem(IDC_SLIDER_VOLUME)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_MUTE)->EnableWindow(FALSE);
	//////////////////////////////////////////////////////////////////////////
	//初始化CEDIT 时间控件
	GetDlgItem(IDC_EDIT_CURRENT_TIME)->SetWindowText("  --:--  ");
	GetDlgItem(IDC_EDIT_ALL_TIME)->SetWindowText("  --:--  ");
	//////////////////////////////////////////////////////////////////////////
	//初始化全局变量 编解码库 SDL初始化等
	InitProgram();
	//////////////////////////////////////////////////////////////////////////
	InitVariable();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFFMPEG_SDL_MFC_PLAYER_1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFFMPEG_SDL_MFC_PLAYER_1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFFMPEG_SDL_MFC_PLAYER_1Dlg::OnBnClickedButtonLoad()
{
	// TODO: Add your control notification handler code here
	//////////////////////////////////////////////////////////////////////////
	//这里一定要做清理工作 如果文件正在播放 重新打开一个新的文件时 要做的清理工作

	//关闭线程等
	OnBnClickedButtonStop(); 
	//初始化变量等
	InitVariable();
	Sleep(500);
	//////////////////////////////////////////////////////////////////////////
	//设置滚动条不可操作
	m_sliderPlay.EnableWindow(FALSE);
	//////////////////////////////////////////////////////////////////////////
	CString str;     //文件总时常显示的文本
	int h_minute = 0;//文件总时常显示的文本(分)
	int h_second = 0;//文件总时常显示的文本（秒）
	// 文件扩展名过滤
	CString szFilter = _T("All Files (*.*)|*.*|avi Files (*.avi)|*.avi|rmvb Files (*.rmvb)|*.rmvb|3gp Files (*.3gp)|*.3gp|mp3 Files (*.mp3)|*.mp3|mp4 Files (*.mp4)|*.mp4|mpeg Files (*.ts)|*.ts|flv Files (*.flv)|*.flv|mov Files (*.mov)|*.mov||");
	CFileDialog dlg(TRUE,NULL ,NULL,OFN_PATHMUSTEXIST|OFN_HIDEREADONLY ,szFilter,NULL);
	if (IDOK == dlg.DoModal())
	{	
		//建立视频数据源（加载视频文件）
		m_sourceFile = dlg.GetPathName();

		//打开要被应用的输入文件
		if (avformat_open_input(&m_streamstate->pFormatCtx,(LPSTR)(LPCTSTR)m_sourceFile, NULL, NULL) < 0)                 
		{
			MessageBox(_T("FFMPEG 打开文件失败 请确认文件类型和路径正确"), NULL, MB_OK );
			goto end;
		}

		//查看流信息
		if (avformat_find_stream_info(m_streamstate->pFormatCtx, NULL) < 0)                                   
		{
			int msgboxID = MessageBox(_T("FFMPEG 获取文件信息失败 是否继续"), NULL, MB_YESNO );
			if (msgboxID != IDYES)
			{
				goto end;
			}
		}
		//输入信息错误写入
		av_dump_format(m_streamstate->pFormatCtx, 0, (LPSTR)(LPCTSTR)m_sourceFile, 0); 

		//得到文件中时常
		m_file_duration = m_streamstate->pFormatCtx->duration / 1000.0 / 1000.0; //从纳秒转换成秒
	
		//如果没有文件总时常
		if (m_file_duration < 0)
		{
			m_file_duration = 0;
		}
		//将文件信息填入列表框控件变量
		CString strTemp;
		strTemp.Format(_T("File Path:%s   File duration:%f(秒)"),
			(LPSTR)(LPCTSTR)m_sourceFile,m_file_duration);

		int nIndex = m_listControl.GetItemCount();    //获取行号 
		m_listControl.InsertItem(nIndex,_T("File"),0);    //在第nIndex行 第0列 插入"file" 
		m_listControl.SetItemText(nIndex,1,strTemp);  //在第nIndex行 第1列 插入strTemp 
		m_listControl.SetCheck(nIndex);

		//得到视频信息的CONTEXT
		if (Open_codec_context(&m_video_stream_idx, m_streamstate->pFormatCtx, AVMEDIA_TYPE_VIDEO) >= 0)
		{
			m_streamstate->video_st = m_streamstate->pFormatCtx->streams[m_video_stream_idx];
			m_video_dec_ctx = m_streamstate->video_st->codec;
			// Find the decoder for the video stream
			m_pvideo_codec = avcodec_find_decoder(m_video_dec_ctx->codec_id);
			if(m_pvideo_codec == NULL) 
			{
				MessageBox(_T("Find video decoder失败 请重新加载"), NULL, MB_OK );
				goto end;
			}
			// Open codec
			if(avcodec_open(m_video_dec_ctx, m_pvideo_codec) < 0)
			{
				MessageBox(_T("Could not open video codec 请重新加载"), NULL, MB_OK );
				goto end;
			}

			//视频的帧率
			m_dbFrameRate = av_q2d(m_streamstate->video_st->r_frame_rate);
			//这里说明是裸流
			if (m_dbFrameRate < 15 || m_dbFrameRate > 50)
			{
				//这种是MP3 有图片的
				m_stream_type = 2;
				m_dbFrameRate = 25.0;
			}
			//宽高,视频编码类型,视频yuv类型,spspps_buf,spspps_size
			m_dwWidth = m_streamstate->pFormatCtx->streams[m_video_stream_idx]->codec->width;
			m_dwHeight = m_streamstate->pFormatCtx->streams[m_video_stream_idx]->codec->height;
			m_video_codecID = m_streamstate->pFormatCtx->streams[m_video_stream_idx]->codec->codec_id;
			m_video_pixelfromat = m_streamstate->pFormatCtx->streams[m_video_stream_idx]->codec->pix_fmt;
			m_spspps_size = m_streamstate->pFormatCtx->streams[m_video_stream_idx]->codec->extradata_size;
			if(m_spspps_size < 1000)
			{
				memcpy(m_spspps,m_streamstate->pFormatCtx->streams[m_video_stream_idx]->codec->extradata,m_spspps_size);
			}
			//这里加一个判断是用于对不同的格式进行pts单位的统一化：统一到 “秒”,可扩展其它格式
			if (strstr(m_streamstate->pFormatCtx->iformat->name,"mpegts")!= NULL)
			{
				double time_base = 90 * 1000;
				m_video_duration = time_base / m_dbFrameRate /100000;
			}
			else
			{
				m_video_duration = 1 / m_dbFrameRate;
			}

			//将视频信息填入列表框控件变量
			CString strTemp; //28 - AV_CODEC_ID_H264 , 0 - AV_PIX_FMT_YUV420P
			if (m_video_codecID == AV_CODEC_ID_H264 && m_video_pixelfromat == AV_PIX_FMT_YUV420P)//AV_CODEC_ID_H264 AV_PIX_FMT_YUV420P
			{
				strTemp.Format(_T("video_stream_type:AV_CODEC_ID_H264   video_pixelfromat:AV_PIX_FMT_YUV420P   duration:%f(秒)   width:%d   height:%d   framerate:%f"),
					m_video_duration,m_dwWidth,m_dwHeight,m_dbFrameRate);
			}
			else if (m_video_codecID == AV_CODEC_ID_MPEG4 && m_video_pixelfromat == AV_PIX_FMT_YUV420P)
			{
				strTemp.Format(_T("video_stream_type:AV_CODEC_ID_MPEG4   video_pixelfromat:AV_PIX_FMT_YUV420P   duration:%f(秒)   width:%d   height:%d   framerate:%f"),
					m_video_duration,m_dwWidth,m_dwHeight,m_dbFrameRate);
			}
			//剩下的自己扩展
			else
			{
				strTemp.Format(_T("video_stream_type:%d   video_pixelfromat:%d   duration:%f(秒)   width:%d   height:%d   framerate:%f"),
					m_video_codecID,m_video_pixelfromat,m_video_duration,m_dwWidth,m_dwHeight,m_dbFrameRate);
			}
			int nIndex	= m_listControl.GetItemCount();    //获取行号 
			m_listControl.InsertItem(nIndex,_T("Video"),0);   //在第nIndex行 第0列 插入"video" 
			m_listControl.SetItemText(nIndex,1,strTemp);  //在第nIndex行 第1列 插入strTemp 
			m_listControl.SetCheck(nIndex);
		}
		//得到音频信息的CONTEXT
		if (Open_codec_context(&m_audio_stream_idx, m_streamstate->pFormatCtx, AVMEDIA_TYPE_AUDIO) >= 0) 
		{
			m_streamstate->audio_st = m_streamstate->pFormatCtx->streams[m_audio_stream_idx];
			m_audio_dec_ctx = m_streamstate->audio_st->codec;
			// Find the decoder for the video stream
			m_paudio_codec = avcodec_find_decoder(m_audio_dec_ctx->codec_id);
			if(m_paudio_codec == NULL) 
			{
				MessageBox(_T("Find audio decoder失败 请重新加载"), NULL, MB_OK );
				goto end;
			}

			// Open codec
			if(avcodec_open(m_audio_dec_ctx, m_paudio_codec) < 0)
			{
				MessageBox(_T("Could not open ayudio codec 请重新加载"), NULL, MB_OK );
				goto end;
			}

			//声道，样本，采样率，视频编码类型，一帧数据大小
			m_dwChannelCount = m_streamstate->pFormatCtx->streams[m_audio_stream_idx]->codec->channels;
			switch (m_streamstate->pFormatCtx->streams[m_audio_stream_idx]->codec->sample_fmt)
			{
			case AV_SAMPLE_FMT_U8:
				m_dwBitsPerSample  = 8;
				break;
			case AV_SAMPLE_FMT_S16:
				m_dwBitsPerSample  = 16;
				break;
			case AV_SAMPLE_FMT_S32:
				m_dwBitsPerSample  = 32;
				break;
			default:
				break;
			}
			m_dwFrequency = m_streamstate->pFormatCtx->streams[m_audio_stream_idx]->codec->sample_rate;
			m_audio_codecID = m_streamstate->pFormatCtx->streams[m_audio_stream_idx]->codec->codec_id;
			m_audio_frame_size = m_streamstate->pFormatCtx->streams[m_audio_stream_idx]->codec->frame_size;

			//这里加一个判断是用于对不同的格式进行pts单位的统一化：统一到 “秒”,可扩展其它格式
			if (strstr(m_streamstate->pFormatCtx->iformat->name,"mpegts")!= NULL)
			{
				double time_base = 90 * 1000;
				m_audio_duration = time_base * m_audio_frame_size / m_dwFrequency /100000;
			}
			else
			{
				double time_base = 1.0;
				m_audio_duration = time_base * m_audio_frame_size / m_dwFrequency;
			}

			//将音频信息填入列表框控件变量
			CString strTemp; //86018-AV_CODEC_ID_AAC
			if (m_audio_codecID == AV_CODEC_ID_AAC)
			{
				strTemp.Format(_T("audio_stream_type:AV_CODEC_ID_AAC   duration:%f(秒)   channel:%d   BitsPerSample:%d   sample_rate:%d   frame_size:%d"),
					m_audio_duration,m_dwChannelCount,m_dwBitsPerSample,m_dwFrequency,m_audio_frame_size);
			}
			else if (m_audio_codecID == AV_CODEC_ID_MP3)
			{
				strTemp.Format(_T("audio_stream_type:AV_CODEC_ID_MP3   duration:%f(秒)   channel:%d   BitsPerSample:%d   sample_rate:%d   frame_size:%d"),
					m_audio_duration,m_dwChannelCount,m_dwBitsPerSample,m_dwFrequency,m_audio_frame_size);
			}
			//剩下的自己扩展
			else
			{
				strTemp.Format(_T("audio_stream_type:%d   duration:%f(秒)   channel:%d   BitsPerSample:%d   sample_rate:%d   frame_size:%d"),
					m_audio_codecID,m_audio_duration,m_dwChannelCount,m_dwBitsPerSample,m_dwFrequency,m_audio_frame_size);
			}
			int nIndex = m_listControl.GetItemCount();    //获取行号 
			m_listControl.InsertItem(nIndex,_T("Audio"),0);   //在第nIndex行 第0列 插入"audio" 
			m_listControl.SetItemText(nIndex,1,strTemp);  //在第nIndex行 第1列 插入strTemp 
			m_listControl.SetCheck(nIndex);
		}
	}
	else
	{
		MessageBox(_T("获取文件名称失败 请重新加载"), NULL, MB_OK );
		goto end;
	}

	//判断文件的 类型 如果还没做判断
	if (m_stream_type == 0)
	{
		//音视频都包含
		if (m_audio_stream_idx >= 0 && m_video_stream_idx >= 0)
		{
			m_stream_type = 1;
		}
		//只有音频
		else if (m_audio_stream_idx >= 0)
		{
			m_stream_type = 2;
		}
		//只有视频
		else if (m_video_stream_idx >= 0)
		{
			m_stream_type = 3;
		}
		else 
		{
			MessageBox(_T("文件有错误 请重新加载"), NULL, MB_OK );
			goto end;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//文件打开设置文件时常
	h_minute = (int)m_file_duration / 60;
	h_second = (int)m_file_duration % 60;
	if (h_minute < 10 && h_second < 10)
	{
		str.Format("  0%d:0%d  ",h_minute,h_second);
	}
	else if (h_minute < 10 && h_second >= 10)
	{
		str.Format("  0%d:%d  ",h_minute,h_second);
	}
	else if (h_minute >= 10 && h_second < 10)
	{
		str.Format("  %d:0%d  ",h_minute,h_second);
	}
	else
	{
		str.Format("  %d:%d  ",h_minute,h_second);
	}
	GetDlgItem(IDC_EDIT_CURRENT_TIME)->SetWindowText("  00:00  ");
	GetDlgItem(IDC_EDIT_ALL_TIME)->SetWindowText(str);
	//////////////////////////////////////////////////////////////////////////
	//设置OPEN控件可操作
	GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(TRUE);
	//添加标题
	this->SetWindowText(m_sourceFile);
end:
	{
		//////////////////////////////////////////////////////////////////////////
		//这里可以做一些其他工作
		//////////////////////////////////////////////////////////////////////////
	}
}

void CFFMPEG_SDL_MFC_PLAYER_1Dlg::video_refresh(void * opaque)
{
	StreamState *streamstate = (StreamState *)opaque;
	if (streamstate == NULL)
	{
		return;
	}
	VideoPicture *vp;
	double delay,diff;

//////////////////////////////////////////////////////////////////////////
	//设置当前时间显示
	CString str; 
	int h_minute = 0;
	int h_second = 0;

	//如果只有视频
	if (m_stream_type == 3)
	{
		h_minute = (int)m_streamstate->video_clock / 60;
		h_second = (int)m_streamstate->video_clock % 60;
		if (h_minute < 10 && h_second < 10)
		{
			str.Format("  0%d:0%d  ",h_minute,h_second);
		}
		else if (h_minute < 10 && h_second >= 10)
		{
			str.Format("  0%d:%d  ",h_minute,h_second);
		}
		else if (h_minute >= 10 && h_second < 10)
		{
			str.Format("  %d:0%d  ",h_minute,h_second);
		}
		else
		{
			str.Format("  %d:%d  ",h_minute,h_second);
		}
		if (m_Isstop) //如果已经停止
		{
			GetDlgItem(IDC_EDIT_CURRENT_TIME)->SetWindowText("  --:--  ");
			//进度条随着时间滚动
			m_sliderPlay.SetPos(0);
		}
		else
		{
			//////////////////////////////////////////////////////////////////////////
			GetDlgItem(IDC_EDIT_CURRENT_TIME)->SetWindowText(str);
			//进度条随着时间滚动
			if (m_pause_play)
			{
				m_sliderPlay.SetPos(m_streamstate->video_clock);
			}
		}
	}
	//如果只有音频 或都有
	else 
	{
		h_minute = (int)m_streamstate->audio_clock / 60;
		h_second = (int)m_streamstate->audio_clock % 60;
		if (h_minute < 10 && h_second < 10)
		{
			str.Format("  0%d:0%d  ",h_minute,h_second);
		}
		else if (h_minute < 10 && h_second >= 10)
		{
			str.Format("  0%d:%d  ",h_minute,h_second);
		}
		else if (h_minute >= 10 && h_second < 10)
		{
			str.Format("  %d:0%d  ",h_minute,h_second);
		}
		else
		{
			str.Format("  %d:%d  ",h_minute,h_second);
		}
		if (m_Isstop) //如果已经停止
		{
			GetDlgItem(IDC_EDIT_CURRENT_TIME)->SetWindowText("  --:--  ");
			//进度条随着时间滚动
			m_sliderPlay.SetPos(0);
		}
		else
		{
			//////////////////////////////////////////////////////////////////////////
			GetDlgItem(IDC_EDIT_CURRENT_TIME)->SetWindowText(str);
			//进度条随着时间滚动
			if (m_pause_play)
			{
				m_sliderPlay.SetPos(m_streamstate->audio_clock);
			}
		}
	}
//////////////////////////////////////////////////////////////////////////

	if(streamstate->video_st)
	{
		if(streamstate->pictq_size == 0)
		{
			Sleep(1);
		} 
		else
		{
			//取出要显示视频的图像数据
			vp = &streamstate->pictq[streamstate->pictq_rindex];
			//算出一帧视频本应该显示的时间
			delay = m_video_duration;
			/* update delay to sync to audio */
			diff = vp->pts - streamstate->audio_clock;

			if(fabs(diff) < AV_NOSYNC_THRESHOLD) //求浮点数x的绝对值
			{
				if(diff <= -delay) 
				{
					delay = 0.01;       //如果 视频显示过慢，离音频 过于远 则 显示时间为10ms
				} 
				else if(diff >= delay)
				{
					delay = 2 * delay;  //如果 视频显示过快 则停留 两帧的时间
				}
			}
			//如果只有视频 而且是裸流 
			if (m_stream_type == 3 && m_dbFrameRate == 25 )
			{
				//这里是刷新时间间隔
				Sleep(m_video_duration * 1000);
			}
			else
			{
				Sleep(delay * 1000);
			}
			/* update queue for next picture! */
			if(++streamstate->pictq_rindex == VIDEO_PICTURE_QUEUE_SIZE) 
			{
				streamstate->pictq_rindex = 0;
			}
			SDL_LockMutex(streamstate->pictq_mutex);
			streamstate->pictq_size--;
			SDL_CondSignal(streamstate->pictq_cond);
			SDL_UnlockMutex(streamstate->pictq_mutex);

			/* show the picture! */
			video_display(streamstate);  
		}
	}
	
    else if (streamstate->audio_st) 
	{
        /* draw the next audio frame */

        /* if only audio stream, then display the audio bars (better
           than nothing, just to test the implementation */
		video_display(streamstate);
    }
}

void CFFMPEG_SDL_MFC_PLAYER_1Dlg::video_display(StreamState *m_streamstate) 
{
	SDL_Rect rect;
	VideoPicture *vp;

	if (!m_screen)
	{
		video_open(m_streamstate); 
	}
	vp = &m_streamstate->pictq[m_streamstate->pictq_rindex];
	if(vp->bmp) 
	{
		if (vp->bmp->w != m_screen_width || vp->bmp->h != m_screen_height)
		{
			//这里一定要注意 当sdl的窗口大小改变的时候 切换那一瞬间 
			//m_streamstate->pictq[m_streamstate->pictq_rindex] 这里的数据有可能还是原来窗口大小的数据 
			//这时将这一真不花到sdl窗口上防止窗口死掉。也可以用get_next_frame控制
			return ;
		}
		rect.x = 0;
		rect.y = 0;
		rect.w = m_screen_width;
		rect.h = m_screen_height;
		SDL_DisplayYUVOverlay(vp->bmp, &rect);
	}
}

int refresh_thread(LPVOID lpParam)
{
	CFFMPEG_SDL_MFC_PLAYER_1Dlg * pDlg = (CFFMPEG_SDL_MFC_PLAYER_1Dlg *)lpParam;
	ASSERT(pDlg);

	while (!pDlg->m_Isstop) 
	{
		//添加刷新函数
		pDlg->video_refresh(pDlg->m_streamstate);
		//FIXME ideally we should wait the correct time but SDLs event passing m_streamstate so slow it would be silly
	}
	return 0;
}

void audio_callback(void * userdata, unsigned char *stream, int len) 
{
	CFFMPEG_SDL_MFC_PLAYER_1Dlg * pDlg = (CFFMPEG_SDL_MFC_PLAYER_1Dlg *)userdata;
	ASSERT(pDlg);
	int len1, audio_size;
	double pts;

	while(len > 0) 
	{
		if(pDlg->m_streamstate->audio_buf_index >= pDlg->m_streamstate->audio_buf_size)
		{
			/* We have already sent all our data; get more */
			int size = sizeof(pDlg->m_streamstate->audio_buf);
			audio_size = pDlg->audio_decode_frame(pDlg->m_streamstate, pDlg->m_streamstate->audio_buf, sizeof(pDlg->m_streamstate->audio_buf),&pts);
			if(audio_size < 0)
			{
				/* If error, output silence */
				pDlg->m_streamstate->audio_buf_size = 1024;
				memset(pDlg->m_streamstate->audio_buf, 0, pDlg->m_streamstate->audio_buf_size);
			}
			else
			{
				pDlg->m_streamstate->audio_buf_size = audio_size;
			}
			pDlg->m_streamstate->audio_buf_index = 0;
		}
		len1 = pDlg->m_streamstate->audio_buf_size - pDlg->m_streamstate->audio_buf_index;
		if(len1 > len)
		{
			len1 = len;
		}
		//////////////////////////////////////////////////////////////////////////
		//memcpy(stream, (unsigned char  *)pDlg->m_streamstate->audio_buf + pDlg->m_streamstate->audio_buf_index, len1);
		//这里是控制音量大小的
		SDL_MixAudio(stream, (unsigned char  *)pDlg->m_streamstate->audio_buf + pDlg->m_streamstate->audio_buf_index, len1, pDlg->m_Volume_pos);
		//////////////////////////////////////////////////////////////////////////
		len -= len1;
		stream += len1;
		pDlg->m_streamstate->audio_buf_index += len1;
	}
}

int CFFMPEG_SDL_MFC_PLAYER_1Dlg::audio_decode_frame(StreamState *m_streamstate, uint8_t *audio_buf, int buf_size,double *pts_ptr) 
{
	int len1, data_size, n;
	AVPacket *pkt = &m_streamstate->audio_pkt;
	m_streamstate->audio_pkt_size  = 0;
	double pts;

	for(;;) 
	{
		if (m_Isstop || m_IsEnd_audio )
		{
			return -1;
		}
		while(m_streamstate->audio_pkt_size > 0) 
		{
			data_size = buf_size;
			len1 = avcodec_decode_audio3(m_streamstate->audio_st->codec, (int16_t *)audio_buf, &data_size,pkt);
			if(len1 < 0) 
			{
				/* if error, skip frame */
				m_streamstate->audio_pkt_size = 0;
				break;
			}
			m_streamstate->audio_pkt_data += len1;
			m_streamstate->audio_pkt_size -= len1;
			if(data_size <= 0)
			{
				/* No data yet, get more frames */
				continue;
			}
			pts = m_streamstate->audio_clock;
			*pts_ptr = pts;

			//这里加一个判断是用于对不同的格式进行pts单位的统一化：统一到 “秒”，可扩展其它格式
			if (strstr(m_streamstate->pFormatCtx->iformat->name,"mpegts")!= NULL)
			{
				double time_base = 90 * 1000;
				n = 2 * m_streamstate->audio_st->codec->channels;
				double rate_size = data_size / n;
				m_streamstate->audio_clock +=  rate_size * time_base/ m_streamstate->audio_st->codec->sample_rate /100000;
			}
			else
			{
				n = 2 * m_streamstate->audio_st->codec->channels;
				double rate_size = data_size / n;
				m_streamstate->audio_clock +=  rate_size / m_streamstate->audio_st->codec->sample_rate;
			}
			/* We have data, return it and come back for more later */
			return data_size;
		}
		if(pkt->data)
		{
			av_free_packet(pkt);
		}

		if (m_pause_play)
		{
			/* next packet */
			if(packet_queue_get(&m_streamstate->audioq, pkt, AUDIO_ID) < 0)
			{
				return -1;
			}
			if(pkt->data == m_flush_pkt.data) 
			{
				avcodec_flush_buffers(m_streamstate->audio_st->codec);
				continue;
			}
			m_streamstate->audio_pkt_data = pkt->data;
			m_streamstate->audio_pkt_size = pkt->size;
		}
	}
}

int video_thread(void *arg)
{
	CFFMPEG_SDL_MFC_PLAYER_1Dlg * pDlg = (CFFMPEG_SDL_MFC_PLAYER_1Dlg *)arg;
	ASSERT(pDlg);

	//AVPacket pkt1, *packet = &pkt1;
	int frameFinished;
	AVFrame * pFrame = NULL;

	pFrame = avcodec_alloc_frame();

	for(;;) 
	{
		AVPacket pkt;
		if (pDlg->m_Isstop)
		{
			break;
		}
		if (pDlg->m_pause_play) //如果正在播放
		{
			if(pDlg->packet_queue_get(&pDlg->m_streamstate->videoq, &pkt, VIDEO_ID) < 0)
			{
				// means we quit getting packets
				break;
			}
			if(pkt.data == pDlg->m_flush_pkt.data) 
			{
				avcodec_flush_buffers(pDlg->m_streamstate->video_st->codec);
				continue;
			}
			// Decode video frame
			avcodec_decode_video2(pDlg->m_streamstate->video_st->codec, pFrame, &frameFinished,&pkt);
			av_free_packet(&pkt);

			// Did we get a video frame?
			if(frameFinished) 
			{
				pDlg->m_streamstate->video_clock +=  pDlg->m_video_duration;
				if(pDlg->queue_picture(pDlg->m_streamstate, pFrame,pDlg->m_streamstate->video_clock) < 0)  
				{
					break;
				}
			}
		}
		else
		{
			continue;
		}
	}
	avcodec_free_frame(&pFrame);
	return 0;
}

int CFFMPEG_SDL_MFC_PLAYER_1Dlg::queue_picture(StreamState *m_streamstate, AVFrame *pFrame,double pts) 
{
	VideoPicture * vp = NULL;
	AVFrame * pOutputFrame = NULL;
	uint8_t * pOutput_buf = NULL;
	int Out_size = 0;
	struct SwsContext * img_convert_ctx;

	pOutputFrame = avcodec_alloc_frame(); 

	/* wait until we have space for a new pic */
	SDL_LockMutex(m_streamstate->pictq_mutex);
	while (m_streamstate->pictq_size >= VIDEO_PICTURE_QUEUE_SIZE ) 
	{
		SDL_CondWait(m_streamstate->pictq_cond, m_streamstate->pictq_mutex);
	}
	SDL_UnlockMutex(m_streamstate->pictq_mutex);

	// windex m_streamstate set to 0 initially
	vp = &m_streamstate->pictq[m_streamstate->pictq_windex];

	/* allocate or resize the buffer! */
	if(!vp->bmp ||
		vp->width != m_screen_width ||
		vp->height != m_screen_height) 
	{
		SDL_Event event;

		vp->allocated = 0;
		/* we have to do it in the main thread */
		alloc_picture(m_streamstate);
		/* wait until we have a picture allocated */
		SDL_LockMutex(m_streamstate->pictq_mutex);
		while (!vp->allocated) 
		{
			SDL_CondWait(m_streamstate->pictq_cond, m_streamstate->pictq_mutex);
		}
		/* if the queue m_streamstate aborted, we have to pop the pending ALLOC event or wait for the allocation to complete */
		//if (SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_EVENTMASK(FF_ALLOC_EVENT)) != 1)
		//{
		//	while (!vp->allocated) 
		//	{
		//		SDL_CondWait(m_streamstate->pictq_cond, m_streamstate->pictq_mutex);
		//	}
		//}
		SDL_UnlockMutex(m_streamstate->pictq_mutex);
	}
	/* We have a place to put our picture on the queue */

	if(vp->bmp) 
	{

		SDL_LockYUVOverlay(vp->bmp);
		/* point pict at the queue */

		pOutputFrame->data[0] = vp->bmp->pixels[0];
		pOutputFrame->data[1] = vp->bmp->pixels[2];
		pOutputFrame->data[2] = vp->bmp->pixels[1];

		pOutputFrame->linesize[0] = vp->bmp->pitches[0];
		pOutputFrame->linesize[1] = vp->bmp->pitches[2];
		pOutputFrame->linesize[2] = vp->bmp->pitches[1];

		img_convert_ctx = sws_getContext(pFrame->width,pFrame->height, m_streamstate->video_st->codec->pix_fmt,
			m_screen_width,m_screen_height,AV_PIX_FMT_YUV420P,SWS_BICUBIC, NULL,NULL,NULL);      
		if(img_convert_ctx == NULL)
		{
			fprintf(stderr, "Cannot initialize the conversion context!\n");
			getchar();
		}

		// 将图片转换为RGB格式
		sws_scale(img_convert_ctx,pFrame->data,pFrame->linesize,0,
			pFrame->height,pOutputFrame->data,pOutputFrame->linesize);

		SDL_UnlockYUVOverlay(vp->bmp);
		vp->pts = pts;
		/* now we inform our display thread that we have a pic ready */
		if(++m_streamstate->pictq_windex == VIDEO_PICTURE_QUEUE_SIZE) 
		{
			m_streamstate->pictq_windex = 0;
		}
		SDL_LockMutex(m_streamstate->pictq_mutex);
		m_streamstate->pictq_size++;
		SDL_UnlockMutex(m_streamstate->pictq_mutex);
	}
	return 0;
}

int CFFMPEG_SDL_MFC_PLAYER_1Dlg::video_open(StreamState *m_streamstate)
{
	int flags = SDL_HWSURFACE | SDL_ASYNCBLIT | SDL_HWACCEL;
	int w,h;
	VideoPicture *vp = &m_streamstate->pictq[m_streamstate->pictq_rindex];

	////这里是获取到 STATIC控件的大小 
	////这里做判断是因为 如果改变大小时 的 宽 和高 出现 奇数则图像会不正确 或 中断 要偶数
	if (m_is_full_screen)
	{
		flags |= SDL_FULLSCREEN;
	}
	else         
	{
		flags |= SDL_RESIZABLE;
	}

	if (m_is_full_screen && m_fs_screen_width) 
	{
		w = m_fs_screen_width;
		h = m_fs_screen_height;
	} 
	else
	{
		//要改变的大小
		w = m_screen_width;
		h = m_screen_height;
	}

	if (m_screen && m_screen_width == m_screen->w && m_screen->w == w
		&& m_screen_height== m_screen->h && m_screen->h == h)
	{
		return 0;
	}

	m_screen = SDL_SetVideoMode(w, h, 0, flags);

	//SDL刷新 否则最大化然后缩小有问题 
	SDL_Flip(m_screen);
	//这里一定要重新刷新MFC窗口
	Invalidate(TRUE);
	if (!m_screen)
	{
		fprintf(stderr, "SDL: could not set video mode - exiting\n");
		OnBnClickedButtonStop();
	}
	return 0;
}


void CFFMPEG_SDL_MFC_PLAYER_1Dlg::alloc_picture(void *userdata)
{
	StreamState *m_streamstate = (StreamState *)userdata;
	VideoPicture *vp;

	vp = &m_streamstate->pictq[m_streamstate->pictq_windex];
	if(vp->bmp) 
	{
		// we already have one make another, bigger/smaller
		SDL_FreeYUVOverlay(vp->bmp);
	}

	video_open(m_streamstate); 
	// Allocate a place to put our YUV image on that screen
	vp->bmp = SDL_CreateYUVOverlay(m_screen_width,
		m_screen_height,
		SDL_YV12_OVERLAY,
		m_screen);
	vp->width = m_screen_width;
	vp->height = m_screen_height;

	SDL_LockMutex(m_streamstate->pictq_mutex);
	vp->allocated = 1;
	SDL_CondSignal(m_streamstate->pictq_cond);
	SDL_UnlockMutex(m_streamstate->pictq_mutex);
}


int CFFMPEG_SDL_MFC_PLAYER_1Dlg::stream_component_open(StreamState *m_streamstate, int stream_index)
{
	AVFormatContext *ic = m_streamstate->pFormatCtx;
	SDL_AudioSpec wanted_spec, spec;

	if (stream_index < 0 || stream_index >= ic->nb_streams)
	{
		return -1;
	}

	ic->streams[stream_index]->discard = AVDISCARD_DEFAULT;

	/* prepare audio output */
	if (stream_index == m_audio_stream_idx) 
	{
		if (!m_paudio_codec || avcodec_open2(m_audio_dec_ctx, m_paudio_codec,NULL) < 0)
		{
			return -1;
		}

		// Set audio settings from codec info
		wanted_spec.freq = m_dwFrequency;

		switch (m_dwBitsPerSample)
		{
		case 8:
			wanted_spec.format = AUDIO_S8;
			break;
		case 16:
			wanted_spec.format = AUDIO_S16SYS;
			break;
		default:
			wanted_spec.format = AUDIO_S16SYS;
			break;
		}
		wanted_spec.channels = m_dwChannelCount;
		wanted_spec.silence = 0;//silence值，由于为signed，故为0 信号
		wanted_spec.samples = SDL_AUDIO_BUFFER_SIZE;
		wanted_spec.callback = audio_callback;
		wanted_spec.userdata = this;
		wanted_spec.size = 0;  //SDL_OpenAudio().调用时计算得到

		//打开音频设备
		if(SDL_OpenAudio(&wanted_spec, &spec) < 0)
		{
			fprintf(stderr, "SDL_OpenAudio: %s\n", SDL_GetError());
			return -1;
		}

		if (spec.format != AUDIO_S16SYS) 
		{
			fprintf(stderr, "SDL advised audio format %d m_streamstate not supported!\n", spec.format);
			return -1;
		}
		m_streamstate->audio_st = ic->streams[stream_index];
		m_streamstate->audio_buf_size  = 0;
		m_streamstate->audio_buf_index = 0;
		memset(&m_streamstate->audio_pkt, 0, sizeof(m_streamstate->audio_pkt));
		packet_queue_init(&m_streamstate->audioq);
		packet_queue_start(&m_streamstate->audioq);
		SDL_PauseAudio(0);
	}
	else if (stream_index == m_video_stream_idx)
	{
		if (!m_pvideo_codec || avcodec_open2(m_video_dec_ctx, m_pvideo_codec,NULL) < 0)
		{
			return -1;
		}
		m_streamstate->video_st = ic->streams[stream_index];
		packet_queue_init(&m_streamstate->videoq);
		packet_queue_start(&m_streamstate->videoq);
		m_streamstate->video_tid = SDL_CreateThread(video_thread, this);
	}
	return 1;
}

int CFFMPEG_SDL_MFC_PLAYER_1Dlg::stream_component_close(StreamState *m_streamstate, int stream_index)
{
	AVFormatContext *ic = m_streamstate->pFormatCtx;

    if (stream_index < 0 || stream_index >= ic->nb_streams)
	{
        return - 1;
	}

	ic->streams[stream_index]->discard = AVDISCARD_ALL;

	if (stream_index == m_audio_stream_idx) 
	{
		packet_queue_abort(&m_streamstate->audioq);
		SDL_CloseAudio();
		packet_queue_flush(&m_streamstate->audioq);
		av_free_packet(&m_streamstate->audio_pkt);
		m_streamstate->audio_st = NULL;
		avcodec_close(m_audio_dec_ctx);
	}
	else if (stream_index == m_video_stream_idx)
	{
		packet_queue_abort(&m_streamstate->videoq);
        /* note: we also signal this mutex to make sure we deblock the
           video thread in all cases */
        SDL_LockMutex(m_streamstate->pictq_mutex);
        SDL_CondSignal(m_streamstate->pictq_cond);
        SDL_UnlockMutex(m_streamstate->pictq_mutex);
		//SDL_WaitThread(m_streamstate->video_tid,NULL);
		SDL_KillThread(m_streamstate->video_tid);
        packet_queue_flush(&m_streamstate->videoq);
		m_streamstate->video_st = NULL;
		avcodec_close(m_video_dec_ctx);
	}
	return 1;
}

void CFFMPEG_SDL_MFC_PLAYER_1Dlg::packet_queue_init(PacketQueue *q) 
{
	memset(q, 0, sizeof(PacketQueue));
	q->mutex = SDL_CreateMutex();
	q->cond = SDL_CreateCond();
}

int CFFMPEG_SDL_MFC_PLAYER_1Dlg::packet_queue_put(PacketQueue *q, AVPacket *pkt) 
{
	int ret;

	/* duplicate the packet */
	if (pkt != &m_flush_pkt && av_dup_packet(pkt) < 0)
	{
		return -1;
	}

	SDL_LockMutex(q->mutex);
	ret = packet_queue_put_private(q, pkt);
	SDL_UnlockMutex(q->mutex);

	if (pkt != &m_flush_pkt && ret < 0)
	{
		av_free_packet(pkt);
	}

	return ret;
}

void CFFMPEG_SDL_MFC_PLAYER_1Dlg::packet_queue_start(PacketQueue *q)
{
	SDL_LockMutex(q->mutex);
	packet_queue_put_private(q, &m_flush_pkt);
	SDL_UnlockMutex(q->mutex);
}

int CFFMPEG_SDL_MFC_PLAYER_1Dlg::packet_queue_get(PacketQueue *q, AVPacket *pkt, int queue_type)
{
	AVPacketList *pkt1;
	int ret;

	SDL_LockMutex(q->mutex);

	for(;;)
	{
		pkt1 = q->first_pkt;
		if (pkt1) 
		{
			q->first_pkt = pkt1->next;
			if (!q->first_pkt)
			{
				q->last_pkt = NULL;
			}
			q->nb_packets--;
			//q->size -= pkt1->pkt.size;
			q->size -= pkt1->pkt.size;
			*pkt = pkt1->pkt;
			av_free(pkt1);
			ret = 1;
			break;
		}
		else 
		{
			int sdlcondwait = SDL_CondWait(q->cond, q->mutex);
		}
	}
	SDL_UnlockMutex(q->mutex);
	return ret;
}

void CFFMPEG_SDL_MFC_PLAYER_1Dlg::packet_queue_abort(PacketQueue *q)
{
	SDL_LockMutex(q->mutex);
	SDL_CondSignal(q->cond);
	SDL_UnlockMutex(q->mutex);
}


void CFFMPEG_SDL_MFC_PLAYER_1Dlg::packet_queue_flush(PacketQueue *q) 
{
	AVPacketList *pkt, *pkt1;

	SDL_LockMutex(q->mutex);
	for(pkt = q->first_pkt; pkt != NULL; pkt = pkt1) 
	{
		pkt1 = pkt->next;
		av_free_packet(&pkt->pkt);
		av_freep(&pkt);
	}
	q->last_pkt = NULL;
	q->first_pkt = NULL;
	q->nb_packets = 0;
	q->size = 0;
	SDL_UnlockMutex(q->mutex);
}

void CFFMPEG_SDL_MFC_PLAYER_1Dlg::packet_queue_destroy(PacketQueue *q)
{
	packet_queue_flush(q);
	SDL_DestroyMutex(q->mutex);
	SDL_DestroyCond(q->cond);
}
int CFFMPEG_SDL_MFC_PLAYER_1Dlg::packet_queue_put_private(PacketQueue *q, AVPacket *pkt)
{
	AVPacketList *pkt1;

	pkt1 = (AVPacketList * )av_malloc(sizeof(AVPacketList));
	if (!pkt1)
	{
		return -1;
	}
	pkt1->pkt = *pkt;
	pkt1->next = NULL;

	if (!q->last_pkt)
	{
		q->first_pkt = pkt1;
	}
	else
	{
		q->last_pkt->next = pkt1;
	}
	q->last_pkt = pkt1;
	q->nb_packets++;
	//q->size += pkt1->pkt.size + sizeof(*pkt1);
	q->size += pkt1->pkt.size;
	/* XXX: should duplicate packet data in DV case */
	SDL_CondSignal(q->cond);
	return 0;
}

int read_thread( LPVOID lpParam )
{
	CFFMPEG_SDL_MFC_PLAYER_1Dlg * pDlg = (CFFMPEG_SDL_MFC_PLAYER_1Dlg *)lpParam;
	ASSERT(pDlg);

	int ret = -1;
	int error = -1;
	int eof = 0;
	AVPacket pkt1, *pkt = &pkt1;

	//创建刷新线程
	pDlg->m_streamstate->refresh_tid = SDL_CreateThread(refresh_thread, pDlg);

	/* open the streams */
	if (AVMEDIA_TYPE_AUDIO >= 0)
	{
		ret = pDlg->stream_component_open(pDlg->m_streamstate, AVMEDIA_TYPE_AUDIO);
	}
	if (AVMEDIA_TYPE_VIDEO >= 0)
	{
		ret = pDlg->stream_component_open(pDlg->m_streamstate, AVMEDIA_TYPE_VIDEO);
	}

	if (pDlg->m_video_stream_idx < 0 && pDlg->m_audio_stream_idx < 0) 
	{
		ret = -1;
		goto fail;
	}

	for(;;)
	{
		if (pDlg->m_Isstop)
		{
			break;
		}
		// seek stuff goes here
		if(pDlg->m_streamstate->seek_req) 
		{
			int stream_index = -1;
			//转化成纳秒
			int64_t seek_target = pDlg->m_streamstate->seek_pos * 1000000;

			//如果不是只有音频
			if (pDlg->m_stream_type != 2)
			{
				if(pDlg->m_video_stream_idx >= 0)
				{
					stream_index = pDlg->m_video_stream_idx;
				}
				else if(pDlg->m_audio_stream_idx >= 0) 
				{
					stream_index = pDlg->m_audio_stream_idx;
				}
			}
			else
			{
				if(pDlg->m_audio_stream_idx >= 0) 
				{
					stream_index = pDlg->m_audio_stream_idx;
				}
				else if(pDlg->m_video_stream_idx >= 0)
				{
					stream_index = pDlg->m_video_stream_idx;
				}
			}
			if(stream_index >= 0)
			{
				AVRational time_base_q ;
				time_base_q.num = 1;
				time_base_q.den = AV_TIME_BASE;
				//这里一定要注意：不单纯的是从秒转成毫秒，//seek_target = seek_target / 1000; 这样做是不对的
				seek_target = av_rescale_q(seek_target, time_base_q, pDlg->m_streamstate->pFormatCtx->streams[stream_index]->time_base);
			}
			error = av_seek_frame(pDlg->m_streamstate->pFormatCtx, stream_index, seek_target, pDlg->m_streamstate->seek_flags);
			//seek成功
			if( error >= 0)
			{
				if(pDlg->m_audio_stream_idx >= 0) 
				{
					pDlg->packet_queue_flush(&pDlg->m_streamstate->audioq);
					pDlg->packet_queue_put(&pDlg->m_streamstate->audioq, &pDlg->m_flush_pkt);
				}
				if(pDlg->m_video_stream_idx >= 0)
				{
					pDlg->packet_queue_flush(&pDlg->m_streamstate->videoq);
					pDlg->packet_queue_put(&pDlg->m_streamstate->videoq, &pDlg->m_flush_pkt);
				}
				//这里要重新这是video_clock 和 audio_clock
				//如果音视频都有
				if (pDlg->m_stream_type == 1)
				{
					pDlg->m_streamstate->video_clock = ((int)(pDlg->m_streamstate->seek_pos / pDlg->m_video_duration) + 1) * pDlg->m_video_duration ;
					if (strstr(pDlg->m_streamstate->pFormatCtx->iformat->name,"mpegts")!= NULL)
					{
						double time_base = 90 * 1000;
						pDlg->m_streamstate->audio_clock = (pDlg->m_streamstate->seek_pos / (1024.0 * time_base/ pDlg->m_streamstate->audio_st->codec->sample_rate /100000) + 1) * 
							(1024.0 * time_base/ pDlg->m_streamstate->audio_st->codec->sample_rate /100000);
					}
					else
					{
						pDlg->m_streamstate->audio_clock = pDlg->m_streamstate->seek_pos ;
						pDlg->m_streamstate->audio_clock = ((pDlg->m_streamstate->seek_pos / (1024.0 / pDlg->m_streamstate->audio_st->codec->sample_rate)) + 1) * 
							(1024.0 / pDlg->m_streamstate->audio_st->codec->sample_rate);
					}
				}
				//只有音频
				else if (pDlg->m_stream_type == 2)
				{
					if (strstr(pDlg->m_streamstate->pFormatCtx->iformat->name,"mpegts")!= NULL)
					{
						double time_base = 90 * 1000;
						pDlg->m_streamstate->audio_clock = (pDlg->m_streamstate->seek_pos / (1024.0 * time_base/ pDlg->m_streamstate->audio_st->codec->sample_rate /100000) + 1) * 
							(1024.0 * time_base/ pDlg->m_streamstate->audio_st->codec->sample_rate /100000);
					}
					else
					{
						pDlg->m_streamstate->audio_clock = pDlg->m_streamstate->seek_pos ;
						pDlg->m_streamstate->audio_clock = ((pDlg->m_streamstate->seek_pos / (1024.0 / pDlg->m_streamstate->audio_st->codec->sample_rate)) + 1) * 
							(1024.0 / pDlg->m_streamstate->audio_st->codec->sample_rate);
					}
				}
				//只有视频
				else if (pDlg->m_stream_type == 3)
				{
					pDlg->m_streamstate->video_clock = ((int)(pDlg->m_streamstate->seek_pos / pDlg->m_video_duration) + 1) * pDlg->m_video_duration ;
				}
			} 
			pDlg->m_streamstate->seek_req = 0;
			pDlg->m_streamstate->seek_time = 0;
			eof = 0;
		}

		/* if the queue are full, no need to read more */
		if (   pDlg->m_streamstate->audioq.size + pDlg->m_streamstate->videoq.size > MAX_QUEUE_SIZE
			|| (   (pDlg->m_streamstate->audioq.nb_packets > MIN_FRAMES )
			&& (pDlg->m_streamstate->videoq.nb_packets > MIN_FRAMES)))
		{
			/* wait 10 ms */
			SDL_Delay(10);
			continue;
		}

		if (eof)
		{
			//到文件末尾 放入队列一个空包
			if (pDlg->m_video_stream_idx >= 0) 
			{
				av_init_packet(pkt);
				pkt->data = NULL;
				pkt->size = 0;
				pkt->stream_index = pDlg->m_video_stream_idx;
				pDlg->packet_queue_put(&pDlg->m_streamstate->videoq, pkt);
			}
			if (pDlg->m_audio_stream_idx >= 0 ) 
			{
				if (pDlg->m_streamstate->audioq.nb_packets == 0)
				{
					pDlg->m_IsEnd_audio = 1;
				}
				av_init_packet(pkt);
				pkt->data = NULL;
				pkt->size = 0;
				pkt->stream_index = pDlg->m_audio_stream_idx;
				pDlg->packet_queue_put(&pDlg->m_streamstate->audioq, pkt);
			}
			SDL_Delay(10);
			if (pDlg->m_streamstate->audioq.size + pDlg->m_streamstate->videoq.size == 0) 
			{
				ret = AVERROR_EOF;
				goto fail;
			}
			eof = 0;
			continue;
		}
		//这里确定文件到了末尾
		ret = av_read_frame(pDlg->m_streamstate->pFormatCtx, pkt);
		if (ret < 0) 
		{
			if (ret == AVERROR_EOF || url_feof(pDlg->m_streamstate->pFormatCtx->pb))
			{
				eof = 1;
			}
			if (pDlg->m_streamstate->pFormatCtx->pb && pDlg->m_streamstate->pFormatCtx->pb->error)
			{
				break;
			}
			SDL_Delay(100); /* wait for user event */
			continue;
		}
		// Is this a packet from the video stream?
		if(pkt->stream_index == pDlg->m_video_stream_idx)
		{
			pDlg->packet_queue_put(&pDlg->m_streamstate->videoq, pkt);
		} 
		else if(pkt->stream_index == pDlg->m_audio_stream_idx) 
		{
			pDlg->packet_queue_put(&pDlg->m_streamstate->audioq, pkt);
		} 
		else 
		{
			av_free_packet(pkt);
		}
	}
	ret = 0;

fail:
	/* close each stream */
	VideoPicture * vp;
	int i;
	pDlg->m_Isstop = 1;

	if (pDlg->m_audio_stream_idx >= 0)
	{
		pDlg->stream_component_close(pDlg->m_streamstate, pDlg->m_audio_stream_idx);
	}
	if (pDlg->m_video_stream_idx >= 0)
	{
		pDlg->stream_component_close(pDlg->m_streamstate,pDlg->m_video_stream_idx);
	}
	if (pDlg->m_streamstate->pFormatCtx) 
	{
		avformat_close_input(&pDlg->m_streamstate->pFormatCtx);
	}
	if (ret == AVERROR_EOF) 
	{
		//退出操作
//////////////////////////////////////////////////////////////////////////
		if (pDlg->m_streamstate->refresh_tid)
		{
			//SDL_WaitThread(pDlg->m_streamstate->refresh_tid,NULL);
			SDL_KillThread(pDlg->m_streamstate->refresh_tid);
		}
		if (pDlg->m_streamstate->videoq.mutex)
		{
			pDlg->packet_queue_destroy(&pDlg->m_streamstate->videoq);
		}
		if (pDlg->m_streamstate->audioq.mutex)
		{
			pDlg->packet_queue_destroy(&pDlg->m_streamstate->audioq);
		}
		/* free all pictures */
		for (i = 0; i < VIDEO_PICTURE_QUEUE_SIZE; i++) 
		{
			vp = &pDlg->m_streamstate->pictq[i];
			if (vp->bmp) 
			{
				SDL_FreeYUVOverlay(vp->bmp);
				vp->bmp = NULL;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		pDlg->m_streamstate->pFormatCtx = NULL;
		pDlg->m_streamstate->audio_st = NULL;
		pDlg->m_streamstate->video_st = NULL;
		pDlg->m_streamstate->audioq.first_pkt = NULL;
		pDlg->m_streamstate->audioq.last_pkt = NULL;
		pDlg->m_streamstate->audioq.nb_packets = 0;
		pDlg->m_streamstate->audioq.size = 0;
		pDlg->m_streamstate->audioq.mutex = NULL;
		pDlg->m_streamstate->audioq.cond = NULL;
		pDlg->m_streamstate->videoq.first_pkt = NULL;
		pDlg->m_streamstate->videoq.last_pkt = NULL;
		pDlg->m_streamstate->videoq.nb_packets = 0;
		pDlg->m_streamstate->videoq.size = 0;
		pDlg->m_streamstate->videoq.mutex = NULL;
		pDlg->m_streamstate->videoq.cond = NULL;
		memset(pDlg->m_streamstate->audio_buf,0,(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2);
		pDlg->m_streamstate->audio_buf_size = 0;
		pDlg->m_streamstate->audio_buf_index = 0;
		//刷新packet
		if (pDlg->m_flush_pkt.data) 
		{
			av_free_packet(&pDlg->m_flush_pkt);
		}
		pDlg->m_streamstate->audio_pkt_data = NULL;
		pDlg->m_streamstate->audio_pkt_size = 0;
		pDlg->m_streamstate->pictq[0].bmp = NULL;
		pDlg->m_streamstate->pictq[0].width = 0;
		pDlg->m_streamstate->pictq[0].height = 0;
		pDlg->m_streamstate->pictq[0].allocated = 0;
		pDlg->m_streamstate->pictq[0].pts = 0.0;
		pDlg->m_streamstate->pictq_size = 0;
		pDlg->m_streamstate->pictq_rindex = 0;
		pDlg->m_streamstate->pictq_windex = 0;
		//释放互斥变量 条件变量
		if (pDlg->m_streamstate->pictq_mutex)
		{
			SDL_DestroyMutex(pDlg->m_streamstate->pictq_mutex);
		}
		if (pDlg->m_streamstate->pictq_cond)
		{
			SDL_DestroyCond(pDlg->m_streamstate->pictq_cond);
		}
		pDlg->m_streamstate->pictq_mutex = NULL;
		pDlg->m_streamstate->pictq_cond = NULL;
		pDlg->m_streamstate->read_tid = NULL; 
		pDlg->m_streamstate->video_tid = NULL;  
		pDlg->m_streamstate->refresh_tid = NULL;
		pDlg->m_streamstate->audio_clock = 0.0;
		pDlg->m_streamstate->video_clock = 0.0;
		pDlg->m_streamstate->seek_time = 0.0;
		pDlg->m_streamstate->seek_req = 0;
		pDlg->m_streamstate->seek_flags = 0;
		pDlg->m_streamstate->seek_pos = 0;

		//////////////////////////////////////////////////////////////////////////
		//pDlg->m_fs_screen_width;
		//pDlg->m_fs_screen_height; 
		//AVPacket m_flush_pkt;
		pDlg->m_sourceFile.Empty();
		pDlg->m_file_duration = 0.0;
		pDlg->m_video_dec_ctx = NULL; 
		pDlg->m_audio_dec_ctx = NULL ; 
		pDlg->m_pvideo_codec = NULL; 
		pDlg->m_paudio_codec = NULL;
		//释放窗口 
		if (pDlg->m_screen)
		{ 
			SDL_FreeSurface( pDlg->m_screen);
			pDlg->m_screen = NULL;
		}
		pDlg->m_is_full_screen = 0;
		pDlg->m_screen_width  = 0;
		pDlg->m_screen_height = 0;
		pDlg->m_Isstop = 0;
		pDlg->m_pause_play = 0;                          
		pDlg->m_slider_pos = 0;  
		pDlg->m_stream_type = 0;

		//////////////////////////////////////////////////////////////////////////
		//video pram
		pDlg->m_video_stream_idx = 0;              
		pDlg->m_dbFrameRate = 0.0;               
		pDlg->m_video_duration = 0.0;              
		pDlg->m_dwWidth = 0;                          
		pDlg->m_dwHeight = 0;                     
		pDlg->m_video_codecID = AV_CODEC_ID_NONE;
		pDlg->m_video_pixelfromat = AV_PIX_FMT_NONE;  
		memset(pDlg->m_spspps,0,100);
		pDlg->m_spspps_size = 0;

		//////////////////////////////////////////////////////////////////////////
		//audio pram
		pDlg->m_audio_stream_idx = 0; 
		pDlg->m_audio_duration = 0.0;              
		pDlg->m_dwChannelCount = 0;              
		pDlg->m_dwBitsPerSample = 0;             
		pDlg->m_dwFrequency = 0;                 
		pDlg->m_audio_codecID = AV_CODEC_ID_NONE;               
		pDlg->m_audio_frame_size = 0;                  
		pDlg->m_IsEnd_audio = 0;   
		//////////////////////////////////////////////////////////////////////////
		pDlg->m_Picture_rect.left = 0;
		pDlg->m_Picture_rect.right = 0;
		pDlg->m_Picture_rect.top = 0;
		pDlg->m_Picture_rect.bottom = 0;
		//////////////////////////////////////////////////////////////////////////


		//清空列表框中所有的数据
		pDlg->m_listControl.DeleteAllItems();

		//将滚动条设置到最前端
		pDlg->m_sliderPlay.SetPos(0);

		//初始化CEDIT 时间控件
		pDlg->GetDlgItem(IDC_EDIT_CURRENT_TIME)->SetWindowText("  --:--  ");
		pDlg->GetDlgItem(IDC_EDIT_ALL_TIME)->SetWindowText("  --:--  ");

		//设置播放暂停按钮 停止按钮 不可操作
		pDlg->GetDlgItem(IDC_BUTTON_PLAY_PAUSE)->EnableWindow(FALSE);
		pDlg->GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		pDlg->GetDlgItem(IDC_SLIDER_VOLUME)->EnableWindow(FALSE);
		pDlg->GetDlgItem(IDC_CHECK_MUTE)->EnableWindow(FALSE);

		pDlg->Invalidate(TRUE);
	}
	return 1;
}

void CFFMPEG_SDL_MFC_PLAYER_1Dlg::OnBnClickedButtonOpen()
{
	// TODO: Add your control notification handler code here

	//////////////////////////////////////////////////////////////////////////
	//设置播放进度条 的最大最小值
	m_sliderPlay.SetPos(0);
	m_sliderPlay.SetRange(0,m_file_duration,0);
	//////////////////////////////////////////////////////////////////////////
	//设置音量 的最大最小值
	m_sliderVolume.SetPos(64);
	m_Volume_pos = m_sliderVolume.GetPos();
	m_sliderVolume.SetRange(0,128,0);
	//////////////////////////////////////////////////////////////////////////

	m_streamstate->read_tid  = SDL_CreateThread(read_thread,(void*)this);
	if (!m_streamstate->read_tid)
	{
		MessageBox(_T("创建读取线程失败 清楚重新创建"), NULL, MB_OK );
		goto end;
	}
end:
	{
		//////////////////////////////////////////////////////////////////////////
		//这里可以做一些其他工作

		//////////////////////////////////////////////////////////////////////////
	}
	//设置OPEN控件不可操作
	GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(FALSE);
	//设置控件可操作
	GetDlgItem(IDC_BUTTON_PLAY_PAUSE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
	GetDlgItem(IDC_SLIDER_VOLUME)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK_MUTE)->EnableWindow(TRUE);
	m_sliderPlay.EnableWindow(TRUE);
}

void CFFMPEG_SDL_MFC_PLAYER_1Dlg::OnBnClickedButtonPlayPause()
{
	// TODO: Add your control notification handler code here
	//这里做判断是因为当暂停 时 声音会一直播放暂停前的一帧音频数据
	if (m_pause_play)
	{
		//暂停
		SDL_PauseAudio(1);
		GetDlgItem(IDC_BUTTON_PLAY_PAUSE)->SetWindowText("播放") ;
	}
	else
	{
		//播放
		SDL_PauseAudio(0);
		GetDlgItem(IDC_BUTTON_PLAY_PAUSE)->SetWindowText("暂停") ;
	}
	m_pause_play = !m_pause_play;
}

void CFFMPEG_SDL_MFC_PLAYER_1Dlg::OnBnClickedButtonStop()
{
	// TODO: Add your control notification handler code here 
	//?? 有时2次以上打开文件会卡住或中断 （优化后已经很少）

	m_Isstop = 1;
	Sleep(500);

	//销毁变量等
	UinitVariable();

	//清空列表框中所有的数据
	m_listControl.DeleteAllItems();

	//将滚动条设置到最前端
	m_sliderPlay.SetPos(0);

	//初始化CEDIT 时间控件
	GetDlgItem(IDC_EDIT_CURRENT_TIME)->SetWindowText("  --:--  ");
	GetDlgItem(IDC_EDIT_ALL_TIME)->SetWindowText("  --:--  ");

	//设置播放暂停按钮 停止按钮 不可操作
	GetDlgItem(IDC_BUTTON_PLAY_PAUSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_SLIDER_VOLUME)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_MUTE)->EnableWindow(FALSE);

	Invalidate(TRUE);
}

void CFFMPEG_SDL_MFC_PLAYER_1Dlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnBnClickedButtonStop();
	UinitProgram();
	OnCancel();
}

int CFFMPEG_SDL_MFC_PLAYER_1Dlg::Access_control_position()
{
	//加载按钮初始化的位置
	GetDlgItem(IDC_BUTTON_LOAD)->GetWindowRect(&m_rect_BUTTON_LOAD);//获取控件相对于屏幕的位置
	ScreenToClient(m_rect_BUTTON_LOAD);//转化为对话框上的相对位置
	m_width_BUTTON_LOAD = m_rect_BUTTON_LOAD.right - m_rect_BUTTON_LOAD.left;  //width为button的宽
	m_height__BUTTON_LOAD = m_rect_BUTTON_LOAD.bottom - m_rect_BUTTON_LOAD.top; //height为button的高

	//打开按钮初始化的位置
	GetDlgItem(IDC_BUTTON_OPEN)->GetWindowRect(&m_rect_BUTTON_OPEN);//获取控件相对于屏幕的位置
	ScreenToClient(m_rect_BUTTON_OPEN);//转化为对话框上的相对位置
	m_width_BUTTON_OPEN = m_rect_BUTTON_OPEN.right - m_rect_BUTTON_OPEN.left;  //width为button的宽
	m_height__BUTTON_OPEN = m_rect_BUTTON_OPEN.bottom - m_rect_BUTTON_OPEN.top; //height为button的高

	//播放暂停按钮初始化的位置
	GetDlgItem(IDC_BUTTON_PLAY_PAUSE)->GetWindowRect(&m_rect_BUTTON_PLAY_PAUSE);//获取控件相对于屏幕的位置
	ScreenToClient(m_rect_BUTTON_PLAY_PAUSE);//转化为对话框上的相对位置
	m_width_BUTTON_PLAY_PAUSE = m_rect_BUTTON_PLAY_PAUSE.right - m_rect_BUTTON_PLAY_PAUSE.left;  //width为button的宽
	m_height__BUTTON_PLAY_PAUSE = m_rect_BUTTON_PLAY_PAUSE.bottom - m_rect_BUTTON_PLAY_PAUSE.top; //height为button的高

	//停止按钮初始化的位置
	GetDlgItem(IDC_BUTTON_STOP)->GetWindowRect(&m_rect_BUTTON_STOP);//获取控件相对于屏幕的位置
	ScreenToClient(m_rect_BUTTON_STOP);//转化为对话框上的相对位置
	m_width_BUTTON_STOP = m_rect_BUTTON_STOP.right - m_rect_BUTTON_STOP.left;  //width为button的宽
	m_height__BUTTON_STOP = m_rect_BUTTON_STOP.bottom - m_rect_BUTTON_STOP.top; //height为button的高


	//退出按钮初始化的位置
	GetDlgItem(IDCANCEL)->GetWindowRect(&m_rect_BUTTON_CANCEL);//获取控件相对于屏幕的位置
	ScreenToClient(m_rect_BUTTON_CANCEL);//转化为对话框上的相对位置
	m_width_BUTTON_CANCEL = m_rect_BUTTON_CANCEL.right - m_rect_BUTTON_CANCEL.left;  //width为button的宽
	m_height__BUTTON_CANCEL = m_rect_BUTTON_CANCEL.bottom - m_rect_BUTTON_CANCEL.top; //height为button的高

	//静音复选框初始化的位置
	GetDlgItem(IDC_CHECK_MUTE)->GetWindowRect(&m_rect_CHECK_MUTE);//获取控件相对于屏幕的位置
	ScreenToClient(m_rect_CHECK_MUTE);//转化为对话框上的相对位置
	m_width_CHECK_MUTE = m_rect_CHECK_MUTE.right - m_rect_CHECK_MUTE.left;  //width为button的宽
	m_height_CHECK_MUTE = m_rect_CHECK_MUTE.bottom - m_rect_CHECK_MUTE.top; //height为button的高

	//音量控制初始化的位置
	GetDlgItem(IDC_SLIDER_VOLUME)->GetWindowRect(&m_rect_SLIDER_VOLUME);//获取控件相对于屏幕的位置
	ScreenToClient(m_rect_SLIDER_VOLUME);//转化为对话框上的相对位置
	m_width_SLIDER_VOLUME = m_rect_SLIDER_VOLUME.right - m_rect_SLIDER_VOLUME.left;  //width为button的宽
	m_height_SLIDER_VOLUME = m_rect_SLIDER_VOLUME.bottom - m_rect_SLIDER_VOLUME.top; //height为button的高

	//显示视频信息的列表框控初始化的位置
	GetDlgItem(IDC_LIST_CONTROL_INFO)->GetWindowRect(&m_rect_LIST_CONTROL);//获取控件相对于屏幕的位置
	ScreenToClient(m_rect_LIST_CONTROL);//转化为对话框上的相对位置
	m_width_LIST_CONTROL = m_rect_LIST_CONTROL.right - m_rect_LIST_CONTROL.left;  //width为button的宽
	m_height_LIST_CONTROL = m_rect_LIST_CONTROL.bottom - m_rect_LIST_CONTROL.top; //height为button的高

	//进度条初始化的位置
	GetDlgItem(IDC_SLIDER_PLAY)->GetWindowRect(&m_rect_SLIDER_PLAYL);//获取控件相对于屏幕的位置
	ScreenToClient(m_rect_SLIDER_PLAYL);//转化为对话框上的相对位置
	m_width_SLIDER_PLAYL = m_rect_SLIDER_PLAYL.right - m_rect_SLIDER_PLAYL.left;  //width为button的宽
	m_height_SLIDER_PLAY = m_rect_SLIDER_PLAYL.bottom - m_rect_SLIDER_PLAYL.top; //height为button的高

	//当前播放时间初始化的位置
	GetDlgItem(IDC_EDIT_CURRENT_TIME)->GetWindowRect(&m_rect_CURRENT_TIME);//获取控件相对于屏幕的位置
	ScreenToClient(m_rect_CURRENT_TIME);//转化为对话框上的相对位置
	m_width_CURRENT_TIME = m_rect_CURRENT_TIME.right - m_rect_CURRENT_TIME.left;  //width为button的宽
	m_height_CURRENT_TIME = m_rect_CURRENT_TIME.bottom - m_rect_CURRENT_TIME.top; //height为button的高

	//文件总时长初始化的位置
	GetDlgItem(IDC_EDIT_ALL_TIME)->GetWindowRect(&m_rect_ALL_TIME);//获取控件相对于屏幕的位置
	ScreenToClient(m_rect_ALL_TIME);//转化为对话框上的相对位置
	m_width_ALL_TIME = m_rect_ALL_TIME.right - m_rect_ALL_TIME.left;  //width为button的宽
	m_height_ALL_TIME = m_rect_ALL_TIME.bottom - m_rect_ALL_TIME.top; //height为button的高

	//播放区域初始化的位置
	GetDlgItem(IDC_PICURE_CONTROL)->GetWindowRect(&m_rect_PICURE_CONTROL);//获取控件相对于屏幕的位置
	ScreenToClient(m_rect_PICURE_CONTROL);//转化为对话框上的相对位置
	m_width_PICURE_CONTROL = m_rect_PICURE_CONTROL.right - m_rect_PICURE_CONTROL.left;  //width为button的宽
	m_height_PICURE_CONTROL = m_rect_PICURE_CONTROL.bottom - m_rect_PICURE_CONTROL.top; //height为button的高

	return 1;
}

int CFFMPEG_SDL_MFC_PLAYER_1Dlg::InitProgram()
{
	int flags;
	/* register all codecs, demux and protocols */
	av_register_all();
	avcodec_register_all();
	avformat_network_init();

	//将CSTATIC控件和sdl显示窗口关联 
	HWND hWnd = this->GetDlgItem(IDC_PICURE_CONTROL)->GetSafeHwnd();
	if( hWnd !=NULL)
	{
		char sdl_var[64];    
		sprintf(sdl_var, "SDL_WINDOWID=%d", hWnd);    //主窗口句柄      //这里一定不能有空格SDL_WINDOWID=%d"
		SDL_putenv(sdl_var);   
		char *myvalue = SDL_getenv("SDL_WINDOWID");   //让SDL取得窗口ID  
	}

	//SDL初始化
	flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER;
	int  sdlinit = SDL_Init (flags);
	if (sdlinit)
	{
		char * sss = SDL_GetError();
		fprintf(stderr, "Could not initialize SDL - %s\n", SDL_GetError());
		fprintf(stderr, "(Did you set the DISPLAY variable?)\n");
		return -1;
	} 

	//设置SDL事件状态
	SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
	SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
	SDL_EventState(SDL_USEREVENT, SDL_IGNORE);

//////////////////////////////////////////////////////////////////////////
	//音视频全局结构体初始化 这里已经清零
	m_streamstate = NULL;
	m_streamstate = (StreamState *)calloc(1,sizeof(StreamState));
	if (!m_streamstate)
	{
		return -1;
	}
	m_streamstate->pFormatCtx = NULL;
	m_streamstate->audio_st = NULL;
	m_streamstate->video_st = NULL;
	m_streamstate->audioq.first_pkt = NULL;
	m_streamstate->audioq.last_pkt = NULL;
	m_streamstate->audioq.nb_packets = 0;
	m_streamstate->audioq.size = 0;
	m_streamstate->audioq.mutex = NULL;
	m_streamstate->audioq.cond = NULL;
	m_streamstate->videoq.first_pkt = NULL;
	m_streamstate->videoq.last_pkt = NULL;
	m_streamstate->videoq.nb_packets = 0;
	m_streamstate->videoq.size = 0;
	m_streamstate->videoq.mutex = NULL;
	m_streamstate->videoq.cond = NULL;
	memset(m_streamstate->audio_buf,0,(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2);
	m_streamstate->audio_buf_size = 0;
	m_streamstate->audio_buf_index = 0;
	//AVPacket       audio_pkt;
	m_streamstate->audio_pkt_data = NULL;
	m_streamstate->audio_pkt_size = 0;
	m_streamstate->pictq[0].bmp = NULL;
	m_streamstate->pictq[0].width = 0;
	m_streamstate->pictq[0].height = 0;
	m_streamstate->pictq[0].allocated = 0;
	m_streamstate->pictq[0].pts = 0.0;
	m_streamstate->pictq_size = 0;
	m_streamstate->pictq_rindex = 0;
	m_streamstate->pictq_windex = 0;
	m_streamstate->pictq_mutex = NULL;
	m_streamstate->pictq_cond = NULL;
	m_streamstate->read_tid = NULL; 
	m_streamstate->video_tid = NULL;  
	m_streamstate->refresh_tid = NULL;
	m_streamstate->audio_clock = 0.0;
	m_streamstate->video_clock = 0.0;
	m_streamstate->seek_time = 0.0;
	m_streamstate->seek_req = 0;
	m_streamstate->seek_flags = 0;
	m_streamstate->seek_pos = 0;

	//////////////////////////////////////////////////////////////////////////
	//获取全屏的屏幕宽高  这里只能调用一次 第二次调用时 大小将改变
	const SDL_VideoInfo *vi = SDL_GetVideoInfo();
	m_fs_screen_width = vi->current_w;
	m_fs_screen_height = vi->current_h; 
	//AVPacket m_flush_pkt;
	m_sourceFile.Empty();
	m_file_duration = 0.0;
	m_video_dec_ctx = NULL; 
	m_audio_dec_ctx = NULL ; 
	m_pvideo_codec = NULL; 
	m_paudio_codec = NULL;
	m_screen = NULL;
	m_is_full_screen = 0;
	m_screen_width  = 0;
	m_screen_height = 0;
	m_Isstop = 0;
	m_pause_play = 0;                          
	m_slider_pos = 0;    
	m_stream_type = 0;

	//////////////////////////////////////////////////////////////////////////
	//video pram
	m_video_stream_idx = 0;              
	m_dbFrameRate = 0.0;               
	m_video_duration = 0.0;              
	m_dwWidth = 0;                          
	m_dwHeight = 0;                     
	m_video_codecID = AV_CODEC_ID_NONE;
	m_video_pixelfromat = AV_PIX_FMT_NONE;  
	memset(m_spspps,0,100);
	m_spspps_size = 0;

	//////////////////////////////////////////////////////////////////////////
	//audio pram
	m_audio_stream_idx = 0; 
	m_audio_duration = 0.0;              
	m_dwChannelCount = 0;              
	m_dwBitsPerSample = 0;             
	m_dwFrequency = 0;                 
	m_audio_codecID = AV_CODEC_ID_NONE;               
	m_audio_frame_size = 0;                  
	m_IsEnd_audio = 0;                         
	m_Volume_pos = 0;       
	//////////////////////////////////////////////////////////////////////////
	m_Picture_rect.left = 0;
	m_Picture_rect.right = 0;
	m_Picture_rect.top = 0;
	m_Picture_rect.bottom = 0;
//////////////////////////////////////////////////////////////////////////
	return 1;
}

int CFFMPEG_SDL_MFC_PLAYER_1Dlg::UinitProgram()
{
//////////////////////////////////////////////////////////////////////////
	//释放全局结构体
	m_streamstate->pFormatCtx = NULL;
	m_streamstate->audio_st = NULL;
	m_streamstate->video_st = NULL;
	m_streamstate->audioq.first_pkt = NULL;
	m_streamstate->audioq.last_pkt = NULL;
	m_streamstate->audioq.nb_packets = 0;
	m_streamstate->audioq.size = 0;
	m_streamstate->audioq.mutex = NULL;
	m_streamstate->audioq.cond = NULL;
	m_streamstate->videoq.first_pkt = NULL;
	m_streamstate->videoq.last_pkt = NULL;
	m_streamstate->videoq.nb_packets = 0;
	m_streamstate->videoq.size = 0;
	m_streamstate->videoq.mutex = NULL;
	m_streamstate->videoq.cond = NULL;
	memset(m_streamstate->audio_buf,0,(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2);
	m_streamstate->audio_buf_size = 0;
	m_streamstate->audio_buf_index = 0;
	//AVPacket       audio_pkt;
	m_streamstate->audio_pkt_data = NULL;
	m_streamstate->audio_pkt_size = 0;
	m_streamstate->pictq[0].bmp = NULL;
	m_streamstate->pictq[0].width = 0;
	m_streamstate->pictq[0].height = 0;
	m_streamstate->pictq[0].allocated = 0;
	m_streamstate->pictq[0].pts = 0.0;
	m_streamstate->pictq_size = 0;
	m_streamstate->pictq_rindex = 0;
	m_streamstate->pictq_windex = 0;
	m_streamstate->pictq_mutex = NULL;
	m_streamstate->pictq_cond = NULL;
	m_streamstate->read_tid = NULL; 
	m_streamstate->video_tid = NULL;  
	m_streamstate->refresh_tid = NULL;
	m_streamstate->audio_clock = 0.0;
	m_streamstate->video_clock = 0.0;
	m_streamstate->seek_time = 0.0;
	m_streamstate->seek_req = 0;
	m_streamstate->seek_flags = 0;
	m_streamstate->seek_pos = 0;
	free(m_streamstate);
	m_streamstate = NULL;

	//////////////////////////////////////////////////////////////////////////
	m_fs_screen_width = 0;
	m_fs_screen_height = 0; 
	//AVPacket m_flush_pkt;
	m_sourceFile.Empty();
	m_file_duration = 0.0;
	m_video_dec_ctx = NULL; 
	m_audio_dec_ctx = NULL ; 
	m_pvideo_codec = NULL; 
	m_paudio_codec = NULL;
	m_screen = NULL;
	m_is_full_screen = 0;
	m_screen_width  = 0;
	m_screen_height = 0;
	m_Isstop = 0;
	m_pause_play = 0;                          
	m_slider_pos = 0;  
	m_stream_type = 0;

	//////////////////////////////////////////////////////////////////////////
	//video pram
	m_video_stream_idx = 0;              
	m_dbFrameRate = 0.0;               
	m_video_duration = 0.0;              
	m_dwWidth = 0;                          
	m_dwHeight = 0;                     
	m_video_codecID = AV_CODEC_ID_NONE;
	m_video_pixelfromat = AV_PIX_FMT_NONE;  
	memset(m_spspps,0,100);
	m_spspps_size = 0;

	//////////////////////////////////////////////////////////////////////////
	//audio pram
	m_audio_stream_idx = 0; 
	m_audio_duration = 0.0;              
	m_dwChannelCount = 0;              
	m_dwBitsPerSample = 0;             
	m_dwFrequency = 0;                 
	m_audio_codecID = AV_CODEC_ID_NONE;               
	m_audio_frame_size = 0;                  
	m_IsEnd_audio = 0;                         
	m_Volume_pos = 0;                          
	//////////////////////////////////////////////////////////////////////////
	m_Picture_rect.left = 0;
	m_Picture_rect.right = 0;
	m_Picture_rect.top = 0;
	m_Picture_rect.bottom = 0;
//////////////////////////////////////////////////////////////////////////
	avformat_network_deinit();
	SDL_Quit();
	return 1;
}

int CFFMPEG_SDL_MFC_PLAYER_1Dlg::InitVariable()
{
//////////////////////////////////////////////////////////////////////////
	//音视频全局结构体初始化
	m_streamstate->pFormatCtx = NULL;
	m_streamstate->audio_st = NULL;
	m_streamstate->video_st = NULL;
	m_streamstate->audioq.first_pkt = NULL;
	m_streamstate->audioq.last_pkt = NULL;
	m_streamstate->audioq.nb_packets = 0;
	m_streamstate->audioq.size = 0;
	m_streamstate->audioq.mutex = NULL;
	m_streamstate->audioq.cond = NULL;
	m_streamstate->videoq.first_pkt = NULL;
	m_streamstate->videoq.last_pkt = NULL;
	m_streamstate->videoq.nb_packets = 0;
	m_streamstate->videoq.size = 0;
	m_streamstate->videoq.mutex = NULL;
	m_streamstate->videoq.cond = NULL;
	memset(m_streamstate->audio_buf,0,(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2);
	m_streamstate->audio_buf_size = 0;
	m_streamstate->audio_buf_index = 0;
	//AVPacket       audio_pkt;
	m_streamstate->audio_pkt_data = NULL;
	m_streamstate->audio_pkt_size = 0;
	m_streamstate->pictq[0].bmp = NULL;
	m_streamstate->pictq[0].width = 0;
	m_streamstate->pictq[0].height = 0;
	m_streamstate->pictq[0].allocated = 0;
	m_streamstate->pictq[0].pts = 0.0;
	m_streamstate->pictq_size = 0;
	m_streamstate->pictq_rindex = 0;
	m_streamstate->pictq_windex = 0;
	m_streamstate->pictq_mutex = NULL;
	m_streamstate->pictq_cond = NULL;
	//创建全局条件变量互斥变量
	m_streamstate->pictq_mutex = SDL_CreateMutex();
	m_streamstate->pictq_cond  = SDL_CreateCond();
	m_streamstate->read_tid = NULL; 
	m_streamstate->video_tid = NULL;  
	m_streamstate->refresh_tid = NULL;
	m_streamstate->audio_clock = 0.0;
	m_streamstate->video_clock = 0.0;
	m_streamstate->seek_time = 0.0;
	m_streamstate->seek_req = 0;
	m_streamstate->seek_flags = 0;
	m_streamstate->seek_pos = 0;

	//////////////////////////////////////////////////////////////////////////
	//m_fs_screen_width
	//m_fs_screen_height
	//刷新packet初始化
	av_init_packet(&m_flush_pkt);
	m_flush_pkt.data = (uint8_t *)(intptr_t)"FLUSH";
	m_sourceFile.Empty();
	m_file_duration = 0.0;
	m_video_dec_ctx = NULL; 
	m_audio_dec_ctx = NULL ; 
	m_pvideo_codec = NULL; 
	m_paudio_codec = NULL;
	m_screen = NULL;
	m_is_full_screen = 0;
	//初始视频大小
	m_picture_static.GetWindowRect(&m_Picture_rect); 
	m_screen_width = m_Picture_rect.right - m_Picture_rect.left;
	m_screen_height = m_Picture_rect.bottom - m_Picture_rect.top;
	//这里是获取到 STATIC控件的大小 
	//这里做判断是因为 如果改变大小时 的 宽 和高 出现 奇数则图像会不正确 或 中断 要偶数
	if (m_screen_width % 2 != 0)
	{
		m_screen_width -= 1;
	}
	if (m_screen_height % 2 != 0)
	{
		m_screen_height -= 1;
	}
	m_Isstop = 0;
	//这里设置文件刚打开时候的状态0 暂停 1播放 
	m_pause_play = 1;                           
	m_slider_pos = 0; 
	m_stream_type = 0;

	//////////////////////////////////////////////////////////////////////////
	//video pram
	m_video_stream_idx = -1;
	m_dbFrameRate = 25.0;
	m_video_duration = 0.040; 
	//初始化的时候 设置为static控件的宽,高
	m_dwWidth = m_Picture_rect.right - m_Picture_rect.left;  
	m_dwHeight = m_Picture_rect.bottom - m_Picture_rect.top;      
	m_video_codecID = AV_CODEC_ID_H264;
	m_video_pixelfromat = AV_PIX_FMT_YUV420P;  
	memset(m_spspps,0,100);                    
	m_spspps_size = 0;

    //////////////////////////////////////////////////////////////////////////
	//audio pram
	m_audio_stream_idx = -1;
	m_audio_duration = 0; 
	m_dwChannelCount = 2;                  
	m_dwBitsPerSample = 16;                 
	m_dwFrequency = 44100;                       
	m_audio_codecID = AV_CODEC_ID_AAC;              
	m_audio_frame_size = 1024;   
	m_IsEnd_audio = 0;
	//设置文件打开时的音量大小
	m_Volume_pos = 64;             
	//////////////////////////////////////////////////////////////////////////
	m_Picture_rect.left = 0;
	m_Picture_rect.right = 0;
	m_Picture_rect.top = 0;
	m_Picture_rect.bottom = 0;
//////////////////////////////////////////////////////////////////////////
	return 1;
}

int CFFMPEG_SDL_MFC_PLAYER_1Dlg::UinitVariable()
{
	VideoPicture * vp;
	int i;
	//释放packet_queue
	if (m_streamstate->read_tid)
	{
		SDL_WaitThread(m_streamstate->read_tid,NULL);
		//SDL_KillThread(m_streamstate->read_tid);
	}
	if (m_streamstate->refresh_tid)
	{
		//SDL_WaitThread(m_streamstate->refresh_tid,NULL);
		SDL_KillThread(m_streamstate->refresh_tid);

	}
	if (m_streamstate->videoq.mutex)
	{
		packet_queue_destroy(&m_streamstate->videoq);
	}
	if (m_streamstate->audioq.mutex)
	{
		packet_queue_destroy(&m_streamstate->audioq);
	}
	/* free all pictures */
	for (i = 0; i < VIDEO_PICTURE_QUEUE_SIZE; i++) 
	{
		vp = &m_streamstate->pictq[i];
		if (vp->bmp) 
		{
			SDL_FreeYUVOverlay(vp->bmp);
			vp->bmp = NULL;
		}
	}
//////////////////////////////////////////////////////////////////////////
	m_streamstate->pFormatCtx = NULL;
	m_streamstate->audio_st = NULL;
	m_streamstate->video_st = NULL;
	m_streamstate->audioq.first_pkt = NULL;
	m_streamstate->audioq.last_pkt = NULL;
	m_streamstate->audioq.nb_packets = 0;
	m_streamstate->audioq.size = 0;
	m_streamstate->audioq.mutex = NULL;
	m_streamstate->audioq.cond = NULL;
	m_streamstate->videoq.first_pkt = NULL;
	m_streamstate->videoq.last_pkt = NULL;
	m_streamstate->videoq.nb_packets = 0;
	m_streamstate->videoq.size = 0;
	m_streamstate->videoq.mutex = NULL;
	m_streamstate->videoq.cond = NULL;
	memset(m_streamstate->audio_buf,0,(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2);
	m_streamstate->audio_buf_size = 0;
	m_streamstate->audio_buf_index = 0;
	//刷新packet
	if (m_flush_pkt.data) 
	{
		av_free_packet(&m_flush_pkt);
	}
	m_streamstate->audio_pkt_data = NULL;
	m_streamstate->audio_pkt_size = 0;
	m_streamstate->pictq[0].bmp = NULL;
	m_streamstate->pictq[0].width = 0;
	m_streamstate->pictq[0].height = 0;
	m_streamstate->pictq[0].allocated = 0;
	m_streamstate->pictq[0].pts = 0.0;
	m_streamstate->pictq_size = 0;
	m_streamstate->pictq_rindex = 0;
	m_streamstate->pictq_windex = 0;
	//释放互斥变量 条件变量
	if (m_streamstate->pictq_mutex)
	{
		SDL_DestroyMutex(m_streamstate->pictq_mutex);
	}
	if (m_streamstate->pictq_cond)
	{
		SDL_DestroyCond(m_streamstate->pictq_cond);
	}
	m_streamstate->pictq_mutex = NULL;
	m_streamstate->pictq_cond = NULL;
	m_streamstate->read_tid = NULL; 
	m_streamstate->video_tid = NULL;  
	m_streamstate->refresh_tid = NULL;
	m_streamstate->audio_clock = 0.0;
	m_streamstate->video_clock = 0.0;
	m_streamstate->seek_time = 0.0;
	m_streamstate->seek_req = 0;
	m_streamstate->seek_flags = 0;
	m_streamstate->seek_pos = 0;

	//////////////////////////////////////////////////////////////////////////
	//m_fs_screen_width;
	//m_fs_screen_height; 
	//AVPacket m_flush_pkt;
	m_sourceFile.Empty();
	m_file_duration = 0.0;
	m_video_dec_ctx = NULL; 
	m_audio_dec_ctx = NULL ; 
	m_pvideo_codec = NULL; 
	m_paudio_codec = NULL;
	//释放窗口 
	if (m_screen)
	{ 
		SDL_FreeSurface( m_screen);
		m_screen = NULL;
	}
	m_is_full_screen = 0;
	m_screen_width  = 0;
	m_screen_height = 0;
	m_Isstop = 1;  //这里必须为1 要不然别的线程可能还没退出 就析构了变量
	m_pause_play = 0;                          
	m_slider_pos = 0;  
	m_stream_type = 0;

	//////////////////////////////////////////////////////////////////////////
	//video pram
	m_video_stream_idx = 0;              
	m_dbFrameRate = 0.0;               
	m_video_duration = 0.0;              
	m_dwWidth = 0;                          
	m_dwHeight = 0;                     
	m_video_codecID = AV_CODEC_ID_NONE;
	m_video_pixelfromat = AV_PIX_FMT_NONE;  
	memset(m_spspps,0,1000);
	m_spspps_size = 0;

	//////////////////////////////////////////////////////////////////////////
	//audio pram
	m_audio_stream_idx = 0; 
	m_audio_duration = 0.0;              
	m_dwChannelCount = 0;              
	m_dwBitsPerSample = 0;             
	m_dwFrequency = 0;                 
	m_audio_codecID = AV_CODEC_ID_NONE;               
	m_audio_frame_size = 0;                  
	m_IsEnd_audio = 0;   
	//////////////////////////////////////////////////////////////////////////
	m_Picture_rect.left = 0;
	m_Picture_rect.right = 0;
	m_Picture_rect.top = 0;
	m_Picture_rect.bottom = 0;
//////////////////////////////////////////////////////////////////////////
	return 1;
}

int CFFMPEG_SDL_MFC_PLAYER_1Dlg::Open_codec_context(int * stream_idx, AVFormatContext * fmt_ctx, enum AVMediaType type)
{
	int ret;
	ret = av_find_best_stream(fmt_ctx, type, -1, -1, NULL, 0);
	if (ret < 0)
	{
		return ret;
	} 
	else 
	{
		*stream_idx = ret;
	}
	return 0;
}

void CFFMPEG_SDL_MFC_PLAYER_1Dlg::OnSize(UINT nType, int cx, int cy)
{
	// TODO: Add your message handler code here

	//?? 暂停时改变窗口大小，图像不会跟着改变
	if (m_IsAccess_control_position == 1)
	{
		//加载按钮位置调整
		GetDlgItem(IDC_BUTTON_LOAD)->MoveWindow(m_rect_BUTTON_LOAD.left,cy - m_height__BUTTON_LOAD,
			m_width_BUTTON_LOAD,m_height__BUTTON_LOAD);

		//打开按钮位置调整
		GetDlgItem(IDC_BUTTON_OPEN)->MoveWindow(m_rect_BUTTON_OPEN.left,cy - m_height__BUTTON_OPEN,
			m_width_BUTTON_OPEN,m_height__BUTTON_OPEN);

		//播放暂停按钮位置调整
		GetDlgItem(IDC_BUTTON_PLAY_PAUSE)->MoveWindow(m_rect_BUTTON_PLAY_PAUSE.left,cy - m_height__BUTTON_PLAY_PAUSE,
			m_width_BUTTON_PLAY_PAUSE,m_height__BUTTON_PLAY_PAUSE);

		//停止按钮位置调整
		GetDlgItem(IDC_BUTTON_STOP)->MoveWindow(m_rect_BUTTON_STOP.left,cy - m_height__BUTTON_STOP,
			m_width_BUTTON_STOP,m_height__BUTTON_STOP);

		//退出按钮位置调整
		GetDlgItem(IDCANCEL)->MoveWindow(m_rect_BUTTON_CANCEL.left,cy - m_height__BUTTON_CANCEL,
			m_width_BUTTON_CANCEL,m_height__BUTTON_CANCEL);

		//静音复选框位置调整
		GetDlgItem(IDC_CHECK_MUTE)->MoveWindow(cx - m_width_CHECK_MUTE - (m_width_MAINFRAME - m_rect_CHECK_MUTE.right) ,cy - m_height_CHECK_MUTE,
			m_width_CHECK_MUTE,m_height_CHECK_MUTE);

		//音量控制位置调整
		GetDlgItem(IDC_SLIDER_VOLUME)->MoveWindow(cx - m_width_SLIDER_VOLUME - (m_width_MAINFRAME - m_rect_SLIDER_VOLUME.right) ,cy - m_height_SLIDER_VOLUME,
			m_width_SLIDER_VOLUME,m_height_SLIDER_VOLUME);

		//显示视频信息的列表框位置调整 
		GetDlgItem(IDC_LIST_CONTROL_INFO)->MoveWindow(m_rect_LIST_CONTROL.left ,cy - m_height_LIST_CONTROL - (m_height_BMAINFRAME - m_rect_LIST_CONTROL.bottom),
			cx,m_height_LIST_CONTROL);

		//进度条位置调整  
		GetDlgItem(IDC_SLIDER_PLAY)->MoveWindow(m_rect_SLIDER_PLAYL.left ,cy - m_height_SLIDER_PLAY - (m_height_BMAINFRAME - m_rect_SLIDER_PLAYL.bottom),
			cx - 2* (m_width_MAINFRAME - m_rect_SLIDER_PLAYL.right) ,m_height_SLIDER_PLAY);

		//当前播放时间位置调整
		GetDlgItem(IDC_EDIT_CURRENT_TIME)->MoveWindow(m_rect_CURRENT_TIME.left ,cy - m_height_CURRENT_TIME - (m_height_BMAINFRAME - m_rect_CURRENT_TIME.bottom),
			m_width_CURRENT_TIME,m_height_CURRENT_TIME);

		//文件总时长位置调整
		GetDlgItem(IDC_EDIT_ALL_TIME)->MoveWindow(cx - m_width_ALL_TIME - (m_width_MAINFRAME - m_rect_ALL_TIME.right) ,cy - m_height_ALL_TIME - (m_height_BMAINFRAME - m_rect_ALL_TIME.bottom),
			m_width_ALL_TIME,m_height_ALL_TIME);

		if (m_is_full_screen)  //如果全屏需要重新调整
		{
			//播放区域位置调整
			GetDlgItem(IDC_PICURE_CONTROL)->MoveWindow(m_rect_PICURE_CONTROL.left ,m_rect_PICURE_CONTROL.top,
				cx ,cy);
		}
		else
		{
			//播放区域位置调整
			GetDlgItem(IDC_PICURE_CONTROL)->MoveWindow(m_rect_PICURE_CONTROL.left ,m_rect_PICURE_CONTROL.top,
				cx - (m_width_MAINFRAME - m_rect_PICURE_CONTROL.right),cy - (m_height_BMAINFRAME - m_rect_PICURE_CONTROL.bottom));
		}
		//获取显示图像的区域 
		GetDlgItem(IDC_PICURE_CONTROL)->GetWindowRect(&m_Picture_rect);
		//将窗口区域大小传给要显示的sdl
		m_screen_width  = m_Picture_rect.right - m_Picture_rect.left;
		m_screen_height = m_Picture_rect.bottom - m_Picture_rect.top;

		//这里设置最小化 托盘
		if (m_screen_width == 0 || m_screen_height == 0)
		{
			m_screen_width = m_width_PICURE_CONTROL;    //设置为初始化的 宽
			m_screen_height = m_height_PICURE_CONTROL;  //设置为初始化的 高
		}

		//这里是获取到 STATIC控件的大小 
		//这里做判断是因为 如果改变大小时 的 宽 和高 出现 奇数则图像会不正确 或 中断 要偶数
		if (m_screen_width % 2 != 0)
		{
			m_screen_width -= 1;
		}
		if (m_screen_height % 2 != 0)
		{
			m_screen_height -= 1;
		}
		//刷新窗口
		Invalidate(TRUE);
	}
	else
	{
		m_width_MAINFRAME = cx;
		m_height_BMAINFRAME = cy;
	}
	CDialog::OnSize(nType, cx, cy);
}

void CFFMPEG_SDL_MFC_PLAYER_1Dlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default

	lpMMI->ptMinTrackSize.x = MFCMINWINDOWSIZE_WIDTH; 
	lpMMI->ptMinTrackSize.y = MFCMINWINDOWSIZE_HEIGHT; 

	CDialog::OnGetMinMaxInfo(lpMMI);
}

void CFFMPEG_SDL_MFC_PLAYER_1Dlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	if (pScrollBar->GetSafeHwnd() == m_sliderPlay.GetSafeHwnd())        //如果是 文件播放进度条
	{
		//?? seek后的文件结束时间可以优化用video->clock代表文件当前播放的时间 和文件结束时间不匹配
		//首先暂停
		//////////////////////////////////////////////////////////////////////////
		if (m_pause_play) //如果正在播放
		{
			//暂停
			SDL_PauseAudio(1);
			m_pause_play = !m_pause_play;
		}
		//////////////////////////////////////////////////////////////////////////
		if (nSBCode == SB_ENDSCROLL) //结束滚动  
		{
			m_streamstate->seek_time = m_slider_pos - (int)(m_streamstate->audio_clock);  //获取改变的时常 可能是整数也可能是负数
			m_streamstate->seek_req = 1;
			m_streamstate->seek_pos = m_slider_pos;
			m_streamstate->seek_flags = AVSEEK_FLAG_FRAME;//m_streamstate->seek_time < 0 ? AVSEEK_FLAG_BACKWARD : 0;
			m_slider_pos = 0;
			//////////////////////////////////////////////////////////////////////////
			//恢复播放
			if (!m_pause_play) //如果正在暂停
			{
				//播放
				SDL_PauseAudio(0);
				m_pause_play = !m_pause_play;
			}
			//////////////////////////////////////////////////////////////////////////
		}
		else
		{
			m_slider_pos = m_sliderPlay.GetPos();
		}
	}
	else if (pScrollBar->GetSafeHwnd() == m_sliderVolume.GetSafeHwnd()) //如果是音量 进度条
	{
	    m_Volume_pos = m_sliderVolume.GetPos();
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CFFMPEG_SDL_MFC_PLAYER_1Dlg::OnBnClickedCheckMute()
{
	// TODO: Add your control notification handler code here
	//设置静音
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_MUTE);
	if (pBtn->GetCheck() == true)
	{
		// 勾选
		m_sliderVolume.SetPos(0);
		m_Volume_pos = m_sliderVolume.GetPos();
	}
	else
	{
		//取消勾选
		m_sliderVolume.SetPos(64);
		m_Volume_pos = m_sliderVolume.GetPos();

	}
}

void CFFMPEG_SDL_MFC_PLAYER_1Dlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	LONG style = ::GetWindowLong(this->m_hWnd,GWL_STYLE);
	if(m_is_full_screen == 0)//全屏显示
	{
		m_is_full_screen = 1;
		style &= ~(WS_DLGFRAME | WS_THICKFRAME);
		style &= WS_CHILDWINDOW;
		SetWindowLong(this->m_hWnd,GWL_STYLE, style);
		this->ShowWindow(SW_SHOWMAXIMIZED);
		CRect rect;
		this->GetWindowRect(&rect);
		::SetWindowPos(this->m_hWnd,HWND_NOTOPMOST,rect.left, rect.top, rect.right- rect.left, rect.bottom-rect.top, SWP_FRAMECHANGED);
		//控件不可见
		//进度条不可见  
		GetDlgItem(IDC_SLIDER_PLAY)->ShowWindow(SW_HIDE); 
		//当前播放时间不可见
		GetDlgItem(IDC_EDIT_CURRENT_TIME)->ShowWindow(SW_HIDE);
		//文件总时长不可见
		GetDlgItem(IDC_EDIT_ALL_TIME)->ShowWindow(SW_HIDE);
		//显示视频信息的列表框不可见 
		GetDlgItem(IDC_LIST_CONTROL_INFO)->ShowWindow(SW_HIDE); 
		//加载按钮不可见
		GetDlgItem(IDC_BUTTON_LOAD)->ShowWindow(SW_HIDE); 
		//打开按钮不可见
		GetDlgItem(IDC_BUTTON_OPEN)->ShowWindow(SW_HIDE);
		//播放暂停按钮不可见
		GetDlgItem(IDC_BUTTON_PLAY_PAUSE)->ShowWindow(SW_HIDE);
		//停止按钮不可见
		GetDlgItem(IDC_BUTTON_STOP)->ShowWindow(SW_HIDE);
		//退出按钮不可见
		GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
		//静音复选框不可见
		GetDlgItem(IDC_CHECK_MUTE)->ShowWindow(SW_HIDE);
		//音量控制不可见
		GetDlgItem(IDC_SLIDER_VOLUME)->ShowWindow(SW_HIDE);
	}
	else
	{
		m_is_full_screen = 0;
		style |=  WS_OVERLAPPEDWINDOW;
		SetWindowLong(this->m_hWnd, GWL_STYLE, style);
		this->ShowWindow(SW_NORMAL);
		//控件恢复
		//进度条恢复  
		GetDlgItem(IDC_SLIDER_PLAY)->ShowWindow(SW_NORMAL); 
		//当前播放时间恢复
		GetDlgItem(IDC_EDIT_CURRENT_TIME)->ShowWindow(SW_NORMAL);
		//文件总时长恢复
		GetDlgItem(IDC_EDIT_ALL_TIME)->ShowWindow(SW_NORMAL);
		//显示视频信息的列表框恢复
		GetDlgItem(IDC_LIST_CONTROL_INFO)->ShowWindow(SW_NORMAL); 
		//加载按钮恢复
		GetDlgItem(IDC_BUTTON_LOAD)->ShowWindow(SW_NORMAL); 
		//打开按钮恢复
		GetDlgItem(IDC_BUTTON_OPEN)->ShowWindow(SW_NORMAL);
		//播放暂停按钮恢复
		GetDlgItem(IDC_BUTTON_PLAY_PAUSE)->ShowWindow(SW_NORMAL);
		//停止按钮恢复
		GetDlgItem(IDC_BUTTON_STOP)->ShowWindow(SW_NORMAL);
		//退出按钮恢复
		GetDlgItem(IDCANCEL)->ShowWindow(SW_NORMAL);
		//静音复选框恢复
		GetDlgItem(IDC_CHECK_MUTE)->ShowWindow(SW_NORMAL);
		//音量控制恢复
		GetDlgItem(IDC_SLIDER_VOLUME)->ShowWindow(SW_NORMAL);
	}
	CDialog::OnLButtonDblClk(nFlags, point);
}
