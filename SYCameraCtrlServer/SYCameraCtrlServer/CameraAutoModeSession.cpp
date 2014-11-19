#include "StdAfx.h"
#include "CameraCtrlServerImpl.h"
#include "CameraAutoModeSession.h"


CameraAutoModeSession::CameraAutoModeSession(const char* ip, unsigned short port, RecvCameraChannelFunc pCallbackFunc) : m_strIp(ip)
																														, m_nPort(port)
																														, m_pCallbackFunc(pCallbackFunc)
																														, m_pClassCallback(NULL)
																														, m_hAutoModeThread(NULL)
																														, m_bAutoModeThreadStop(true)
																														, m_hAutoModeThreadClose(NULL)
																														, m_ConnectSocket(NULL)
																														, m_wsaEvent(NULL)
																														, m_emLinkStatus(emCameraLinkStatus_Unlink)
{
}


CameraAutoModeSession::~CameraAutoModeSession(void)
{
}

void CameraAutoModeSession::SetClassCallback( ICameraCtrlServerCallback* pClass )
{
	m_pClassCallback = pClass;
}

bool CameraAutoModeSession::Start()
{
	bool bret;
	m_bAutoModeThreadStop = false;
	if(!m_hAutoModeThreadClose)
		m_hAutoModeThreadClose = CreateEvent(NULL, FALSE, FALSE, NULL);
	if(!m_hAutoModeThread)
		m_hAutoModeThread = CreateThread(NULL, 0, OnAutoThread, this, 0, NULL);
	bret = m_hAutoModeThread != NULL ? true : false;
	return bret;
}

bool CameraAutoModeSession::Stop()
{
	bool bret;
	if(m_hAutoModeThread)
	{
		m_bAutoModeThreadStop = true;
		SetEvent(m_hAutoModeThreadClose);
		DWORD dwRet = WaitForSingleObject(m_hAutoModeThread, 3000);
		if(dwRet == WAIT_TIMEOUT)
			bret = false;
		CloseHandle(m_hAutoModeThread);
		m_hAutoModeThread = NULL;
	}
	if(m_hAutoModeThreadClose)
		CloseHandle(m_hAutoModeThreadClose);
	return bret;
}

bool CameraAutoModeSession::SetAutoMode( bool bAuto /*= true*/ )
{
	if(m_emLinkStatus != emCameraLinkStatus_Linked)
		return false;

	/*::WSAEventSelect(m_ConnectSocket, m_wsaEvent, 0);
	unsigned long ul=0;
	ioctlsocket(m_ConnectSocket,FIONBIO,(unsigned long *)&ul);*/

	char buf[256] = {0};
	
	short iMsgtype=10;
	memcpy(buf,&iMsgtype,2);
	int ctrlType = bAuto ? 2 : 1;//1-maunal mode, 2-auto mode
	memcpy(buf+2,&ctrlType,4);
	int ret = send(m_ConnectSocket, buf, 6, 0);

	/*ul=1;
	ioctlsocket(m_ConnectSocket,FIONBIO,(unsigned long *)&ul);*/
	return ret > 0;
}

void CameraAutoModeSession::AcquireCameraLinkStatus( emCameraLinkStatus& status )
{
	status = m_emLinkStatus;
}

DWORD WINAPI CameraAutoModeSession::OnAutoThread( LPVOID pParam )
{
	CameraAutoModeSession* pThis = static_cast<CameraAutoModeSession*>(pParam);
	if(pThis)
		return pThis->OnAutoProc();
	return -1;
}

DWORD CameraAutoModeSession::OnAutoProc()
{
	if(strcmp(m_strIp.c_str(), "localhost") == 0)
	{
		hostent* localHost;
		char* localIP;
		localHost = gethostbyname("");
		localIP = inet_ntoa (*(struct in_addr *)*localHost->h_addr_list);
		m_strIp = localIP;
	}

	SOCKADDR_IN addrInfo;

	memset(&addrInfo, 0, sizeof(SOCKADDR_IN));   
	addrInfo.sin_addr.S_un.S_addr = inet_addr(m_strIp.c_str());   
	addrInfo.sin_family           = AF_INET;   
	addrInfo.sin_port             = htons(m_nPort);

	

	if(!m_wsaEvent)
	{
		m_wsaEvent = ::WSACreateEvent();
		if(!m_wsaEvent)
			return FALSE;
	}

	HANDLE arryHandle[2] = {0};
	arryHandle[0] = m_hAutoModeThreadClose;
	arryHandle[1] = m_wsaEvent;
	while(!m_bAutoModeThreadStop)
	{
		if( m_emLinkStatus == emCameraLinkStatus_Unlink)
		{
			m_emLinkStatus = emCameraLinkStatus_LinkWaiting;
			if(m_pClassCallback)
				m_pClassCallback->OnCameraLinkStatusNotify( emCameraCtrl_AutoMode, m_emLinkStatus);
		}

		if(m_emLinkStatus == emCameraLinkStatus_Linked)
		{
			unsigned long ul=1;
			ioctlsocket(m_ConnectSocket,FIONBIO,(unsigned long *)&ul);
			::WSAEventSelect(m_ConnectSocket, m_wsaEvent, FD_CLOSE);
		}
		else
		{
			m_ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			int ret = connect(m_ConnectSocket, (SOCKADDR *)&addrInfo, sizeof(SOCKADDR_IN));
			int iRet = WSAGetLastError();
			if( -1 == ret )
			{
				Sleep(2000);
				continue;
			}
			::WSAEventSelect(m_ConnectSocket, m_wsaEvent, FD_CONNECT | FD_CLOSE);
			
		}

		int nIndex;
		nIndex = ::WSAWaitForMultipleEvents(2, arryHandle, FALSE, WSA_INFINITE, FALSE);
			 
		if(nIndex == WSA_WAIT_FAILED)
		{
			Sleep(1000);
			continue;
		}
		else if(WSA_WAIT_EVENT_0 == nIndex)
		{
			break;
		}
		else
		{
			WSANETWORKEVENTS event;  
			::WSAEnumNetworkEvents(m_ConnectSocket, m_wsaEvent, &event);  
			if(event.lNetworkEvents & FD_CONNECT)
			{
				m_emLinkStatus = emCameraLinkStatus_Linked;
				if(m_pClassCallback)
					m_pClassCallback->OnCameraLinkStatusNotify( emCameraCtrl_AutoMode, m_emLinkStatus);
			}
			else if(event.lNetworkEvents & FD_CLOSE)
			{
				m_emLinkStatus = emCameraLinkStatus_Unlink;
				if(m_pClassCallback)
					m_pClassCallback->OnCameraLinkStatusNotify( emCameraCtrl_AutoMode, m_emLinkStatus);
				closesocket(m_ConnectSocket);
			}
		}
	}

	
	m_ConnectSocket = NULL;
	WSACloseEvent(m_wsaEvent);
}