#pragma once
#include "HeaderData.h"

#define STATIC_SIDEBORDER_LEFT 1
#define STATIC_SIDEBORDER_TOP 2
#define STATIC_SIDEBORDER_RIGHT 4
#define STATIC_SIDEBORDER_BOTTOM 8

#define STATIC_SIDEBORDER_All            STATIC_SIDEBORDER_LEFT|STATIC_SIDEBORDER_TOP|STATIC_SIDEBORDER_RIGHT|STATIC_SIDEBORDER_BOTTOM
#define STATIC_SIDEBORDER_NOSIDE         0
#define STATIC_SIDEBORDER_NOBOTTOM       STATIC_SIDEBORDER_LEFT|STATIC_SIDEBORDER_TOP|STATIC_SIDEBORDER_RIGHT
#define STATIC_SIDEBORDER_NOTOP          STATIC_SIDEBORDER_LEFT|STATIC_SIDEBORDER_BOTTOM|STATIC_SIDEBORDER_RIGHT
// CBmpTextStatic
typedef UINT STATIC_SIDEBORDER_MODE;

class UD_EXPORT_CLASS CBmpTextStatic : public CStatic
{
	DECLARE_DYNAMIC(CBmpTextStatic)

public:
	CBmpTextStatic(CWnd* pFather = NULL);
	virtual ~CBmpTextStatic();

	void SetCaption(LPCTSTR caption);
	void SetRightTopImage(LPCTSTR strName){m_rtImgName = strName;}
	void ShowRightTopImage(BOOL bShow = TRUE){m_bShowRTimg = bShow;}
	void SetFontSize(short fntSize);
	void SetFontColor(COLORREF color = RGB(140,140,140));
	void SetBorder(BOOL bSetting = TRUE);
	void SetBorderColor(COLORREF color);
	void SetSideBorder( STATIC_SIDEBORDER_MODE mode = STATIC_SIDEBORDER_All);
	void SetCtrlBkColor(BOOL bEnable = TRUE, COLORREF color = RGB(0,0,0)); // 设置背景色和设置图片只能一个有效

	BOOL SetStaticBitmap(LPCTSTR strName);
	void CloseStaticBitmap();
	BOOL SetImage(LPCTSTR strName);

	void SetTextHorAlign(UINT nFormat);
	void SetTextVerAlign(UINT nFormat);
	void SetTextOffsetFromLeft(UINT offset = 0);
	void SetTextOffsetFromTop(UINT offset = 0);
	void LockStatic( BOOL bLock = TRUE);
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

private:
	CWnd* m_pFatherWnd;
	CString m_csCaption;
	COLORREF m_color;
	BOOL m_bBorder;
	STATIC_SIDEBORDER_MODE m_nBorderMode;
	BOOL m_bBkColor;
	COLORREF m_bkColor;

	UINT m_nTextHorFormat;
	UINT m_nTextVerFormat;
	UINT m_nLeftOffset;
	UINT m_nTopOffset;

	HBITMAP m_hBmp;
	short m_nFontSize;
	BOOL m_bLock;

	CString m_rtImgName;
	BOOL m_bShowRTimg;

	COLORREF m_crBorder;

	CImage m_img;
};


