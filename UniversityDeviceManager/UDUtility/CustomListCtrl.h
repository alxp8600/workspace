#pragma once
#include "HeaderData.h"

enum emCustomList
{
	Custom_Univ,
	Custom_Academy,
	Custom_Device
};
#define WM_LIST_ITEM_CHANGED_MSG WM_USER + 100


// CCustomListCtrl

class UD_EXPORT_CLASS CCustomListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CCustomListCtrl)

public:
	CCustomListCtrl(emCustomList emList);
	virtual ~CCustomListCtrl();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetFixWidth(BOOL bFix = TRUE);
	void SetColumNum(UINT8 num);
	BOOL InsertListColumn( int nCol,  LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT,  int nWidth = -1);

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
private:
	void SetDefaultColumNum();
private:
	UINT8 m_nColums;
	typedef std::vector<UINT> ColumWidthsArry;
	ColumWidthsArry m_colWidthArry;
	BOOL m_bFixColumWidth;
	emCustomList m_emList;
};


