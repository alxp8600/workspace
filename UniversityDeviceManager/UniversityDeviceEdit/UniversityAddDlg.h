#pragma once
#include "afxwin.h"


// CUniversityAddDlg 对话框

class CUniversityAddDlg : public CDialog
{
	DECLARE_DYNAMIC(CUniversityAddDlg)

public:
	CUniversityAddDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUniversityAddDlg();

	virtual BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_DLG_UNIV_ADD };

	CString GetUniversityName();
	void SetUniversityName(CString csName);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
private:
	CString m_csUnivName;
};
