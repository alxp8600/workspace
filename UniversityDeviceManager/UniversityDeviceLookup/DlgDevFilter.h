#pragma once

struct DevFilterOpt
{

};
// CDlgDevFilter �Ի���

class CDlgDevFilter : public CDialog
{
	DECLARE_DYNAMIC(CDlgDevFilter)

public:
	CDlgDevFilter(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgDevFilter();

	virtual BOOL OnInitDialog();
// �Ի�������
	enum { IDD = IDD_DLG_DEV_FILTER };

	void SetDevXmlPath(CString csPath);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnBnClickedBtnFilterAction();
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bCheckDevName;
	BOOL m_bCheckPrices;
	BOOL m_bCheckSort;
	BOOL m_bCheckKeeper;
	CString m_csEditDevName;
	CString m_csEditDevSort;
	CString m_csEditDevKeeper;
	CCustomListCtrl m_devList;
	CString m_csXmlPath;
	DeviceXmlMnger m_xmlDev;
	UINT m_unDevLowPrice;
	UINT m_unDevHigPrice;
};
