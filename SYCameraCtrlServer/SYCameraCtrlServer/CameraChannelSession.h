#pragma once

struct SwitchCmd
{
	ULONGLONG  cmdframe; //Convert form FILETIME
	int 	  nChannel;
	int		  nFxNum;
	char	  UsrData[250];
};


class CameraChannelSession
{
public:
	CameraChannelSession(RecvCameraChannelFunc pCallbackFunc);
	~CameraChannelSession(void);

	void SetClassCallback(ICameraCtrlServerCallback* pClass);
	bool Start(); 
	bool Stop();
	void AcquireCameraLinkStatus(emCameraLinkStatus& status);

private:
	static DWORD WINAPI OnChannelThread(LPVOID pParam);
	DWORD OnChannelProc();

private:
	RecvCameraChannelFunc m_pCallbackFunc;
	ICameraCtrlServerCallback* m_pClassCallback;

	HANDLE m_hChannelThread;
	bool m_bChannelThreadStop;
	HANDLE m_hChannelThreadClose;

	SOCKET  m_ListenSocket;
	SOCKET	m_AcceptSocket;

	WSAEVENT m_wsaEvent;
	emCameraLinkStatus m_emLinkStatus;
};

