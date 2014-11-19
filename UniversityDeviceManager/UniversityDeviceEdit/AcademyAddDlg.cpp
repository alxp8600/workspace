// AcademyAddDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UniversityDeviceEdit.h"
#include "AcademyAddDlg.h"
#include "afxdialogex.h"


// CAcademyAddDlg �Ի���

IMPLEMENT_DYNAMIC(CAcademyAddDlg, CDialog)

CAcademyAddDlg::CAcademyAddDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAcademyAddDlg::IDD, pParent)
	, m_csAcaName(_T(""))
{

}

CAcademyAddDlg::~CAcademyAddDlg()
{
}

void CAcademyAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ACA_ADD, m_csAcaName);
}


BEGIN_MESSAGE_MAP(CAcademyAddDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAcademyAddDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAcademyAddDlg ��Ϣ�������

BOOL CAcademyAddDlg::OnInitDialog()
{
	bool bRet = CDialog::OnInitDialog();
	UpdateData(FALSE);
	return bRet;
}

void CAcademyAddDlg::OnBnClickedOk()
{
	UpdateData();
	CDialog::OnOK();
}

CString CAcademyAddDlg::GetAcademyName()
{
	return m_csAcaName;
}

void CAcademyAddDlg::SetAcademyName( CString csName )
{
	m_csAcaName = csName;
}

