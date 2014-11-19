#pragma once
#include "afxdtctl.h"
#include "afxwin.h"
// CDevInfoEdtDlg 对话框

class CDevInfoEdtDlg : public CDialog
{
	DECLARE_DYNAMIC(CDevInfoEdtDlg)

public:
	CDevInfoEdtDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDevInfoEdtDlg();
	virtual BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_DLG_DEV_INFO };
	void SetAddOrModifyType(BOOL bFlag = TRUE);
	void SetCurrentDevPath(CString csPath);
	DeviceInfo GetDeviceInfo();
	void SetDeviceInfo(DeviceInfo info);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedBtnDevImgBrowse();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnDevImgBrowse2();
	afx_msg void OnBnClickedBtnDevImgBrowse3();
	afx_msg void OnBnClickedBtnUploadData();

private:
	UINT m_nDevID;
	CString m_csSortName;
	UINT m_nPrice;
	CDateTimeCtrl m_dtcDate;
	CString m_csKeeper;
	CString m_csDevName;
	CString m_csImgPath;
	CString m_csImgPath2;
	CString m_csImgPath3;
	CString m_csDate;
	CString m_csFactoryDate;
	CString m_csAccptanceDate;
	CString m_csScrapDate;
	CBmpTextStatic m_stcImgShow;
	CString m_csDevPath;
	BOOL m_bDlgFlag;
	
	CBmpTextStatic m_stcImgShow2;
	CBmpTextStatic m_stcImgShow3;
	CString m_csDevType;
	CString m_csOriginPlace;
	CString m_csNowPlace;
	CString m_csDevTechNorm;
	CString m_csPhone;
	CString m_csEmail;
	CDateTimeCtrl m_dtcFactoryDate;
	CDateTimeCtrl m_dtcAccptanceDate;
	CDateTimeCtrl m_dtcScrapDate;
	CString m_csRemark;
	CString m_csUploadDataPath;
public:
	afx_msg void OnBnClickedBtnDeleteData();
};
