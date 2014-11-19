#pragma once


// CAcademyAddDlg 对话框

class CAcademyAddDlg : public CDialog
{
	DECLARE_DYNAMIC(CAcademyAddDlg)

public:
	CAcademyAddDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAcademyAddDlg();

	virtual BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_DLG_ACA_ADA };

	CString GetAcademyName();
	void SetAcademyName(CString csName);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
private:
	CString m_csAcaName;
};
