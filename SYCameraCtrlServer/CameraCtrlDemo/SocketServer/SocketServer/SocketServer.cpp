// SocketServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../../include/SYCameraCtrlInterface.h"
#include "../../include/TestDemoMacro.h"

std::string g_strIp;
class DemoClass : public ICameraCtrlServerCallback
{
public:
	virtual void OnRecvCameraChannelCallback(unsigned short nChannel);
	virtual void OnCameraLinkStatusNotify(emCameraCtrlType type, emCameraLinkStatus status);
};

void DemoClass::OnRecvCameraChannelCallback( unsigned short nChannel )
{
	printf("Recv channel No. is %d\n", nChannel);
}

void DemoClass::OnCameraLinkStatusNotify( emCameraCtrlType type, emCameraLinkStatus status )
{
	std::string strType;
	std::string strStatus;
	switch (type)
	{
	case emCameraCtrl_AutoMode:
		strType = "AutoMode";
		break;
	case emCameraCtrl_Channel:
		strType = "Channel";
		break;
	}
	switch (status)
	{
	case emCameraLinkStatus_Unlink:
		strStatus = "Unlink";
		break;
	case emCameraLinkStatus_LinkWaiting:
		strStatus = "LinkWaiting";
		break;
	case emCameraLinkStatus_Linked:
		strStatus = "Linked";
		break;
	}
	printf("Camera ctrl type[%s]'s status: %s\n", strType, strStatus);
}

void TestCameraDll()
{
	DemoClass demo;
	CameraCtrlServer server(g_strIp.c_str(), SERVER_PORT);
	server.SetClassCallback(&demo);
	server.Start();

	while(1)
	{
		char c = getchar();
		if(c == 'q')
			break;
		else if( c == '2')
		{
			if(server.SetAutoMode())
				printf("SetAutoMode OK! --------------------------------------------------------\n");
		}
		else if(c == '1')
		{
			if(server.SetAutoMode(false))
			{
				printf("SetMaunalMode OK! --------------------------------------------------------\n");
			}
		}
	}
	server.Stop();
}

int _tmain(int argc, _TCHAR* argv[])
{
	printf("When auto mode link is success, please input '1' char to set manual mode, or input '2' char to set auto mode!\n");
	printf("Please input ip addr(if use local computer, please input 'localhost' string):\n");
	std::cin>>g_strIp;
	TestCameraDll();
	return 0;
}

