// UniversityAddDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UniversityDeviceEdit.h"
#include "UniversityAddDlg.h"
#include "afxdialogex.h"


// CUniversityAddDlg 对话框

IMPLEMENT_DYNAMIC(CUniversityAddDlg, CDialog)

CUniversityAddDlg::CUniversityAddDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUniversityAddDlg::IDD, pParent)
	, m_csUnivName(_T(""))
{

}

CUniversityAddDlg::~CUniversityAddDlg()
{
}

void CUniversityAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_UNIV_ADD, m_csUnivName);
}


BEGIN_MESSAGE_MAP(CUniversityAddDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CUniversityAddDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CUniversityAddDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CUniversityAddDlg 消息处理程序
BOOL CUniversityAddDlg::OnInitDialog()
{
	BOOL bRet = CDialog::OnInitDialog();
	UpdateData(FALSE);
	GetDlgItem(IDC_EDIT_UNIV_ADD)->SetFocus();
	return bRet;
}

void CUniversityAddDlg::OnBnClickedOk()
{
	UpdateData();
	CDialog::OnOK();
}


void CUniversityAddDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}

CString CUniversityAddDlg::GetUniversityName()
{
	return m_csUnivName;
}

void CUniversityAddDlg::SetUniversityName( CString csName )
{
	m_csUnivName = csName;
}


