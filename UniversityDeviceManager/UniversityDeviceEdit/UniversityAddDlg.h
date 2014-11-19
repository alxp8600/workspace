#pragma once
#include "afxwin.h"


// CUniversityAddDlg �Ի���

class CUniversityAddDlg : public CDialog
{
	DECLARE_DYNAMIC(CUniversityAddDlg)

public:
	CUniversityAddDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUniversityAddDlg();

	virtual BOOL OnInitDialog();

// �Ի�������
	enum { IDD = IDD_DLG_UNIV_ADD };

	CString GetUniversityName();
	void SetUniversityName(CString csName);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
private:
	CString m_csUnivName;
};
