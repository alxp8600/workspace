#include "StdAfx.h"
#include "CameraCtrlServerImpl.h"


CameraCtrlServer::CameraCtrlServer( const char* ip, unsigned int port, RecvCameraChannelFunc pCallbackFunc )
{
	m_pImpl = new CameraCtrlServerImpl(ip, port, pCallbackFunc);
}

CameraCtrlServer::~CameraCtrlServer()
{
	if( m_pImpl)
		delete m_pImpl;
}

void CameraCtrlServer::SetClassCallback( ICameraCtrlServerCallback* pClass )
{
	if(m_pImpl)
		m_pImpl->SetClassCallback(pClass);
}

bool CameraCtrlServer::Start()
{
	if(m_pImpl)
		return m_pImpl->Start();
	return false;
}

bool CameraCtrlServer::Stop()
{
	if(m_pImpl)
		return m_pImpl->Stop();
	return false;
}

bool CameraCtrlServer::SetAutoMode( bool bAuto /*= true*/ )
{
	if(m_pImpl)
		return m_pImpl->SetAutoMode(bAuto);
	return false;
}

void CameraCtrlServer::AcquireCameraLinkStatus( emCameraCtrlType type, emCameraLinkStatus& status )
{
	if(m_pImpl)
		m_pImpl->AcquireCameraLinkStatus(type, status);
}


CameraCtrlServerImpl::CameraCtrlServerImpl(const char* ip, unsigned short port, RecvCameraChannelFunc pCallbackFunc) : m_channelSession(pCallbackFunc)
																													, m_autoSession(ip, port, pCallbackFunc)
{
}


CameraCtrlServerImpl::~CameraCtrlServerImpl(void)
{
}

bool CameraCtrlServerImpl::Start()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );
	bool bret;
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		return false;
	}


	if ( LOBYTE( wsaData.wVersion ) != 2 ||
		HIBYTE( wsaData.wVersion ) != 2 ) {
			WSACleanup();
			return false; 
	}
	bret = m_channelSession.Start();
	bret = m_autoSession.Start();
	return bret;
}

bool CameraCtrlServerImpl::Stop()
{
	bool bret = true;
	bret = m_channelSession.Stop();
	bret = m_autoSession.Stop();
	WSACleanup();
	return bret;
}

bool CameraCtrlServerImpl::SetAutoMode( bool bAuto /*= true*/ )
{
	return m_autoSession.SetAutoMode(bAuto);
}

void CameraCtrlServerImpl::AcquireCameraLinkStatus( emCameraCtrlType type, emCameraLinkStatus& status )
{
	switch (type)
	{
	case emCameraCtrl_Channel:
		m_channelSession.AcquireCameraLinkStatus(status);
		break;
	case emCameraCtrl_AutoMode:
		m_autoSession.AcquireCameraLinkStatus(status);
		break;
	}
}

void CameraCtrlServerImpl::SetClassCallback( ICameraCtrlServerCallback* pClass )
{
	m_channelSession.SetClassCallback(pClass);
	m_autoSession.SetClassCallback(pClass);
}
