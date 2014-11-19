
// UniversityDeviceEditDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UniversityDeviceEdit.h"
#include "UniversityDeviceEditDlg.h"
#include "afxdialogex.h"
#include "UniversityAddDlg.h"
#include "AcademyAddDlg.h"
#include "DevInfoEdtDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CUniversityDeviceEditDlg �Ի���




CUniversityDeviceEditDlg::CUniversityDeviceEditDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUniversityDeviceEditDlg::IDD, pParent)
	, m_lstUniv(Custom_Univ)
	, m_lstAcademy(Custom_Academy)
	, m_lstDevice(Custom_Device)
	, m_dwUnivCurIndex(-1)
	, m_dwAcaCurIndex(-1)
	, m_dwDevCurIndex(-1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUniversityDeviceEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_UNIV, m_lstUniv);
	DDX_Control(pDX, IDC_LIST_ACADEMY, m_lstAcademy);
	DDX_Control(pDX, IDC_LIST_DEVICE, m_lstDevice);
}

BEGIN_MESSAGE_MAP(CUniversityDeviceEditDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_ADD_UNIV, &CUniversityDeviceEditDlg::OnBnClickedBtnAddUniv)
	ON_BN_CLICKED(IDC_BTN_MODIFY_UNIV, &CUniversityDeviceEditDlg::OnBnClickedBtnModifyUniv)
	ON_BN_CLICKED(IDC_BTN_DELETE_UNIV, &CUniversityDeviceEditDlg::OnBnClickedBtnDeleteUniv)
	ON_BN_CLICKED(IDC_BTN_ADD_ACADEMY, &CUniversityDeviceEditDlg::OnBnClickedBtnAddAcademy)
	ON_BN_CLICKED(IDC_BTN_MODIFY_ACADEMY, &CUniversityDeviceEditDlg::OnBnClickedBtnModifyAcademy)
	ON_BN_CLICKED(IDC_BTN_DELETE_ACADEMY, &CUniversityDeviceEditDlg::OnBnClickedBtnDeleteAcademy)
	ON_BN_CLICKED(IDC_BTN_ADD_DEVICE, &CUniversityDeviceEditDlg::OnBnClickedBtnAddDevice)
	ON_BN_CLICKED(IDC_BTN_MODIFY_DEVICE, &CUniversityDeviceEditDlg::OnBnClickedBtnModifyDevice)
	ON_BN_CLICKED(IDC_BTN_DELETE_DEVICE, &CUniversityDeviceEditDlg::OnBnClickedBtnDeleteDevice)
	ON_BN_CLICKED(IDC_BTN_COPY, &CUniversityDeviceEditDlg::OnBnClickedBtnCopy)
	ON_BN_CLICKED(IDC_BTN_RECOVER, &CUniversityDeviceEditDlg::OnBnClickedBtnRecover)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CUniversityDeviceEditDlg::OnBnClickedBtnSave)
	ON_MESSAGE(WM_LIST_ITEM_CHANGED_MSG, &CUniversityDeviceEditDlg::OnCustomListItemChangedMsg)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DEVICE, &CUniversityDeviceEditDlg::OnNMDblclkListDevice)
END_MESSAGE_MAP()


// CUniversityDeviceEditDlg ��Ϣ�������

BOOL CUniversityDeviceEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	DWORD dwStyle = m_lstUniv.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT; 
	dwStyle |= LVS_EX_GRIDLINES;
	m_lstUniv.SetExtendedStyle(dwStyle);
	m_lstUniv.SetColumNum(2);
	m_lstUniv.SetFixWidth();
	m_lstUniv.InsertListColumn(0,_T("��ѧ���"),LVCFMT_CENTER, 80);
	CRect rc;
	m_lstUniv.GetWindowRect(rc);
	m_lstUniv.InsertListColumn(1,_T("��ѧ����"),LVCFMT_CENTER, rc.Width()-80);

	CString strUniv = PathManager::GetCurrentModelPath();
	strUniv += _T("data\\university.xml");
	m_xmlUniversity.SetXmlPath(strUniv);

	RefreshUniversityList();

	dwStyle |= LVS_EX_FULLROWSELECT; 
	dwStyle |= LVS_EX_GRIDLINES;
	m_lstAcademy.SetExtendedStyle(dwStyle);
	m_lstAcademy.SetColumNum(2);
	m_lstAcademy.SetFixWidth();
	m_lstAcademy.InsertListColumn(0,_T("ѧԺ���"),LVCFMT_CENTER, 100);
	m_lstAcademy.GetWindowRect(rc);
	m_lstAcademy.InsertListColumn(1,_T("ѧԺ����"),LVCFMT_CENTER, rc.Width()-100);




	dwStyle |= LVS_EX_FULLROWSELECT; 
	dwStyle |= LVS_EX_GRIDLINES;
	m_lstDevice.SetExtendedStyle(dwStyle);
	m_lstDevice.SetColumNum(16);
	m_lstDevice.InsertListColumn(0,_T("�豸���"),LVCFMT_CENTER, 80);
	m_lstDevice.InsertListColumn(1,_T("�豸����"),LVCFMT_CENTER, 80);
	m_lstDevice.InsertListColumn(2,_T("�豸�ͺ�"),LVCFMT_CENTER, 80);
	m_lstDevice.InsertListColumn(3,_T("�豸����"),LVCFMT_CENTER, 110);
	m_lstDevice.InsertListColumn(4,_T("���ۣ�Ԫ��"),LVCFMT_CENTER, 80);
	m_lstDevice.InsertListColumn(5,_T("��������"),LVCFMT_CENTER, 80);
	m_lstDevice.InsertListColumn(6,_T("������"),LVCFMT_CENTER, 80);
	m_lstDevice.InsertListColumn(7,_T("�豸���ڵ�"),LVCFMT_CENTER, 110);
	m_lstDevice.InsertListColumn(8,_T("����"),LVCFMT_CENTER, 110);
	m_lstDevice.InsertListColumn(9,_T("�豸����ָ��"),LVCFMT_CENTER, 110);
	m_lstDevice.InsertListColumn(10,_T("��������"),LVCFMT_CENTER, 80);
	m_lstDevice.InsertListColumn(11,_T("���յص�"),LVCFMT_CENTER, 80);
	m_lstDevice.InsertListColumn(12,_T("���ϵص�"),LVCFMT_CENTER, 80);
	m_lstDevice.InsertListColumn(13,_T("��ϵ�绰"),LVCFMT_CENTER, 110);
	m_lstDevice.InsertListColumn(14,_T("�����ʼ�"),LVCFMT_CENTER, 110);
	m_lstDevice.InsertListColumn(15,_T("��ע"),LVCFMT_CENTER, 110);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


BOOL CUniversityDeviceEditDlg::PreTranslateMessage( MSG* pMsg )
{
	return CDialog::PreTranslateMessage(pMsg);
}

void CUniversityDeviceEditDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUniversityDeviceEditDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CUniversityDeviceEditDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUniversityDeviceEditDlg::OnBnClickedBtnAddUniv()
{
	CUniversityAddDlg dlg;
	if(IDOK == dlg.DoModal())
	{
		CString strSpecial = _T("/\\?*|\"<>:");  
		USES_CONVERSION;
		CString strUni = dlg.GetUniversityName();
		if (strpbrk(W2A(strUni),W2A(strSpecial)) != NULL)
		{
			MessageBox(_T("���ܰ��������κ��ַ���/n\\/:*?\"<>|"));
			return ;
		}
		if(strUni.GetAt(0) >= _T('0') && strUni.GetAt(0) <= _T('9'))
		{
			MessageBox(_T("���ƿ�ͷ����Ϊ���֣�"));
			return ;
		}

		if(m_xmlUniversity.InsertUniversityName(strUni))
			RefreshUniversityList();
	}
}


void CUniversityDeviceEditDlg::OnBnClickedBtnModifyUniv()
{
	if(m_dwUnivCurIndex >= 0 && m_dwUnivCurIndex < m_lstUniv.GetItemCount())
	{
		CUniversityAddDlg dlg;
		dlg.SetUniversityName(m_xmlUniversity.GetUniversityNameByIndex(m_dwUnivCurIndex));
		if(IDOK == dlg.DoModal())
		{
			CString strSpecial = _T("/\\?*|\"<>:");  
			USES_CONVERSION;
			CString strUni = dlg.GetUniversityName();
			if (strpbrk(W2A(strUni),W2A(strSpecial)) != NULL)
			{
				MessageBox(_T("���ܰ��������κ��ַ���/n\\/:*?\"<>|"));
				return ;
			}
			if(strUni.GetAt(0) >= _T('0') && strUni.GetAt(0) <= _T('9'))
			{
				MessageBox(_T("���ƿ�ͷ����Ϊ���֣�"));
				return ;
			}
			m_xmlUniversity.ModifyUniversityName(m_dwUnivCurIndex, strUni);
			RefreshUniversityList();
		}
	}
	else
	{
		MessageBox(_T("��ѡ��һ�"));
	}
	
}


void CUniversityDeviceEditDlg::OnBnClickedBtnDeleteUniv()
{
	if(m_dwUnivCurIndex >= 0 && m_dwUnivCurIndex < m_lstUniv.GetItemCount())
	{
		if(IDYES == MessageBox(_T("ɾ���󣬶�Ӧ��ѧԺ���豸��Ϣ���ᱻɾ�����޷��ָ���ȷ��ɾ����"), _T("����"), MB_YESNO))
		{

			CString str = PathManager::GetCurrentModelPath();
			str +=_T("data\\");
			str += m_xmlUniversity.GetUniversityNameByIndex(m_dwUnivCurIndex);
			m_xmlUniversity.DeleteUniversityName(m_dwUnivCurIndex);
			PathManager::DeleteDirectory(str);
			RefreshUniversityList();
			m_dwUnivCurIndex = -1;
			m_lstAcademy.DeleteAllItems();
			m_lstDevice.DeleteAllItems();
		}
	}
	else
	{
		MessageBox(_T("��ѡ��һ�"));
	}
}


void CUniversityDeviceEditDlg::OnBnClickedBtnAddAcademy()
{
	if(m_dwUnivCurIndex >= 0 && m_dwUnivCurIndex < m_lstUniv.GetItemCount())
	{
		CAcademyAddDlg dlg;
		if(IDOK == dlg.DoModal())
		{
			CString strSpecial = _T("/\\?*|\"<>:");  
			USES_CONVERSION;
			CString strAca = dlg.GetAcademyName();
			if (strpbrk(W2A(strAca),W2A(strSpecial)) != NULL)
			{
				MessageBox(_T("���ܰ��������κ��ַ���/n\\/:*?\"<>|"));
				return ;
			}
			if(strAca.GetAt(0) >= _T('0') && strAca.GetAt(0) <= _T('9'))
			{
				MessageBox(_T("���ƿ�ͷ����Ϊ���֣�"));
				return ;
			}
			if(m_xmlAcademy.InsertAcademyName(strAca))
				RefreshAcademyList();
		}
	}
	else
	{
		MessageBox(_T("δѡ���ѧ��"));
	}
}


void CUniversityDeviceEditDlg::OnBnClickedBtnModifyAcademy()
{
	if(m_dwAcaCurIndex >= 0 && m_dwAcaCurIndex < m_lstAcademy.GetItemCount())
	{
		CAcademyAddDlg dlg;
		dlg.SetAcademyName(m_xmlAcademy.GetAcademyNameByIndex(m_dwAcaCurIndex));
		if(IDOK == dlg.DoModal())
		{
			CString strSpecial = _T("/\\?*|\"<>:");  
			USES_CONVERSION;
			CString strAca = dlg.GetAcademyName();
			if (strpbrk(W2A(strAca),W2A(strSpecial)) != NULL)
			{
				MessageBox(_T("���ܰ��������κ��ַ���/n\\/:*?\"<>|"));
				return ;
			}
			if(strAca.GetAt(0) >= _T('0') && strAca.GetAt(0) <= _T('9'))
			{
				MessageBox(_T("���ƿ�ͷ����Ϊ���֣�"));
				return ;
			}
			m_xmlAcademy.ModifyAcademyName(m_dwAcaCurIndex, strAca);
			RefreshAcademyList();
		}
	}
	else
	{
		MessageBox(_T("��ѡ��һ�"));
	}
}


void CUniversityDeviceEditDlg::OnBnClickedBtnDeleteAcademy()
{
	if(m_dwAcaCurIndex >= 0 && m_dwAcaCurIndex < m_lstAcademy.GetItemCount())
	{
		if(IDYES == MessageBox(_T("ɾ���󣬻�Ѷ�Ӧ���豸��ϢҲɾ�����޷��ָ���ȷ��ɾ����"), _T("����"), MB_YESNO))
		{
			CString str = PathManager::GetCurrentModelPath();
			str +=_T("data\\");
			str += m_xmlUniversity.GetUniversityNameByIndex(m_dwUnivCurIndex);
			str += _T("\\");
			str += m_xmlAcademy.GetAcademyNameByIndex(m_dwAcaCurIndex);
			PathManager::DeleteDirectory(str);
			m_xmlAcademy.DeleteAcademyName(m_dwAcaCurIndex);
			RefreshAcademyList();
			m_dwAcaCurIndex = -1;
			m_lstDevice.DeleteAllItems();
		}
	}
	else
	{
		MessageBox(_T("��ѡ��һ�"));
	}
}


void CUniversityDeviceEditDlg::OnBnClickedBtnAddDevice()
{
	if(m_dwAcaCurIndex >= 0 && m_dwAcaCurIndex < m_lstAcademy.GetItemCount())
	{
		CDevInfoEdtDlg dlg;
		CString strUn = m_xmlUniversity.GetUniversityNameByIndex(m_dwUnivCurIndex);
		CString strAca = m_xmlAcademy.GetAcademyNameByIndex(m_dwAcaCurIndex);
		CString strModel = _T("data\\");
		strModel += strUn;
		strModel += _T("\\");
		strModel += strAca;
		strModel += _T("\\");
		dlg.SetCurrentDevPath(strModel);
		if(IDOK == dlg.DoModal())
		{
			DeviceInfo info = dlg.GetDeviceInfo();
			m_xmlDevices.InsertNewDevice(info);
			InsertDeviceToList(info);
		}
	}
	else
	{
		MessageBox(_T("��ѡ��һ��ѧԺ��"));
	}
}


void CUniversityDeviceEditDlg::OnBnClickedBtnModifyDevice()
{
	if(m_dwDevCurIndex >= 0 && m_dwDevCurIndex < m_lstDevice.GetItemCount())
	{
		DeviceInfo info = m_xmlDevices.GetDeviceByIndex(m_dwDevCurIndex);
		CDevInfoEdtDlg dlg;
		dlg.SetAddOrModifyType(FALSE);
		dlg.SetDeviceInfo(info);

		CString strUn = m_xmlUniversity.GetUniversityNameByIndex(m_dwUnivCurIndex);
		CString strAca = m_xmlAcademy.GetAcademyNameByIndex(m_dwAcaCurIndex);
		CString strModel = _T("data\\");
		strModel += strUn;
		strModel += _T("\\");
		strModel += strAca;
		strModel += _T("\\");
		dlg.SetCurrentDevPath(strModel);

		if(IDOK == dlg.DoModal())
		{
			info = dlg.GetDeviceInfo();
			m_xmlDevices.ModifyDevice(info, m_dwDevCurIndex);
			ModifyDeviceFromList(info);
		}
	}
	else
	{
		MessageBox(_T("��ѡ��һ���豸��"));
	}
}


void CUniversityDeviceEditDlg::OnBnClickedBtnDeleteDevice()
{
	if(m_dwDevCurIndex >= 0 && m_dwDevCurIndex < m_lstDevice.GetItemCount())
	{
		if(IDYES == MessageBox(_T("ɾ�����޷��ָ���ȷ��ɾ����"), _T("����"), MB_YESNO))
		{
			m_xmlDevices.DeleteDevice(m_dwDevCurIndex);
			DeleteDeviceFromList();
		}
	}
	else
	{
		MessageBox(_T("��ѡ��һ���豸��"));
	}
	
}


void CUniversityDeviceEditDlg::OnBnClickedBtnCopy()
{
	CString strModel = PathManager::GetCurrentModelPath();
	CString csMain = strModel + _T("data");
	CString csNew = strModel + _T("data_copy");
	PathManager::DeleteDirectory(csNew);
	Sleep(500);
	if(PathManager::CopyDir(csMain, csNew))
		MessageBox(_T("���ݳɹ���"));
	else
		MessageBox(_T("����ʧ�ܣ�"));
}


void CUniversityDeviceEditDlg::OnBnClickedBtnRecover()
{
	if(IDYES == MessageBox(_T("����ָ��������ݣ���ǰ���ݻᶪʧ��"), _T("����"), MB_YESNO))
	{
		CString strModel = PathManager::GetCurrentModelPath();
		CString csMain = strModel + _T("data");
		CString csNew = strModel + _T("data_copy");
		PathManager::DeleteDirectory(csMain);
		Sleep(500);
		if(PathManager::CopyDir(csNew, csMain))
		{
			MessageBox(_T("�ָ��ɹ���"));
			g_bRestart = true;
			SendMessage(WM_CLOSE, 0,0);
		}
		else
			MessageBox(_T("�ָ�ʧ�ܣ�"));

		
	}
	
}


void CUniversityDeviceEditDlg::OnBnClickedBtnSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CUniversityDeviceEditDlg::RefreshUniversityList()
{
	CSNameVec unLst;
	m_xmlUniversity.GetUniversityList(unLst);
	int size = unLst.size();
	int nRow = 0;
	if(m_lstUniv.GetItemCount() > 0)
	{
		m_lstUniv.DeleteAllItems();
	}
	for (int i = size - 1; i >= 0; i--)
	{
		nRow = m_lstUniv.InsertItem(0, _T(""));
		CString str;
		str.Format(_T("%d"), i+1);
		m_lstUniv.SetItemText(nRow,0,str);
		m_lstUniv.SetItemText(nRow,1,unLst[i]);

	}
}

void CUniversityDeviceEditDlg::RefreshAcademyList()
{
	CString str = m_xmlUniversity.GetUniversityNameByIndex(m_dwUnivCurIndex);
	CString strModel = PathManager::GetCurrentModelPath();
	strModel += _T("data\\");
	strModel += str;
	if(!PathFileExists(strModel))
	{
		CreateDirectory(strModel,NULL);
	}
	CString strPath = strModel + _T("\\") + str + _T(".xml");
	m_xmlAcademy.SetXmlPath(strPath);
	CSNameVec acaLst;
	m_xmlAcademy.GetAcademyList(acaLst);

	int size = acaLst.size();
	int nRow = 0;
	if(m_lstAcademy.GetItemCount() > 0)
	{
		m_lstAcademy.DeleteAllItems();
	}
	for (int i = size - 1; i >= 0; i--)
	{
		nRow = m_lstAcademy.InsertItem(0, _T(""));
		CString str;
		str.Format(_T("%d"), i+1);
		m_lstAcademy.SetItemText(nRow,0,str);
		m_lstAcademy.SetItemText(nRow,1,acaLst[i]);

	}

}

void CUniversityDeviceEditDlg::RefreshDeviceList()
{
	CString strUn = m_xmlUniversity.GetUniversityNameByIndex(m_dwUnivCurIndex);
	CString strAca = m_xmlAcademy.GetAcademyNameByIndex(m_dwAcaCurIndex);
	CString strModel = PathManager::GetCurrentModelPath();
	strModel += _T("data\\");
	strModel += strUn;
	strModel += _T("\\");
	strModel += strAca;
	if(!PathFileExists(strModel))
	{
		CreateDirectory(strModel,NULL);
	}
	CString strPath = strModel + _T("\\") + strAca + _T(".xml");
	m_xmlDevices.SetXmlPath(strPath);

	if(m_lstDevice.GetItemCount() > 0)
	{
		m_lstDevice.DeleteAllItems();
	}

	CSNameVec devLst;
	int num = m_xmlDevices.GetDeviceNum();
	int nRow = 0;
	for (int i = num-1; i >= 0; i--)
	{
		DeviceInfo info;
		info = m_xmlDevices.GetDeviceByIndex(i);
		nRow = m_lstDevice.InsertItem(0, _T(""));
		m_lstDevice.SetItemText(nRow,0,info.csDevId);
		m_lstDevice.SetItemText(nRow,1,info.csDevName);
		m_lstDevice.SetItemText(nRow,2,info.csDevType);
		m_lstDevice.SetItemText(nRow,3,info.csSortName);
		m_lstDevice.SetItemText(nRow,4,info.csPrice);
		m_lstDevice.SetItemText(nRow,5,info.csEntryDate);
		m_lstDevice.SetItemText(nRow,6,info.csKeeper);
		m_lstDevice.SetItemText(nRow,7,info.csDevNowPlace);
		m_lstDevice.SetItemText(nRow,8,info.csOriginPlace);
		m_lstDevice.SetItemText(nRow,9,info.csTechNorm);
		m_lstDevice.SetItemText(nRow,10,info.csFactoryDate);
		m_lstDevice.SetItemText(nRow,11,info.csAccptanceDate);
		m_lstDevice.SetItemText(nRow,12,info.csScrapDate);
		m_lstDevice.SetItemText(nRow,13,info.csPhone);
		m_lstDevice.SetItemText(nRow,14,info.csEmail);
		m_lstDevice.SetItemText(nRow,15,info.csRemark);
	}
}

void CUniversityDeviceEditDlg::InsertDeviceToList( DeviceInfo& info )
{
	int nRow = m_lstDevice.GetItemCount();
	m_lstDevice.InsertItem(nRow, _T(""));
	m_lstDevice.SetItemText(nRow,0,info.csDevId);
	m_lstDevice.SetItemText(nRow,1,info.csDevName);
	m_lstDevice.SetItemText(nRow,2,info.csDevType);
	m_lstDevice.SetItemText(nRow,3,info.csSortName);
	m_lstDevice.SetItemText(nRow,4,info.csPrice);
	m_lstDevice.SetItemText(nRow,5,info.csEntryDate);
	m_lstDevice.SetItemText(nRow,6,info.csKeeper);
	m_lstDevice.SetItemText(nRow,7,info.csDevNowPlace);
	m_lstDevice.SetItemText(nRow,8,info.csOriginPlace);
	m_lstDevice.SetItemText(nRow,9,info.csTechNorm);
	m_lstDevice.SetItemText(nRow,10,info.csFactoryDate);
	m_lstDevice.SetItemText(nRow,11,info.csAccptanceDate);
	m_lstDevice.SetItemText(nRow,12,info.csScrapDate);
	m_lstDevice.SetItemText(nRow,13,info.csPhone);
	m_lstDevice.SetItemText(nRow,14,info.csEmail);
	m_lstDevice.SetItemText(nRow,15,info.csRemark);
	
}

void CUniversityDeviceEditDlg::ModifyDeviceFromList( DeviceInfo& info )
{
	if(m_dwDevCurIndex>= 0 && m_dwDevCurIndex < m_lstDevice.GetItemCount())
	{
		int nRow = m_dwDevCurIndex;
		m_lstDevice.SetItemText(nRow,0,info.csDevId);
		m_lstDevice.SetItemText(nRow,1,info.csDevName);
		m_lstDevice.SetItemText(nRow,2,info.csDevType);
		m_lstDevice.SetItemText(nRow,3,info.csSortName);
		m_lstDevice.SetItemText(nRow,4,info.csPrice);
		m_lstDevice.SetItemText(nRow,5,info.csEntryDate);
		m_lstDevice.SetItemText(nRow,6,info.csKeeper);
		m_lstDevice.SetItemText(nRow,7,info.csDevNowPlace);
		m_lstDevice.SetItemText(nRow,8,info.csOriginPlace);
		m_lstDevice.SetItemText(nRow,9,info.csTechNorm);
		m_lstDevice.SetItemText(nRow,10,info.csFactoryDate);
		m_lstDevice.SetItemText(nRow,11,info.csAccptanceDate);
		m_lstDevice.SetItemText(nRow,12,info.csScrapDate);
		m_lstDevice.SetItemText(nRow,13,info.csPhone);
		m_lstDevice.SetItemText(nRow,14,info.csEmail);
		m_lstDevice.SetItemText(nRow,15,info.csRemark);
	}
}

void CUniversityDeviceEditDlg::DeleteDeviceFromList()
{
	if(m_dwDevCurIndex>= 0 && m_dwDevCurIndex < m_lstDevice.GetItemCount())
	{
		m_lstDevice.DeleteItem(m_dwDevCurIndex);
		m_dwDevCurIndex = -1;
	}
}

LRESULT CUniversityDeviceEditDlg::OnCustomListItemChangedMsg( WPARAM wParam, LPARAM lParam )
{
	emCustomList emLst = static_cast<emCustomList>(wParam);
	switch (emLst)
	{
	case Custom_Univ:
		CtrlUnivItemChanged(lParam);
		break;
	case Custom_Academy:
		CtrlAcademyItemChanged(lParam);
		break;
	case Custom_Device:
		CtrlDeviceItemChanged(lParam);
		break;
	}
	return S_OK;
}

void CUniversityDeviceEditDlg::CtrlUnivItemChanged( int index )
{
	m_dwUnivCurIndex = index;
	RefreshAcademyList();
	m_lstDevice.DeleteAllItems();
	
}

void CUniversityDeviceEditDlg::CtrlAcademyItemChanged( int index )
{
	m_dwAcaCurIndex = index;
	RefreshDeviceList();
}

void CUniversityDeviceEditDlg::CtrlDeviceItemChanged( int index )
{
	m_dwDevCurIndex = index;
}

void CUniversityDeviceEditDlg::OnNMDblclkListDevice(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	m_dwDevCurIndex = pNMItemActivate->iItem;
	OnBnClickedBtnModifyDevice();
	*pResult = 0;
}
