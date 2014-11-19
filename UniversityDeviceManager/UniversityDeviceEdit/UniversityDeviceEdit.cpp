
// UniversityDeviceEdit.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "UniversityDeviceEdit.h"
#include "UniversityDeviceEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUniversityDeviceEditApp

BEGIN_MESSAGE_MAP(CUniversityDeviceEditApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CUniversityDeviceEditApp ����

CUniversityDeviceEditApp::CUniversityDeviceEditApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CUniversityDeviceEditApp ����

CUniversityDeviceEditApp theApp;
bool g_bRestart = false;

void exit_fn1(void)
{
	theApp.ReleaseMuxterLocker();
	if(g_bRestart)
	{
		TCHAR buf[256];
		::GetModuleFileName(NULL,buf,sizeof(buf));
		CString strPath = buf;
		USES_CONVERSION;
		WinExec(T2A(strPath), SW_SHOW);//����strPath·���µ�ִ���ļ�
	}
	return;
}
// CUniversityDeviceEditApp ��ʼ��

BOOL CUniversityDeviceEditApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�

	if(IsNotOnlyOnceRun(m_hOnceMutex, _T("UniversityDeviceLocker")))
	{
		AfxMessageBox(_T("ֻ������һ�����򣬻��߲��ܺͲ�ѯ���ͬʱ������"));
		return FALSE;
	}
	atexit(exit_fn1);
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CUniversityDeviceEditDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}
	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

BOOL CUniversityDeviceEditApp::IsNotOnlyOnceRun( HANDLE& hMux, LPCTSTR pszAppName )
{
	ASSERT(hMux == NULL);
	hMux = ::CreateMutex(NULL, TRUE, pszAppName);
	if (hMux == NULL || ::GetLastError() != ERROR_ALREADY_EXISTS)
		return FALSE;

	CWnd* pTopLevelWnd = CWnd::GetDesktopWindow()->GetTopWindow();
	while (pTopLevelWnd != NULL)
	{
		if (::GetProp(pTopLevelWnd->GetSafeHwnd(), pszAppName) != NULL)
		{
			if (pTopLevelWnd->IsIconic())
				pTopLevelWnd->ShowWindow(SW_RESTORE);
			pTopLevelWnd->SetForegroundWindow();
			pTopLevelWnd->GetLastActivePopup()->SetForegroundWindow();
			break;
		}
		pTopLevelWnd = pTopLevelWnd->GetNextWindow();
	}
	return TRUE;
}

void CUniversityDeviceEditApp::ReleaseMuxterLocker()
{
	if(m_hOnceMutex)
	{
		::ReleaseMutex(m_hOnceMutex);
		CloseHandle(m_hOnceMutex);
		m_hOnceMutex = NULL;
	}
}