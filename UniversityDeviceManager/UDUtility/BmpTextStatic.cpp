// BmpTextStatic.cpp : 实现文件
//

#include "stdafx.h"
#include "BmpTextStatic.h"
#include <atlimage.h>

// CBmpTextStatic

IMPLEMENT_DYNAMIC(CBmpTextStatic, CStatic)

CBmpTextStatic::CBmpTextStatic(CWnd* pFather) : m_pFatherWnd(pFather)
								, m_csCaption(_T(""))
								, m_color(RGB(140,140,140))
								, m_bBorder(FALSE)
								, m_nBorderMode(0)
								, m_bBkColor(FALSE)
								, m_bkColor(RGB(45,145,251))
								, m_nLeftOffset(0)
								, m_nTopOffset(0)
								, m_nTextHorFormat(DT_SINGLELINE | DT_CENTER)
								, m_nTextVerFormat(DT_SINGLELINE| DT_VCENTER)
								, m_hBmp(NULL)
								, m_bLock(FALSE)
								, m_nFontSize(8)
								, m_bShowRTimg(FALSE)
								, m_rtImgName(_T(""))
								, m_crBorder(RGB(45,145,251))
{

}

CBmpTextStatic::~CBmpTextStatic()
{
	if(m_hBmp)
		DeleteObject(m_hBmp);

}


BEGIN_MESSAGE_MAP(CBmpTextStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CBmpTextStatic::OnPaint()
{
	CRect rcClient;
	CPaintDC dc(this); 

	GetClientRect(&rcClient);

	CBitmap *pOldBitmap;
	CDC dcMem;
	BITMAP bmpInfo;
	dcMem.CreateCompatibleDC(&dc);
	if(GetBitmap())
	{
		CBitmap* pBmp = CBitmap::FromHandle(GetBitmap());
		pOldBitmap = dcMem.SelectObject(pBmp);
		pBmp->GetBitmap(&bmpInfo);
		SetStretchBltMode(dc.m_hDC,STRETCH_HALFTONE);//防止图片大于static大小后，图片被压缩失真
		dc.StretchBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
		dcMem.SelectObject(pOldBitmap);
	}
	else
	{
		if(!m_img.IsNull())
		{
			SetStretchBltMode(dc.m_hDC,STRETCH_HALFTONE);//防止图片大于static大小后，图片被压缩失真
			m_img.Draw(dcMem, rcClient);
		}
	}
	
	if(m_bShowRTimg)
	{
		CImage img;
		HBITMAP hBitmap;
		CBitmap* pbmp = NULL;
		img.Load(m_rtImgName);
		hBitmap = img.operator HBITMAP();
		if (hBitmap)
		{
			pbmp = CBitmap::FromHandle(hBitmap);
			pbmp->GetBitmap(&bmpInfo);
			pOldBitmap = dcMem.SelectObject(pbmp);
			dc.StretchBlt(rcClient.right - bmpInfo.bmWidth - 1, rcClient.top + 1, bmpInfo.bmWidth, bmpInfo.bmHeight, &dcMem, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
			dcMem.SelectObject(pOldBitmap);
		}
	}
	

	if(m_bBkColor)
	{
		dc.FillSolidRect(rcClient, m_bkColor);
	}

	if(m_bBorder)
	{
		CPen pen(PS_SOLID, 1, m_crBorder);
		CPen *pOldPen = dc.SelectObject(&pen);  
		dc.MoveTo(rcClient.left, rcClient.top);
		dc.LineTo(rcClient.right-1, rcClient.top);
		dc.LineTo(rcClient.right-1, rcClient.bottom-1);
		dc.LineTo(rcClient.left, rcClient.bottom-1);
		dc.LineTo(rcClient.left, rcClient.top);
		dc.SelectObject(pOldPen); 
	}
	else
	{
		if(m_nBorderMode)
		{
			CPen pen(PS_SOLID, 1, m_crBorder);
			CPen *pOldPen = dc.SelectObject(&pen);
			
			if( m_nBorderMode & STATIC_SIDEBORDER_TOP)
			{
				dc.MoveTo(rcClient.left, rcClient.top);
				dc.LineTo(rcClient.right-1, rcClient.top);
			}
			if (m_nBorderMode & STATIC_SIDEBORDER_RIGHT)
			{
				dc.MoveTo(rcClient.right-1, rcClient.top);
				dc.LineTo(rcClient.right-1, rcClient.bottom-1);
			}
			if (m_nBorderMode & STATIC_SIDEBORDER_BOTTOM)
			{
				dc.MoveTo(rcClient.right-1, rcClient.bottom-1);
				dc.LineTo(rcClient.left, rcClient.bottom-1);
			}
			if (m_nBorderMode & STATIC_SIDEBORDER_LEFT)
			{
				dc.MoveTo(rcClient.left, rcClient.bottom-1);
				dc.LineTo(rcClient.left, rcClient.top);
			}
			dc.SelectObject(pOldPen); 
		}
	}


	VERIFY(dcMem.DeleteDC());

	if(!m_csCaption.IsEmpty())
	{
		CFont	fntArial;
		CFont	*pOldFont = NULL;
		int nHeight = -MulDiv(m_nFontSize, dc.GetDeviceCaps(LOGPIXELSY), 72);
		fntArial.CreateFont(nHeight, 0, 0, 0, 
			FW_NORMAL, FALSE, FALSE, 0, 
			ANSI_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			PROOF_QUALITY,
			VARIABLE_PITCH | 0x04 | FF_DONTCARE,
			_T("Arial"));
		pOldFont = dc.SelectObject(&fntArial);

		COLORREF clrOldColor = dc.SetTextColor(m_color);
		int nOldMode = dc.SetBkMode(TRANSPARENT);
		rcClient.left = m_nLeftOffset;
		rcClient.top = m_nTopOffset;
		dc.DrawText(m_csCaption, -1, &rcClient, m_nTextHorFormat|m_nTextVerFormat);
		dc.SetTextColor(clrOldColor);
		dc.SetBkMode(nOldMode);
		dc.SelectObject(pOldFont);
	}
	CStatic::OnPaint();
}

BOOL CBmpTextStatic::OnEraseBkgnd( CDC* pDC )
{
	return TRUE;
	//return CStatic::OnEraseBkgnd(pDC);
}

void CBmpTextStatic::SetCaption( LPCTSTR caption )
{
	if(caption)
		m_csCaption = caption;
	else
		m_csCaption = _T("");
}

void CBmpTextStatic::SetFontColor( COLORREF color )
{
	m_color = color;
}

void CBmpTextStatic::SetBorder( BOOL bSetting /*= TRUE*/ )
{
	m_bBorder = bSetting;
}

void CBmpTextStatic::SetSideBorder( STATIC_SIDEBORDER_MODE mode )
{
	m_nBorderMode = mode;
}

void CBmpTextStatic::SetCtrlBkColor(BOOL bEnable, COLORREF color)
{
	m_bBkColor = bEnable;
	if(bEnable)
		m_bkColor = color;
}

void CBmpTextStatic::SetTextHorAlign( UINT nFormat )
{
	m_nTextHorFormat = DT_SINGLELINE | nFormat;
}

void CBmpTextStatic::SetTextOffsetFromLeft( UINT offset /*= 0*/ )
{
	m_nLeftOffset = offset;
}

void CBmpTextStatic::SetTextVerAlign( UINT nFormat )
{
	m_nTextVerFormat = DT_SINGLELINE | nFormat;
}

void CBmpTextStatic::SetTextOffsetFromTop( UINT offset /*= 0*/ )
{
	m_nTopOffset = offset;
}

BOOL CBmpTextStatic::SetStaticBitmap( LPCTSTR strName )
{
	CImage img;
	HRESULT hr = img.Load(strName);
	if(S_OK == hr && !img.IsNull())
	{
		if(m_hBmp)
		{
			::DeleteObject(m_hBmp);
			m_hBmp = NULL;
		}
		m_hBmp = img.Detach();
		HBITMAP oldBmp = SetBitmap(m_hBmp);
		if(oldBmp)
		{
			DeleteObject(oldBmp);
		}
		return TRUE;
	}

	
	return FALSE;
}

BOOL CBmpTextStatic::SetImage(LPCTSTR strName)
{
	if(m_img.IsNull())
	{
		if(S_OK == m_img.Load(strName))
			return TRUE;
	}
	
	return FALSE;
}

BOOL CBmpTextStatic::PreTranslateMessage( MSG* pMsg )
{
	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONUP|| pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP || pMsg->message == WM_LBUTTONDBLCLK)
	{
		if(m_bLock)
			return TRUE;
	}
	return CStatic::PreTranslateMessage(pMsg);
}

void CBmpTextStatic::LockStatic( BOOL bLock /*= TRUE*/ )
{
	m_bLock = bLock;
}

void CBmpTextStatic::SetFontSize( short fntSize )
{
	if(fntSize<= 0)
		fntSize = 8;
	m_nFontSize = fntSize;
}

void CBmpTextStatic::SetBorderColor( COLORREF color )
{
	m_crBorder = color;
}

void CBmpTextStatic::CloseStaticBitmap()
{
	if(m_hBmp)
	{
		DeleteObject(m_hBmp);
		SetBitmap(NULL);
		m_hBmp = NULL;
	}
}
// CBmpTextStatic 消息处理程序


