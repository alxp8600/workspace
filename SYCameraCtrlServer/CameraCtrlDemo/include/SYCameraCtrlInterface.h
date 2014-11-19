#pragma once

#ifdef SYCAMERACTRLSERVER_EXPORTS
#define SYCAMERACTRLSERVER_EXPORTS __declspec(dllexport)
#else
#define SYCAMERACTRLSERVER_EXPORTS __declspec(dllimport)
#endif

#define CHANNEL_LOCCAL_PORT_DEF 29595

enum emCameraCtrlType
{
	emCameraCtrl_AutoMode,
	emCameraCtrl_Channel
};

enum emCameraLinkStatus
{
	emCameraLinkStatus_Unlink,
	emCameraLinkStatus_LinkWaiting,
	emCameraLinkStatus_Linked
};

class SYCAMERACTRLSERVER_EXPORTS ICameraCtrlServerCallback
{
public:
	virtual void OnRecvCameraChannelCallback(unsigned short nChannel) = 0;
	virtual void OnCameraLinkStatusNotify(emCameraCtrlType type, emCameraLinkStatus status) = 0;
};

typedef void (__stdcall *RecvCameraChannelFunc)(unsigned short nChannel);

class CameraCtrlServerImpl;

/**
** 回调方式有两种:
** 第一种为注册RecvCameraChannelFunc函数方式,通过AcquireCameraLinkStatus（主动）查询链接状态
** 第二种为通过SetClassCallback注册ICameraCtrlServerCallback接口,通过OnCameraLinkStatusNotify（被动）获取链接状态
**/


class SYCAMERACTRLSERVER_EXPORTS CameraCtrlServer
{
public:
	CameraCtrlServer(const char* ip, unsigned int port, RecvCameraChannelFunc pCallbackFunc = NULL); //如果RecvCameraChannelFunc为NUll，请使用SetClassCallback方式。
																									 
	~CameraCtrlServer();

public:
	void SetClassCallback(ICameraCtrlServerCallback* pClass);
	bool Start(); 
	bool Stop();
	bool SetAutoMode(bool bAuto = true); //true为自动， false为手动
	void AcquireCameraLinkStatus(emCameraCtrlType type, emCameraLinkStatus& status);

protected:
	CameraCtrlServerImpl* m_pImpl;
};

