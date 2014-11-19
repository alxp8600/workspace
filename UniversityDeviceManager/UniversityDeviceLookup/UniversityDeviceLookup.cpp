
// UniversityDeviceManager.cpp : 定义应用程序的类行为。
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


// CUniversityDeviceManagerApp 构造

CUniversityDeviceManagerApp::CUniversityDeviceManagerApp() : m_hOnceMutex(NULL)
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CUniversityDeviceManagerApp 对象

CUniversityDeviceManagerApp theApp;


// CUniversityDeviceManagerApp 初始化

BOOL CUniversityDeviceManagerApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。


	if(IsNotOnlyOnceRun(m_hOnceMutex, _T("UniversityDeviceLocker")))
	{
		AfxMessageBox(_T("只能启动一个程序，或者不能和编辑软件同时启动！"));
		return FALSE;
	}



	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CUniversityDeviceManagerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}
	if(m_hOnceMutex)
	{
		ReleaseMutex(m_hOnceMutex);
	}
	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
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