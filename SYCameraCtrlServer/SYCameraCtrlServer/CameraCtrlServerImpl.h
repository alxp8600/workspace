#pragma once
#include "../../include/SYCameraCtrlInterface.h"
#include "CameraChannelSession.h"
#include "CameraAutoModeSession.h"

class CameraCtrlServerImpl
{
public:
	CameraCtrlServerImpl(const char* ip, unsigned short port, RecvCameraChannelFunc pCallbackFunc);
	~CameraCtrlServerImpl(void);

	void SetClassCallback(ICameraCtrlServerCallback* pClass);
	bool Start(); 
	bool Stop();
	bool SetAutoMode(bool bAuto = true);
	void AcquireCameraLinkStatus(emCameraCtrlType type, emCameraLinkStatus& status);

private:
	CameraChannelSession m_channelSession;
	CameraAutoModeSession m_autoSession;
};

