#include "StdAfx.h"
#include "CameraCtrlServerImpl.h"
#include "CameraChannelSession.h"


CameraChannelSession::CameraChannelSession(RecvCameraChannelFunc pCallbackFunc) : m_pCallbackFunc(pCallbackFunc)
																				, m_pClassCallback(NULL)
																				, m_hChannelThread(NULL)
																				, m_bChannelThreadStop(true)
																				, m_hChannelThreadClose(NULL)
																				, m_ListenSocket(NULL)
																				, m_AcceptSocket(NULL)
																				, m_wsaEvent(NULL)
																				, m_emLinkStatus(emCameraLinkStatus_Unlink)
{
}


CameraChannelSession::~CameraChannelSession(void)
{
}

void CameraChannelSession::SetClassCallback( ICameraCtrlServerCallback* pClass )
{
	m_pClassCallback = pClass;
}

bool CameraChannelSession::Start()
{
	bool bret;
	m_bChannelThreadStop = false;
	if(!m_hChannelThreadClose)
		m_hChannelThreadClose = CreateEvent(NULL, FALSE, FALSE, NULL);
	if(!m_hChannelThread)
		m_hChannelThread = CreateThread(NULL, 0, OnChannelThread, this, 0, NULL);
	bret = m_hChannelThread != NULL ? true : false;
	return bret;
}

bool CameraChannelSession::Stop()
{
	bool bret;
	if(m_hChannelThread)
	{
		m_bChannelThreadStop = true;
		SetEvent(m_hChannelThreadClose);
		DWORD dwRet = WaitForSingleObject(m_hChannelThread, 3000);
		if(dwRet == WAIT_TIMEOUT)
			bret = false;
		CloseHandle(m_hChannelThread);
		m_hChannelThread = NULL;
	}
	if(m_hChannelThreadClose)
		CloseHandle(m_hChannelThreadClose);
	return bret;
}

void CameraChannelSession::AcquireCameraLinkStatus( emCameraLinkStatus& status )
{
	status = m_emLinkStatus;
}

DWORD WINAPI CameraChannelSession::OnChannelThread( LPVOID pParam )
{
	CameraChannelSession* pThis = static_cast<CameraChannelSession*>(pParam);
	if(pThis)
		return pThis->OnChannelProc();
	return -1;
}

DWORD CameraChannelSession::OnChannelProc()
{
	m_ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_ListenSocket == INVALID_SOCKET) 
	{
		WSACleanup();
		return false;
	}
	unsigned long ul=1;
	ioctlsocket(m_ListenSocket,FIONBIO,(unsigned long *)&ul);
	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port for the socket that is being bound.
	hostent* localHost;
	char* localIP;
	localHost = gethostbyname("");
	localIP = inet_ntoa (*(struct in_addr *)*localHost->h_addr_list);

	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr(localIP);
	service.sin_port = htons(CHANNEL_LOCCAL_PORT_DEF);

	if(!m_wsaEvent)
	{
		m_wsaEvent = ::WSACreateEvent();
		if(!m_wsaEvent)
			return FALSE;
	}

	if (bind( m_ListenSocket, 
		(SOCKADDR*) &service, 
		sizeof(service)) == SOCKET_ERROR) 
	{
		closesocket(m_ListenSocket);
		return false;
	}

	//----------------------
	// Listen for incoming connection requests.
	// on the created socket
	if (listen( m_ListenSocket, 1 ) == SOCKET_ERROR)
	{
		return false;
	}

	//----------------------
	// Create a SOCKET for accepting incoming requests.

	m_AcceptSocket = SOCKET_ERROR;

	char pBuffer[512] = {0};
	int bytesRecv = 0;
	int totalRecv = 0;

	HANDLE arryHandle[2] = {0};
	arryHandle[0] = m_hChannelThreadClose;
	arryHandle[1] = m_wsaEvent;

	while( !m_bChannelThreadStop ) 
	{
		if(m_emLinkStatus == emCameraLinkStatus_Unlink)
		{
			m_emLinkStatus = emCameraLinkStatus_LinkWaiting;
			::WSAEventSelect(m_ListenSocket, m_wsaEvent, FD_ACCEPT | FD_CLOSE);
			if(m_pClassCallback)
				m_pClassCallback->OnCameraLinkStatusNotify( emCameraCtrl_Channel, m_emLinkStatus);
		}
		else if(m_emLinkStatus == emCameraLinkStatus_Linked)
		{
			::WSAEventSelect(m_AcceptSocket, m_wsaEvent, FD_CLOSE | FD_READ );
		}

		int nIndex = ::WSAWaitForMultipleEvents(2, arryHandle, FALSE, WSA_INFINITE, FALSE);  
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
			if(m_emLinkStatus != emCameraLinkStatus_Linked)
			{
				::WSAEnumNetworkEvents(m_ListenSocket, m_wsaEvent, &event);
			}
			else
			{
				::WSAEnumNetworkEvents(m_AcceptSocket, m_wsaEvent, &event);
			}
			 
			if(event.lNetworkEvents & FD_ACCEPT)
			{
				m_emLinkStatus = emCameraLinkStatus_Linked;
				m_AcceptSocket = accept( m_ListenSocket, NULL, NULL );
				if(m_pClassCallback)
					m_pClassCallback->OnCameraLinkStatusNotify( emCameraCtrl_Channel, m_emLinkStatus);
			}
			else if(event.lNetworkEvents & FD_CLOSE)
			{
				closesocket(m_AcceptSocket);
				m_emLinkStatus = emCameraLinkStatus_Unlink;
				if(m_pClassCallback)
					m_pClassCallback->OnCameraLinkStatusNotify( emCameraCtrl_Channel, m_emLinkStatus);
			}
			else if(event.lNetworkEvents & FD_READ)
			{
				SwitchCmd cmdmsg;
				bytesRecv = recv(m_AcceptSocket, pBuffer + totalRecv, 512 - totalRecv, 0 );
				totalRecv += bytesRecv;
				if(totalRecv >= sizeof(SwitchCmd))
				{
					memcpy(&cmdmsg, pBuffer, sizeof(SwitchCmd));
					memcpy(pBuffer, pBuffer+sizeof(SwitchCmd), 512 - sizeof(SwitchCmd));
					totalRecv -= sizeof(SwitchCmd);
					if(m_pCallbackFunc)
					{
						m_pCallbackFunc(cmdmsg.nChannel);
					}
					if(m_pClassCallback)
					{
						m_pClassCallback->OnRecvCameraChannelCallback(cmdmsg.nChannel);
					}
				}
			}
		}
	}
	closesocket(m_ListenSocket);
	WSACloseEvent(m_wsaEvent);
}