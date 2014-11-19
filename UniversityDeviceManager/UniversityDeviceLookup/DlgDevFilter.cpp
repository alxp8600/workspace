// DlgDevFilter.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UniversityDeviceLookup.h"
#include "DlgDevFilter.h"
#include "afxdialogex.h"


// CDlgDevFilter �Ի���

IMPLEMENT_DYNAMIC(CDlgDevFilter, CDialog)

CDlgDevFilter::CDlgDevFilter(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDevFilter::IDD, pParent)
	, m_bCheckDevName(FALSE)
	, m_bCheckPrices(FALSE)
	, m_bCheckSort(FALSE)
	, m_bCheckKeeper(FALSE)
	, m_csEditDevName(_T(""))
	, m_csEditDevSort(_T(""))
	, m_csEditDevKeeper(_T(""))
	, m_devList(Custom_Device)
	, m_unDevLowPrice(0)
	, m_unDevHigPrice(0)
{

}

CDlgDevFilter::~CDlgDevFilter()
{
}

void CDlgDevFilter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_DEV_NAME, m_bCheckDevName);
	DDX_Check(pDX, IDC_CHECK_DEV_PRICE, m_bCheckPrices);
	DDX_Check(pDX, IDC_CHECK_DEV_SORT, m_bCheckSort);
	DDX_Check(pDX, IDC_CHECK_DEV_KEEPER, m_bCheckKeeper);
	DDX_Text(pDX, IDC_EDIT_DEV_NAME_FILTER, m_csEditDevName);
	DDX_Text(pDX, IDC_EDIT_DEV_SORT_FILTER, m_csEditDevSort);
	DDX_Text(pDX, IDC_EDIT_DEV_KEEPER_FILTER, m_csEditDevKeeper);
	DDX_Control(pDX, IDC_LIST_DEV_FLITER, m_devList);
	DDX_Text(pDX, IDC_EDIT_DEV_PRICE_LOW_FILTER, m_unDevLowPrice);
	DDX_Text(pDX, IDC_EDIT_DEV_PRICE_HIG_FILTER, m_unDevHigPrice);
}

BOOL CDlgDevFilter::OnInitDialog()
{
	BOOL bRet = CDialog::OnInitDialog();
	DWORD dwStyle = m_devList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT; 
	dwStyle |= LVS_EX_GRIDLINES;
	m_devList.SetExtendedStyle(dwStyle);
	m_devList.SetColumNum(16);
	m_devList.InsertListColumn(0,_T("�豸���"),LVCFMT_CENTER, 80);
	m_devList.InsertListColumn(1,_T("�豸����"),LVCFMT_CENTER, 80);
	m_devList.InsertListColumn(2,_T("�豸�ͺ�"),LVCFMT_CENTER, 80);
	m_devList.InsertListColumn(3,_T("�豸����"),LVCFMT_CENTER, 110);
	m_devList.InsertListColumn(4,_T("���ۣ�Ԫ��"),LVCFMT_CENTER, 80);
	m_devList.InsertListColumn(5,_T("��������"),LVCFMT_CENTER, 80);
	m_devList.InsertListColumn(6,_T("������"),LVCFMT_CENTER, 80);
	m_devList.InsertListColumn(7,_T("�豸���ڵ�"),LVCFMT_CENTER, 110);
	m_devList.InsertListColumn(8,_T("����"),LVCFMT_CENTER, 110);
	m_devList.InsertListColumn(9,_T("�豸����ָ��"),LVCFMT_CENTER, 110);
	m_devList.InsertListColumn(10,_T("��������"),LVCFMT_CENTER, 80);
	m_devList.InsertListColumn(11,_T("���յص�"),LVCFMT_CENTER, 80);
	m_devList.InsertListColumn(12,_T("���ϵص�"),LVCFMT_CENTER, 80);
	m_devList.InsertListColumn(13,_T("��ϵ�绰"),LVCFMT_CENTER, 110);
	m_devList.InsertListColumn(14,_T("�����ʼ�"),LVCFMT_CENTER, 110);
	m_devList.InsertListColumn(15,_T("��ע"),LVCFMT_CENTER, 110);
	return bRet;
}

BEGIN_MESSAGE_MAP(CDlgDevFilter, CDialog)
	ON_BN_CLICKED(IDC_BTN_FILTER_ACTION, &CDlgDevFilter::OnBnClickedBtnFilterAction)
END_MESSAGE_MAP()


// CDlgDevFilter ��Ϣ�������


void CDlgDevFilter::OnBnClickedBtnFilterAction()
{
	m_devList.DeleteAllItems();
	UpdateData();
	m_xmlDev.SetXmlPath(m_csXmlPath);
	int nDevNum = m_xmlDev.GetDeviceNum();
	for (int i = nDevNum-1; i >= 0; i--)
	{
		DeviceInfo info = m_xmlDev.GetDeviceByIndex(i);
		if(m_bCheckDevName && info.csDevName != m_csEditDevName)
			continue;
		if(m_bCheckSort && info.csSortName != m_csEditDevSort)
			continue;
		if(m_bCheckKeeper && info.csKeeper != m_csEditDevKeeper)
			continue;
		UINT nPrice = _ttoi(info.csPrice);
		if(m_bCheckPrices && (nPrice > m_unDevHigPrice || nPrice < m_unDevLowPrice))
			continue;
		int nRow = m_devList.InsertItem(0, _T(""));
		m_devList.SetItemText(nRow,0,info.csDevId);
		m_devList.SetItemText(nRow,1,info.csDevName);
		m_devList.SetItemText(nRow,2,info.csDevType);
		m_devList.SetItemText(nRow,3,info.csSortName);
		m_devList.SetItemText(nRow,4,info.csPrice);
		m_devList.SetItemText(nRow,5,info.csEntryDate);
		m_devList.SetItemText(nRow,6,info.csKeeper);
		m_devList.SetItemText(nRow,7,info.csDevNowPlace);
		m_devList.SetItemText(nRow,8,info.csOriginPlace);
		m_devList.SetItemText(nRow,9,info.csTechNorm);
		m_devList.SetItemText(nRow,10,info.csFactoryDate);
		m_devList.SetItemText(nRow,11,info.csAccptanceDate);
		m_devList.SetItemText(nRow,12,info.csScrapDate);
		m_devList.SetItemText(nRow,13,info.csPhone);
		m_devList.SetItemText(nRow,14,info.csEmail);
		m_devList.SetItemText(nRow,15,info.csRemark);

	}

}

void CDlgDevFilter::SetDevXmlPath( CString csPath )
{
	m_csXmlPath = csPath;
}