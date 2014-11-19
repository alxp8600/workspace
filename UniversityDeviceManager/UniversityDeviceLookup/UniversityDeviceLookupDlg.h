
// UniversityDeviceManagerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CUniversityDeviceManagerDlg �Ի���
class CUniversityDeviceManagerDlg : public CDialogEx
{
// ����
public:
	CUniversityDeviceManagerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_UNIVERSITYDEVICEMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedBtnLookup();
	afx_msg void OnCbnSelchangeComboUniversity();
	afx_msg void OnCbnSelchangeComboAcademy();
	afx_msg LRESULT OnDevItemChangedMsg(WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnPriceChart();
	afx_msg void OnBnClickedBtnPriceChartUni();
	afx_msg void OnBnClickedBtnLookUploadData();
	afx_msg void OnBnClickedBtnDevFilter();
	DECLARE_MESSAGE_MAP()

private:
	void RefreshUnivComb();
	void RefreshAcademyComb();
	void RefreshDevList();
	void CtrlDeviceItemChanged( int index );
	void ShowDevInfo();
	BOOL StatPriceEveryYearByAcademy(int nUniIndex, int nAcaIndex, PriceMap& mapPrice);
	BOOL StatPriceEveryYearByUniv();
private:
	DWORD m_dwDevID;
	CCustomListCtrl m_lstDev;
	CComboBox m_combUniv;
	CComboBox m_combAcademy;

	UniversityXmlMnger m_xmlUniv;
	CSNameVec m_csUnivLst;

	AcademyXmlMnger m_xmlAcademy;
	CSNameVec m_csAcaLst;

	DeviceXmlMnger m_xmlDev;

	int m_nCurUnivCombIndex;
	int m_nCurAcaCombIndex;

	DeviceInfo m_curDevInfo;
	CBmpTextStatic m_stcImgShow;
	CBmpTextStatic m_stcImgShow2;
	CBmpTextStatic m_stcImgShow3;
	CString m_csCurUnivName;
	CString m_csCurAcaName;
	COLORREF m_bkColor;

	PriceMap m_mapAcaPrices;
	PriceMap m_mapUniPrices;
};
