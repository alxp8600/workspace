
// UniversityDeviceManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UniversityDeviceLookup.h"
#include "UniversityDeviceLookupDlg.h"
#include "afxdialogex.h"
#include "DlgPriceChart.h"
#include "DlgDevFilter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUniversityDeviceManagerDlg 对话框




CUniversityDeviceManagerDlg::CUniversityDeviceManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUniversityDeviceManagerDlg::IDD, pParent)
	, m_dwDevID(0)
	, m_lstDev(Custom_Device)
	, m_nCurUnivCombIndex(-1)
	, m_nCurAcaCombIndex(-1)
	, m_csCurUnivName(_T(""))
	, m_csCurAcaName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUniversityDeviceManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DEV_ID, m_dwDevID);
	DDX_Control(pDX, IDC_LIST_DEVICELIST, m_lstDev);
	DDX_Control(pDX, IDC_COMBO_UNIVERSITY, m_combUniv);
	DDX_Control(pDX, IDC_COMBO_ACADEMY, m_combAcademy);
}

BEGIN_MESSAGE_MAP(CUniversityDeviceManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_LOOKUP, &CUniversityDeviceManagerDlg::OnBnClickedBtnLookup)
	ON_CBN_SELCHANGE(IDC_COMBO_UNIVERSITY, &CUniversityDeviceManagerDlg::OnCbnSelchangeComboUniversity)
	ON_CBN_SELCHANGE(IDC_COMBO_ACADEMY, &CUniversityDeviceManagerDlg::OnCbnSelchangeComboAcademy)
	ON_MESSAGE(WM_LIST_ITEM_CHANGED_MSG, &CUniversityDeviceManagerDlg::OnDevItemChangedMsg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_PRICE_CHART, &CUniversityDeviceManagerDlg::OnBnClickedBtnPriceChart)
	ON_BN_CLICKED(IDC_BTN_PRICE_CHART_UNI, &CUniversityDeviceManagerDlg::OnBnClickedBtnPriceChartUni)
	ON_BN_CLICKED(IDC_BTN_LOOK_UPLOAD_DATA, &CUniversityDeviceManagerDlg::OnBnClickedBtnLookUploadData)
	ON_BN_CLICKED(IDC_BTN_DEV_FILTER, &CUniversityDeviceManagerDlg::OnBnClickedBtnDevFilter)
END_MESSAGE_MAP()


// CUniversityDeviceManagerDlg 消息处理程序

BOOL CUniversityDeviceManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	m_bkColor = GetSysColor(COLOR_3DFACE);
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_stcImgShow.SubclassWindow(GetDlgItem(IDC_PIC_DEV)->m_hWnd);
	m_stcImgShow2.SubclassWindow(GetDlgItem(IDC_PIC_DEV2)->m_hWnd);
	m_stcImgShow3.SubclassWindow(GetDlgItem(IDC_PIC_DEV3)->m_hWnd);
	CRect rc;
	m_stcImgShow.GetWindowRect(rc);
	rc.right = rc.left + 160;
	rc.bottom = rc.top + 120;
	ScreenToClient(rc);
	m_stcImgShow.MoveWindow(rc);

	m_stcImgShow2.GetWindowRect(rc);
	rc.right = rc.left + 160;
	rc.bottom = rc.top + 120;
	ScreenToClient(rc);
	m_stcImgShow2.MoveWindow(rc);

	m_stcImgShow3.GetWindowRect(rc);
	rc.right = rc.left + 160;
	rc.bottom = rc.top + 120;
	ScreenToClient(rc);
	m_stcImgShow3.MoveWindow(rc);


	DWORD dwStyle = m_lstDev.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT; 
	dwStyle |= LVS_EX_GRIDLINES;
	m_lstDev.SetExtendedStyle(dwStyle);
	m_lstDev.SetColumNum(8);
	m_lstDev.InsertListColumn(0,_T("设备编号"),LVCFMT_CENTER, 80);
	m_lstDev.InsertListColumn(1,_T("设备名称"),LVCFMT_CENTER, 80);
	m_lstDev.InsertListColumn(2,_T("设备型号"),LVCFMT_CENTER, 80);
	m_lstDev.InsertListColumn(3,_T("设备分类"),LVCFMT_CENTER, 110);
	m_lstDev.InsertListColumn(4,_T("单价（元）"),LVCFMT_CENTER, 80);
	m_lstDev.InsertListColumn(5,_T("入账日期"),LVCFMT_CENTER, 80);
	m_lstDev.InsertListColumn(6,_T("保管人"),LVCFMT_CENTER, 80);
	m_lstDev.InsertListColumn(7,_T("设备所在地"),LVCFMT_CENTER, 110);
	
	RefreshUnivComb();


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CUniversityDeviceManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUniversityDeviceManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);

		
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CUniversityDeviceManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUniversityDeviceManagerDlg::OnBnClickedBtnLookup()
{
	UpdateData();
	CString strID;
	strID.Format(_T("%d"), m_dwDevID);
	if(m_nCurUnivCombIndex >= 0 && m_nCurUnivCombIndex < m_csUnivLst.size())
	{
		CString strModel = PathManager::GetCurrentModelPath();
		CString strUni = _T("data\\") + m_csUnivLst[m_nCurUnivCombIndex] + _T("\\");
		for (int i = 0; i < m_csAcaLst.size(); i++)
		{
			CString strXmlDev = strModel + strUni + m_csAcaLst[i] + _T("\\") + m_csAcaLst[i] + _T(".xml");
			m_xmlDev.SetXmlPath(strXmlDev);
			if(m_xmlDev.GetDeviceById(strID, m_curDevInfo))
			{
				m_csCurAcaName = m_csAcaLst[i];
				m_csCurUnivName = m_csUnivLst[m_nCurUnivCombIndex];
				ShowDevInfo();
				m_lstDev.SetItemState(-1, 0, -1);
				return;
			}
		}
		MessageBox(_T("未找到匹配的设备编号！"));
	}
	else
	{
		MessageBox(_T("请选择一所大学！"));
	}
}

void CUniversityDeviceManagerDlg::RefreshUnivComb()
{
	CString strUniv = PathManager::GetCurrentModelPath();
	strUniv += _T("data\\university.xml");
	m_xmlUniv.SetXmlPath(strUniv);
	m_xmlUniv.GetUniversityList(m_csUnivLst);
	m_combUniv.ResetContent();
	for (int i = 0; i < m_csUnivLst.size() ; i++)
	{
		m_combUniv.AddString(m_csUnivLst[i]);
	}
}

void CUniversityDeviceManagerDlg::RefreshAcademyComb()
{
	if(m_nCurUnivCombIndex >= 0 && m_nCurUnivCombIndex < m_csUnivLst.size())
	{
		CString strAca = PathManager::GetCurrentModelPath();
		strAca += _T("data\\");
		CString str = m_csUnivLst[m_nCurUnivCombIndex] + _T("\\") + m_csUnivLst[m_nCurUnivCombIndex] +_T(".xml");
		strAca += str;
		m_xmlAcademy.SetXmlPath(strAca);
		m_xmlAcademy.GetAcademyList(m_csAcaLst);
		m_combAcademy.ResetContent();
		for (int i = 0; i < m_csAcaLst.size(); i++)
		{
			m_combAcademy.AddString(m_csAcaLst[i]);
		}
	}
}

void CUniversityDeviceManagerDlg::RefreshDevList()
{
	if(m_nCurAcaCombIndex >= 0 && m_nCurAcaCombIndex < m_csAcaLst.size())
	{
		CString strDev = PathManager::GetCurrentModelPath();
		strDev += _T("data\\");
		CString str = m_csUnivLst[m_nCurUnivCombIndex] + _T("\\") + m_csAcaLst[m_nCurAcaCombIndex] + _T("\\") + m_csAcaLst[m_nCurAcaCombIndex] +_T(".xml");
		strDev += str;
		m_xmlDev.SetXmlPath(strDev);
		DWORD nAllPrice = 0;
		m_lstDev.DeleteAllItems();
		int nDevNum = m_xmlDev.GetDeviceNum(&nAllPrice);
		for (int i = nDevNum-1; i >= 0; i--)
		{
			DeviceInfo info;
			info = m_xmlDev.GetDeviceByIndex(i);
			int nRow = m_lstDev.InsertItem(0, _T(""));
			m_lstDev.SetItemText(nRow,0,info.csDevId);
			m_lstDev.SetItemText(nRow,1,info.csDevName);
			m_lstDev.SetItemText(nRow,2,info.csDevType);
			m_lstDev.SetItemText(nRow,3,info.csSortName);
			m_lstDev.SetItemText(nRow,4,info.csPrice);
			m_lstDev.SetItemText(nRow,5,info.csEntryDate);
			m_lstDev.SetItemText(nRow,6,info.csKeeper);
			m_lstDev.SetItemText(nRow,7,info.csDevNowPlace);
		}
		CString strNum;
		strNum.Format(_T("%d"), nDevNum);
		GetDlgItem(IDC_STATIC_ACA_DEV_NUM)->SetWindowText(strNum);
		strNum.Format(_T("%d"), nAllPrice);
		GetDlgItem(IDC_STATIC_ACA_DEV_ALL_PRICE)->SetWindowText(strNum);
	}
}

void CUniversityDeviceManagerDlg::OnCbnSelchangeComboUniversity()
{
	int index = m_combUniv.GetCurSel();
	if(index == m_nCurUnivCombIndex)
		return;
	m_nCurUnivCombIndex = index;
	RefreshAcademyComb();
	if(m_nCurUnivCombIndex >= 0 && m_nCurUnivCombIndex < m_csUnivLst.size())
	{
		CString strModel = PathManager::GetCurrentModelPath();
		CString strUni = _T("data\\") + m_csUnivLst[m_nCurUnivCombIndex] + _T("\\");
		int nAllDev = 0;
		DWORD dwAllPrice = 0;
		for (int i = 0; i < m_csAcaLst.size(); i++)
		{
			CString strXmlDev = strModel + strUni + m_csAcaLst[i] + _T("\\") + m_csAcaLst[i] + _T(".xml");
			m_xmlDev.SetXmlPath(strXmlDev);
			DWORD dwDevPrice = 0;
			int nDevNum = m_xmlDev.GetDeviceNum(&dwDevPrice);
			nAllDev += nDevNum;
			dwAllPrice += dwDevPrice;
		}
		CString str;
		str.Format(_T("%d"), nAllDev);
		GetDlgItem(IDC_STATIC_UNI_DEV_NUM)->SetWindowText(str);
		str.Format(_T("%d"), dwAllPrice);
		GetDlgItem(IDC_STATIC_UNI_DEV_ALL_PRICE)->SetWindowText(str);

		GetDlgItem(IDC_STATIC_ACA_DEV_NUM)->SetWindowText(_T("0"));
		GetDlgItem(IDC_STATIC_ACA_DEV_ALL_PRICE)->SetWindowText(_T("0"));
		m_lstDev.DeleteAllItems();

		m_curDevInfo = DeviceInfo();
		m_csCurUnivName = m_csUnivLst[m_nCurUnivCombIndex];
		m_csCurAcaName = _T("");
		m_nCurAcaCombIndex = -1;
		ShowDevInfo();
	}
}


void CUniversityDeviceManagerDlg::OnCbnSelchangeComboAcademy()
{
	int index = m_combAcademy.GetCurSel();
	if(index == m_nCurAcaCombIndex)
		return;
	m_nCurAcaCombIndex = index;
	RefreshDevList();
	m_curDevInfo = DeviceInfo();
	m_csCurAcaName = m_csAcaLst[m_nCurAcaCombIndex];
	ShowDevInfo();
}

void CUniversityDeviceManagerDlg::ShowDevInfo()
{
	if(!m_csCurUnivName.IsEmpty())
		GetDlgItem(IDC_STATIC_DESCR_UNIV)->SetWindowText(m_csCurUnivName);
	else
		GetDlgItem(IDC_STATIC_DESCR_UNIV)->SetWindowText(_T("未知"));

	if(!m_csCurAcaName.IsEmpty())
		GetDlgItem(IDC_STATIC_DESCR_ACA)->SetWindowText(m_csCurAcaName);
	else
		GetDlgItem(IDC_STATIC_DESCR_ACA)->SetWindowText(_T("未知"));
	
	if(m_curDevInfo.csDevId.IsEmpty())
		GetDlgItem(IDC_STATIC_DESCR_ID)->SetWindowText(_T("未知"));
	else
		GetDlgItem(IDC_STATIC_DESCR_ID)->SetWindowText(m_curDevInfo.csDevId);

	if(m_curDevInfo.csSortName.IsEmpty())
		GetDlgItem(IDC_STATIC_DESCR_SORT)->SetWindowText(_T("未知"));
	else
		GetDlgItem(IDC_STATIC_DESCR_SORT)->SetWindowText(m_curDevInfo.csSortName);

	if(m_curDevInfo.csPrice.IsEmpty())
		GetDlgItem(IDC_STATIC_DESCR_PRICE)->SetWindowText(_T("未知"));
	else
		GetDlgItem(IDC_STATIC_DESCR_PRICE)->SetWindowText(m_curDevInfo.csPrice);

	if(m_curDevInfo.csEntryDate.IsEmpty())
		GetDlgItem(IDC_STATIC_DESCR_DATE)->SetWindowText(_T("未知"));
	else
		GetDlgItem(IDC_STATIC_DESCR_DATE)->SetWindowText(m_curDevInfo.csEntryDate);

	if(m_curDevInfo.csKeeper.IsEmpty())
		GetDlgItem(IDC_STATIC_DESCR_KEEPER)->SetWindowText(_T("未知"));
	else
		GetDlgItem(IDC_STATIC_DESCR_KEEPER)->SetWindowText(m_curDevInfo.csKeeper);

	if(m_curDevInfo.csDevName.IsEmpty())
		GetDlgItem(IDC_STATIC_DESCR_DEV_NAME)->SetWindowText(_T("未知"));
	else
		GetDlgItem(IDC_STATIC_DESCR_DEV_NAME)->SetWindowText(m_curDevInfo.csDevName);

	if(m_curDevInfo.csDevType.IsEmpty())
		GetDlgItem(IDC_STATIC_DESCR_DEV_TYPE)->SetWindowText(_T("未知"));
	else
		GetDlgItem(IDC_STATIC_DESCR_DEV_TYPE)->SetWindowText(m_curDevInfo.csDevType);

	if(m_curDevInfo.csPhone.IsEmpty())
		GetDlgItem(IDC_STATIC_DESCR_PHONE)->SetWindowText(_T("未知"));
	else
		GetDlgItem(IDC_STATIC_DESCR_PHONE)->SetWindowText(m_curDevInfo.csPhone);

	if(m_curDevInfo.csEmail.IsEmpty())
		GetDlgItem(IDC_STATIC_DESCR_EMAIL)->SetWindowText(_T("未知"));
	else
		GetDlgItem(IDC_STATIC_DESCR_EMAIL)->SetWindowText(m_curDevInfo.csEmail);

	if(m_curDevInfo.csFactoryDate.IsEmpty())
		GetDlgItem(IDC_STATIC_DESCR_FACT_DATE)->SetWindowText(_T("未知"));
	else
		GetDlgItem(IDC_STATIC_DESCR_FACT_DATE)->SetWindowText(m_curDevInfo.csFactoryDate);

	if(m_curDevInfo.csAccptanceDate.IsEmpty())
		GetDlgItem(IDC_STATIC_DESCR_ACCP_DATE)->SetWindowText(_T("未知"));
	else
		GetDlgItem(IDC_STATIC_DESCR_ACCP_DATE)->SetWindowText(m_curDevInfo.csAccptanceDate);

	if(m_curDevInfo.csScrapDate.IsEmpty())
		GetDlgItem(IDC_STATIC_DESCR_SCRAP_DATE)->SetWindowText(_T("未知"));
	else
		GetDlgItem(IDC_STATIC_DESCR_SCRAP_DATE)->SetWindowText(m_curDevInfo.csScrapDate);

	if(m_curDevInfo.csDevNowPlace.IsEmpty())
		GetDlgItem(IDC_STATIC_DESCR_NOW_PLACE)->SetWindowText(_T("未知"));
	else
		GetDlgItem(IDC_STATIC_DESCR_NOW_PLACE)->SetWindowText(m_curDevInfo.csDevNowPlace);

	if(m_curDevInfo.csOriginPlace.IsEmpty())
		GetDlgItem(IDC_STATIC_DESCR_ORIGIN_PLACE)->SetWindowText(_T("未知"));
	else
		GetDlgItem(IDC_STATIC_DESCR_ORIGIN_PLACE)->SetWindowText(m_curDevInfo.csOriginPlace);

	if(m_curDevInfo.csTechNorm.IsEmpty())
		GetDlgItem(IDC_STATIC_DESCR_NORM)->SetWindowText(_T("未知"));
	else
		GetDlgItem(IDC_STATIC_DESCR_NORM)->SetWindowText(m_curDevInfo.csTechNorm);

	if(m_curDevInfo.csRemark.IsEmpty())
		GetDlgItem(IDC_STATIC_DESCR_REMARK)->SetWindowText(_T("未知"));
	else
		GetDlgItem(IDC_STATIC_DESCR_REMARK)->SetWindowText(m_curDevInfo.csRemark);



	CString str = PathManager::GetCurrentModelPath();
	CString strfull = str + m_curDevInfo.csImgPath;
	if(!m_curDevInfo.csImgPath.IsEmpty() && PathFileExists(strfull))
	{
		m_stcImgShow.SetCtrlBkColor(FALSE);
		m_stcImgShow.SetStaticBitmap(strfull);
	}
	else
	{
		//m_stcImgShow.ModifyStyle( SS_CENTERIMAGE,  0);
		m_stcImgShow.CloseStaticBitmap();
		m_stcImgShow.SetCtrlBkColor(1, m_bkColor);
	}
	strfull = str + m_curDevInfo.csImgPath2;
	if(!m_curDevInfo.csImgPath2.IsEmpty() && PathFileExists(strfull))
	{
		m_stcImgShow2.SetCtrlBkColor(FALSE);
		m_stcImgShow2.SetStaticBitmap(strfull);
	}
	else
	{
		m_stcImgShow2.CloseStaticBitmap();
		m_stcImgShow2.SetCtrlBkColor(1, m_bkColor);
	}

	strfull = str + m_curDevInfo.csImgPath3;
	if(!m_curDevInfo.csImgPath3.IsEmpty() && PathFileExists(strfull))
	{
		m_stcImgShow3.SetCtrlBkColor(FALSE);
		m_stcImgShow3.SetStaticBitmap(strfull);
	}
	else
	{
		m_stcImgShow3.CloseStaticBitmap();
		m_stcImgShow3.SetCtrlBkColor(1, m_bkColor);
	}

	strfull = str + m_curDevInfo.csUploadDataPath;
	if( !m_curDevInfo.csUploadDataPath.IsEmpty() && PathFileExists(strfull))
		GetDlgItem(IDC_BTN_LOOK_UPLOAD_DATA)->ShowWindow(SW_SHOW);
	else
		GetDlgItem(IDC_BTN_LOOK_UPLOAD_DATA)->ShowWindow(SW_HIDE);

	m_stcImgShow.Invalidate(TRUE);
	m_stcImgShow2.Invalidate(TRUE);
	m_stcImgShow3.Invalidate(TRUE);
}

LRESULT CUniversityDeviceManagerDlg::OnDevItemChangedMsg( WPARAM wParam, LPARAM lParam )
{
	emCustomList emLst = static_cast<emCustomList>(wParam);
	switch (emLst)
	{
	case Custom_Device:
		CtrlDeviceItemChanged(lParam);
		break;
	}
	return S_OK;
}

void CUniversityDeviceManagerDlg::CtrlDeviceItemChanged( int index )
{
	if(index >= 0 && index < m_lstDev.GetItemCount())
	{
		CString strDev = PathManager::GetCurrentModelPath();
		strDev += _T("data\\");
		CString str = m_csUnivLst[m_nCurUnivCombIndex] + _T("\\") + m_csAcaLst[m_nCurAcaCombIndex] + _T("\\") + m_csAcaLst[m_nCurAcaCombIndex] +_T(".xml");
		strDev += str;
		m_xmlDev.SetXmlPath(strDev);
		m_curDevInfo = m_xmlDev.GetDeviceByIndex(index);
		m_csCurUnivName = m_csUnivLst[m_nCurUnivCombIndex];
		m_csCurAcaName = m_csAcaLst[m_nCurAcaCombIndex];
		ShowDevInfo();
	}
}

HBRUSH CUniversityDeviceManagerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

BOOL CUniversityDeviceManagerDlg::StatPriceEveryYearByAcademy(int nUniIndex, int nAcaIndex, PriceMap& mapPrice)
{
	if((nUniIndex < 0 && nUniIndex >= m_csUnivLst.size())
		|| (nAcaIndex < 0 && nAcaIndex >= m_csAcaLst.size()))
		return FALSE;

	CString strDev = PathManager::GetCurrentModelPath();
	strDev += _T("data\\");
	CString str = m_csUnivLst[nUniIndex] + _T("\\") + m_csAcaLst[nAcaIndex] + _T("\\") + m_csAcaLst[nAcaIndex] +_T(".xml");
	strDev += str;
	m_xmlDev.SetXmlPath(strDev);
	int num = m_xmlDev.GetDeviceNum();
	if(num > 0)
	{
		for (int i = 0; i < num; i++)
		{
			DeviceInfo info = m_xmlDev.GetDeviceByIndex(i);
			COleDateTime time;
			time.ParseDateTime(info.csEntryDate);
			int nYear = time.GetYear();
			UINT nPrice = _ttoi(info.csPrice);
			PriceMap::iterator iter = mapPrice.find(nYear);
			if(iter != mapPrice.end())
			{
				iter->second += nPrice;
			}
			else
			{
				mapPrice.insert(PriceMap::value_type(nYear, nPrice));
			}
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CUniversityDeviceManagerDlg::StatPriceEveryYearByUniv()
{
	if(m_nCurUnivCombIndex >= 0 && m_nCurUnivCombIndex < m_csUnivLst.size() && m_csAcaLst.size() > 0)
	{
		CString strModel = PathManager::GetCurrentModelPath();
		CString strUni = _T("data\\") + m_csUnivLst[m_nCurUnivCombIndex] + _T("\\");
		int nAllDev = 0;
		DWORD dwAllPrice = 0;
		BOOL bFlag = FALSE;
		m_mapUniPrices.clear();
		for (int i = 0; i < m_csAcaLst.size(); i++)
		{
			if(StatPriceEveryYearByAcademy(m_nCurUnivCombIndex, i, m_mapUniPrices))
				bFlag = TRUE;
		}
		return bFlag;
	}
	return FALSE;
}

void CUniversityDeviceManagerDlg::OnBnClickedBtnPriceChart()
{
	m_mapAcaPrices.clear();
	if(StatPriceEveryYearByAcademy(m_nCurUnivCombIndex, m_nCurAcaCombIndex, m_mapAcaPrices))
	{
		CDlgPriceChart dlg;
		dlg.SetPriceData(&m_mapAcaPrices);
		dlg.DoModal();
	}
	else
	{
		MessageBox(_T("没有选择学院或者学院下没有设备！"));
	}
}



void CUniversityDeviceManagerDlg::OnBnClickedBtnPriceChartUni()
{
	if(StatPriceEveryYearByUniv())
	{
		CDlgPriceChart dlg;
		dlg.SetPriceData(&m_mapUniPrices);
		dlg.DoModal();
	}
	else
	{
		MessageBox(_T("没有选择大学或者大学下没有学院设备！"));
	}
}


void CUniversityDeviceManagerDlg::OnBnClickedBtnLookUploadData()
{
	CString strfull = PathManager::GetCurrentModelPath() + m_curDevInfo.csUploadDataPath;
	HINSTANCE nRet = ShellExecute(this->m_hWnd, _T("open"), strfull,_T(""), _T(""), SW_SHOW );
	if((int)nRet <= 32)
	{
		CString err;
		err.Format(_T("查看资料失败！ 错误码=%d"), (int)nRet);
		MessageBox(err);
	}
	
}


void CUniversityDeviceManagerDlg::OnBnClickedBtnDevFilter()
{
	if((m_nCurUnivCombIndex < 0 && m_nCurUnivCombIndex >= m_csUnivLst.size())
		|| (m_nCurAcaCombIndex < 0 && m_nCurAcaCombIndex >= m_csAcaLst.size()))
	{
		MessageBox(_T("请选择一个学院！"));
		return;
	}

	CString strDev = PathManager::GetCurrentModelPath();
	strDev += _T("data\\");
	CString str = m_csUnivLst[m_nCurUnivCombIndex] + _T("\\") + m_csAcaLst[m_nCurAcaCombIndex] + _T("\\") + m_csAcaLst[m_nCurAcaCombIndex] +_T(".xml");
	strDev += str;
	CDlgDevFilter filter;
	filter.SetDevXmlPath(strDev);
	filter.DoModal();
}
