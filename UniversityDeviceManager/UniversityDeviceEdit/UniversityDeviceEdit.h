
// UniversityDeviceEdit.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CUniversityDeviceEditApp:
// �йش����ʵ�֣������ UniversityDeviceEdit.cpp
//

class CUniversityDeviceEditApp : public CWinApp
{
public:
	CUniversityDeviceEditApp();

	void ReleaseMuxterLocker();
	BOOL IsNotOnlyOnceRun(HANDLE& hMux, LPCTSTR pszAppName);
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	
	
public:
	HANDLE m_hOnceMutex;
};

extern CUniversityDeviceEditApp theApp;