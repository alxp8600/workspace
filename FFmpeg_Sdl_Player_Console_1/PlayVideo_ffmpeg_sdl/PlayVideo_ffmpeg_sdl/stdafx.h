// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"
//#include <Windows.h>
#include <stdio.h>
#include <tchar.h>

#include <afxwin.h>
#include <string>
#include <iostream>
#include <deque>

extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavfilter/avfilter.h"
#include "libavutil/avutil.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
#include "libavutil/fifo.h"
#include "SDL/SDL.h"
};


#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avfilter.lib")
#pragma comment(lib, "swresample.lib")
#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")



// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
