
// UniversityDeviceEditDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"

// CUniversityDeviceEditDlg �Ի���
class CUniversityDeviceEditDlg : public CDialogEx
{
// ����
public:
	CUniversityDeviceEditDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_UNIVERSITYDEVICEEDIT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	virtual BOOL  PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedBtnAddUniv();
	afx_msg void OnBnClickedBtnModifyUniv();
	afx_msg void OnBnClickedBtnDeleteUniv();
	afx_msg void OnBnClickedBtnAddAcademy();
	afx_msg void OnBnClickedBtnModifyAcademy();
	afx_msg void OnBnClickedBtnDeleteAcademy();
	afx_msg void OnBnClickedBtnAddDevice();
	afx_msg void OnBnClickedBtnModifyDevice();
	afx_msg void OnBnClickedBtnDeleteDevice();
	afx_msg void OnBnClickedBtnCopy();
	afx_msg void OnBnClickedBtnRecover();
	afx_msg void OnBnClickedBtnSave();
	afx_msg LRESULT OnCustomListItemChangedMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNMDblclkListDevice(NMHDR *pNMHDR, LRESULT *pResult);

private:
	void RefreshUniversityList();
	void RefreshAcademyList();
	void RefreshDeviceList();
	void InsertDeviceToList(DeviceInfo& info);
	void ModifyDeviceFromList(DeviceInfo& info);
	void DeleteDeviceFromList();
	void CtrlUnivItemChanged(int index);
	void CtrlAcademyItemChanged(int index);
	void CtrlDeviceItemChanged(int index);
private:
	CCustomListCtrl m_lstUniv;
	CCustomListCtrl m_lstAcademy;
	CCustomListCtrl m_lstDevice;

	UniversityXmlMnger m_xmlUniversity;
	AcademyXmlMnger   m_xmlAcademy;
	DeviceXmlMnger    m_xmlDevices;
	TiXmlDocument m_docAcademy;
	TiXmlDocument m_docDevice;
	int m_dwUnivCurIndex;
	int m_dwAcaCurIndex;
	int m_dwDevCurIndex;
};
