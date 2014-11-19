// SocketClient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "../../include/SYCameraCtrlInterface.h"
#include "../../include/TestDemoMacro.h"

struct SwitchCmd
{
	ULONGLONG  cmdframe; //Convert form FILETIME
	int 	  nChannel;
	int		  nFxNum;
	char	  UsrData[250];
};

std::string g_strIp;

bool g_bStop = false;

DWORD WINAPI SocketServer(LPVOID pvoid)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 1, 1 );
	bool bret;
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		return 0;
	}


	if ( LOBYTE( wsaData.wVersion ) != 1 ||
		HIBYTE( wsaData.wVersion ) != 1 ) {
			WSACleanup();
			return 0; 
	}

	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server == INVALID_SOCKET) 
	{
		WSACleanup();
		return 0;
	}
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
	service.sin_port = htons(SERVER_PORT);

	if (bind( server, 
		(SOCKADDR*) &service, 
		sizeof(service)) == SOCKET_ERROR) 
	{
		closesocket(server);
		return 0;
	}

	if (listen( server, 1 ) == SOCKET_ERROR)
	{
		return 0;
	}

	SOCKET client = accept( server, NULL, NULL );

	printf("server accept ok!\n");

	while(1)
	{
		char buf[64] = {0};
		int len = recv(client, buf, 256, 0);
		if(len >= 6)
		{
			int mode;
			memcpy(&mode, buf+2, 4);
			std::string str;
			if(mode == 2)
				str = "AutoMode";
			else if(mode == 1)
			{
				str = "ManualMode";
			}
			printf("camera auto mode is: %s\n", str);
		}

	}
	closesocket(client);
	closesocket(server);
	WSACleanup();
	return 0;
}

char g_c;

DWORD WINAPI SocketClient(LPVOID p)
{

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 1, 1 );
	bool bret;
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		return 0;
	}


	if ( LOBYTE( wsaData.wVersion ) != 1 ||
		HIBYTE( wsaData.wVersion ) != 1 ) {
			WSACleanup();
			return 0; 
	}
	if(strcmp(g_strIp.c_str(), "localhost") == 0)
	{
		hostent* localHost;
		char* localIP;
		localHost = gethostbyname("");
		localIP = inet_ntoa (*(struct in_addr *)*localHost->h_addr_list);
		g_strIp = localIP;
	}
	

	SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN addrInfo;   
	memset(&addrInfo, 0, sizeof(SOCKADDR_IN));   
	addrInfo.sin_addr.S_un.S_addr = inet_addr(g_strIp.c_str());   
	addrInfo.sin_family           = AF_INET;   
	addrInfo.sin_port             = htons(CHANNEL_LOCCAL_PORT_DEF);

	while(!g_bStop)
	{
		int ret = connect(client, (SOCKADDR *)&addrInfo, sizeof(SOCKADDR_IN));
		if(-1 ==ret)
		{
			printf("Reconnect server!\n");
			Sleep(2000);
			continue;
		}
		else
		{
			break;
		}
	}
	if(g_bStop)
		return 1;

	printf("Connect Ok!\n");

	while(!g_bStop)
	{
		SwitchCmd cmd = {0};
		cmd.nChannel = rand();
		send(client, (const char*)&cmd, sizeof(SwitchCmd), 0);
		Sleep(2000);
		
	}
	closesocket(client);
	WSACleanup();
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE sThread;
	HANDLE cThread;
	printf("Please choose link mode, input 's'(server) or 'c'(client):\n");
	while(1)
	{
		g_bStop = false;
		
		int c = getchar();
		if( c == 'e')
			break;
		else if(c == 's')
		{
			sThread = CreateThread(NULL, 0, SocketServer, 0,0,NULL);
			printf("input 'q' end current link!\n");
			while(1)
			{
				char cc = getchar();
				if(cc == 'q')
				{
					g_bStop = true;
					WaitForSingleObject(sThread, INFINITE);
					printf("Please choose link mode, input 's'(server) or 'c'(client):\n");
					break;
				}
			}
		}
		else if(c == 'c')
		{
			printf("Please server ip addr(if use local computer, please input 'localhost' string):\n");
			std::cin>>g_strIp;
			cThread = CreateThread(NULL, 0, SocketClient, 0,0,NULL);
			printf("input 'q' end current link!\n");
			while(1)
			{
				g_c = getchar();
				if(g_c == 'q')
				{
					g_bStop = true;
					WaitForSingleObject(cThread, INFINITE);
					printf("Please choose link mode, input 's'(server) or 'c'(client):\n");
					break;
				}
			}
		}
	}
	
	return 0;
}

