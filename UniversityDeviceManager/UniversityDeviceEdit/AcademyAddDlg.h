#pragma once


// CAcademyAddDlg �Ի���

class CAcademyAddDlg : public CDialog
{
	DECLARE_DYNAMIC(CAcademyAddDlg)

public:
	CAcademyAddDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAcademyAddDlg();

	virtual BOOL OnInitDialog();

// �Ի�������
	enum { IDD = IDD_DLG_ACA_ADA };

	CString GetAcademyName();
	void SetAcademyName(CString csName);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
private:
	CString m_csAcaName;
};
