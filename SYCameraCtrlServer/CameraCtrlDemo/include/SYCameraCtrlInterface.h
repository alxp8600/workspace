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
** �ص���ʽ������:
** ��һ��Ϊע��RecvCameraChannelFunc������ʽ,ͨ��AcquireCameraLinkStatus����������ѯ����״̬
** �ڶ���Ϊͨ��SetClassCallbackע��ICameraCtrlServerCallback�ӿ�,ͨ��OnCameraLinkStatusNotify����������ȡ����״̬
**/


class SYCAMERACTRLSERVER_EXPORTS CameraCtrlServer
{
public:
	CameraCtrlServer(const char* ip, unsigned int port, RecvCameraChannelFunc pCallbackFunc = NULL); //���RecvCameraChannelFuncΪNUll����ʹ��SetClassCallback��ʽ��
																									 
	~CameraCtrlServer();

public:
	void SetClassCallback(ICameraCtrlServerCallback* pClass);
	bool Start(); 
	bool Stop();
	bool SetAutoMode(bool bAuto = true); //trueΪ�Զ��� falseΪ�ֶ�
	void AcquireCameraLinkStatus(emCameraCtrlType type, emCameraLinkStatus& status);

protected:
	CameraCtrlServerImpl* m_pImpl;
};

