
// UniversityDeviceManager.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "UniversityDeviceLookup.h"
#include "UniversityDeviceLookupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUniversityDeviceManagerApp

BEGIN_MESSAGE_MAP(CUniversityDeviceManagerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CUniversityDeviceManagerApp ����

CUniversityDeviceManagerApp::CUniversityDeviceManagerApp() : m_hOnceMutex(NULL)
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CUniversityDeviceManagerApp ����

CUniversityDeviceManagerApp theApp;


// CUniversityDeviceManagerApp ��ʼ��

BOOL CUniversityDeviceManagerApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�


	if(IsNotOnlyOnceRun(m_hOnceMutex, _T("UniversityDeviceLocker")))
	{
		AfxMessageBox(_T("ֻ������һ�����򣬻��߲��ܺͱ༭���ͬʱ������"));
		return FALSE;
	}



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

	CUniversityDeviceManagerDlg dlg;
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
	if(m_hOnceMutex)
	{
		ReleaseMutex(m_hOnceMutex);
	}
	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

BOOL CUniversityDeviceManagerApp::IsNotOnlyOnceRun( HANDLE& hMux, LPCTSTR pszAppName )
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