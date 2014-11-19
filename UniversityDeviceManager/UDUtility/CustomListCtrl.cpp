// CustomListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "CustomListCtrl.h"


// CCustomListCtrl

IMPLEMENT_DYNAMIC(CCustomListCtrl, CListCtrl)

CCustomListCtrl::CCustomListCtrl(emCustomList emList) : m_nColums(2)
									, m_bFixColumWidth(FALSE)
									, m_emList(emList)
{

}

CCustomListCtrl::~CCustomListCtrl()
{
}


BEGIN_MESSAGE_MAP(CCustomListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &CCustomListCtrl::OnLvnItemchanged)
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CCustomListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetDefaultColumNum();
	return 0;
}

void CCustomListCtrl::SetFixWidth( BOOL bFix /*= TRUE*/ )
{
	m_bFixColumWidth = bFix;
	if(m_bFixColumWidth)
	{
		CHeaderCtrl* pHeaderCtrl = (CHeaderCtrl*)GetHeaderCtrl();   
		pHeaderCtrl->EnableWindow( FALSE ) ;
	}
	else
	{
		CHeaderCtrl* pHeaderCtrl = (CHeaderCtrl*)GetHeaderCtrl();   
		pHeaderCtrl->EnableWindow( TRUE ) ;
	}
}


// CCustomListCtrl 消息处理程序




void CCustomListCtrl::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if(0 != pNMLV->uNewState)
	{
		::SendMessage(GetParent()->m_hWnd, WM_LIST_ITEM_CHANGED_MSG, m_emList, pNMLV->iItem);
	}
	*pResult = 0;
}

void CCustomListCtrl::SetColumNum( UINT8 num )
{
	m_nColums = num > 1 ? num : 2;
	int size = m_colWidthArry.size();
	for (int i = 0; i < m_nColums - size; i++)
	{
		m_colWidthArry.push_back(200);
	}
}

void CCustomListCtrl::SetDefaultColumNum()
{
	m_colWidthArry.push_back(200);
	m_colWidthArry.push_back(200);
}

BOOL CCustomListCtrl::InsertListColumn(  int nCol,  LPCTSTR lpszColumnHeading,  int nFormat /*= LVCFMT_LEFT*/,  int nWidth /*= -1*/ )
{
	int size = m_colWidthArry.size();
	if(nCol < size)
	{
		m_colWidthArry[nCol] = nWidth;
		InsertColumn( nCol, lpszColumnHeading, LVCFMT_LEFT, nWidth );
		return TRUE;
	}
	return FALSE;
}

BOOL CCustomListCtrl::PreTranslateMessage( MSG* pMsg )
{
	/*if(pMsg->wParam == 13)
	return GetParent()->PreTranslateMessage(pMsg);*/
	return CListCtrl::PreTranslateMessage(pMsg);
}
