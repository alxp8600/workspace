#pragma once
class CameraAutoModeSession
{
public:
	CameraAutoModeSession(const char* ip, unsigned short port, RecvCameraChannelFunc pCallbackFunc);
	~CameraAutoModeSession(void);

	void SetClassCallback(ICameraCtrlServerCallback* pClass);
	bool Start(); 
	bool Stop();
	bool SetAutoMode(bool bAuto = true);
	void AcquireCameraLinkStatus(emCameraLinkStatus& status);

private:
	static DWORD WINAPI OnAutoThread(LPVOID pParam);
	DWORD OnAutoProc();

private:
	RecvCameraChannelFunc m_pCallbackFunc;
	ICameraCtrlServerCallback* m_pClassCallback;

	std::string m_strIp;
	unsigned short m_nPort;

	HANDLE m_hAutoModeThread;
	bool m_bAutoModeThreadStop;
	HANDLE m_hAutoModeThreadClose;
	emCameraLinkStatus m_emLinkStatus;
	SOCKET m_ConnectSocket;
	WSAEVENT m_wsaEvent;
};

