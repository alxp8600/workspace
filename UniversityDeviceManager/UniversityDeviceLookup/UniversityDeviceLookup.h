
// UniversityDeviceManager.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CUniversityDeviceManagerApp:
// �йش����ʵ�֣������ UniversityDeviceManager.cpp
//

class CUniversityDeviceManagerApp : public CWinApp
{
public:
	CUniversityDeviceManagerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()


	BOOL IsNotOnlyOnceRun(HANDLE& hMux, LPCTSTR pszAppName);

private:
	HANDLE m_hOnceMutex;
};

extern CUniversityDeviceManagerApp theApp;