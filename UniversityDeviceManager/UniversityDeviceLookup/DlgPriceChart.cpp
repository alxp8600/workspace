// DlgPriceChart.cpp : 实现文件
//

#include "stdafx.h"
#include "UniversityDeviceLookup.h"
#include "DlgPriceChart.h"
#include "afxdialogex.h"

#define DLG_SIDE_OFFSET 60
// CDlgPriceChart 对话框

IMPLEMENT_DYNAMIC(CDlgPriceChart, CDialog)

CDlgPriceChart::CDlgPriceChart(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPriceChart::IDD, pParent)
	, m_pMapPrice(NULL)
	, m_bOnceInsert(TRUE)
	, m_bShow(FALSE)
{

}

CDlgPriceChart::~CDlgPriceChart()
{
}

void CDlgPriceChart::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPriceChart, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgPriceChart 消息处理程序
 BOOL CDlgPriceChart::OnInitDialog()
 {
	 BOOL bRet = CDialog::OnInitDialog();
	 if(!m_bShow)
	 {
		 MessageBox(_T("只有一年的数据或者没数据，不予显示价格走向图！"));
	 }
	 return bRet;
 }

void CDlgPriceChart::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if(!m_bShow)
		return;

	if(m_pMapPrice)
	{
		CRect rcClient;
		GetClientRect(&rcClient);


		CPen pen(PS_SOLID, 1, RGB(0,0,0));
		CPen *pOldPen = dc.SelectObject(&pen);  
		dc.MoveTo(rcClient.left+DLG_SIDE_OFFSET, rcClient.top+DLG_SIDE_OFFSET);
		dc.LineTo(rcClient.right-DLG_SIDE_OFFSET, rcClient.top+DLG_SIDE_OFFSET);
		dc.LineTo(rcClient.right-DLG_SIDE_OFFSET, rcClient.bottom-DLG_SIDE_OFFSET);
		dc.LineTo(rcClient.left+DLG_SIDE_OFFSET, rcClient.bottom-DLG_SIDE_OFFSET);
		dc.LineTo(rcClient.left+DLG_SIDE_OFFSET, rcClient.top+DLG_SIDE_OFFSET);
		dc.SelectObject(pOldPen); 

		m_leftPos = rcClient.left+DLG_SIDE_OFFSET;
		m_bottomPos = rcClient.bottom-DLG_SIDE_OFFSET;
		m_chartWidth = rcClient.right-DLG_SIDE_OFFSET - (rcClient.left+DLG_SIDE_OFFSET);
		m_chartHeight = rcClient.bottom-DLG_SIDE_OFFSET - (rcClient.top+DLG_SIDE_OFFSET);

		DrawXCoord(dc);
		DrawYCoord(dc);
		DrawChart(dc);
		m_bOnceInsert = FALSE;
	}


}

void CDlgPriceChart::SetPriceData( PriceMap* pMap )
{
	m_pMapPrice = pMap;
	if(m_pMapPrice->size() > 1)
	{
		m_bShow = TRUE;
	}
}

void CDlgPriceChart::DrawXCoord(CPaintDC& dc)
{
	PriceMap::iterator iter = m_pMapPrice->begin();
	PriceMap::iterator iterHeader = m_pMapPrice->begin();
	int max = m_pMapPrice->size();
	int* gaps = new int[max];
	int index = 0;

	for (; iter != m_pMapPrice->end(); ++iter)
	{
		gaps[index] = iter->first - iterHeader->first;
		index++;
	}
	CFont	fntArial;
	CFont	*pOldFont = NULL;
	int nHeight = 12;
	fntArial.CreateFont(nHeight, 0, 0, 0, 
		FW_NORMAL, FALSE, FALSE, 0, 
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY,
		VARIABLE_PITCH | 0x04 | FF_DONTCARE,
		_T("Arial"));
	CPen pen(PS_SOLID, 1, RGB(0,0,255));
	CPen *pOldPen = dc.SelectObject(&pen);
	dc.SelectObject(pOldPen);
	pOldFont = dc.SelectObject(&fntArial);
	COLORREF clrOldColor = dc.SetTextColor(RGB(0,0,0));
	int nOldMode = dc.SetBkMode(TRANSPARENT);

	for (int i = 0; i < max; i++)
	{
		CPoint pt;
		pt.y = m_bottomPos;
		pt.x = m_leftPos + m_chartWidth* gaps[i]/gaps[max-1];

		if(m_bOnceInsert)
		{
			m_xCoords.push_back(pt);
		}
	
		dc.MoveTo(pt);
		pt.y -= 5;
		dc.LineTo(pt);
		pt.y += 5;

		CRect rc;
		rc.left = pt.x;
		rc.top = pt.y;
		rc.right = rc.left + 30;
		rc.bottom = rc.top+ 28;
		CString str;
		str.Format(_T("%d年"), iterHeader->first);
		dc.DrawText(str, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		
		iterHeader++;
		
	}
	dc.SetTextColor(clrOldColor);
	dc.SetBkMode(nOldMode);
	dc.SelectObject(pOldFont);
	DeleteObject(fntArial);
	delete[] gaps;
}

void CDlgPriceChart::DrawYCoord(CPaintDC& dc)
{
	PriceMap::iterator iter = m_pMapPrice->begin();
	int max = m_pMapPrice->size();
	int* gaps = new int[max];
	int index = 0;
	UINT maxPrice = iter->second;
	UINT minPrice = iter->second;

	for (; iter != m_pMapPrice->end(); ++iter)
	{
		if(iter->second > maxPrice)
			maxPrice = iter->second;
		if(iter->second < minPrice)
			minPrice = iter->second;
	}
	iter = m_pMapPrice->begin();
	CPoint pt;
	for (; iter != m_pMapPrice->end(); ++iter)
	{
		pt.x = m_leftPos;
		pt.y = m_bottomPos - m_chartHeight* ((double)(iter->second - minPrice)/(double)(maxPrice-minPrice));
		if(m_bOnceInsert)
			m_yCoords.push_back(pt);
	}
	CFont	fntArial;
	CFont	*pOldFont = NULL;
	CPen pen(PS_SOLID, 1, RGB(0,0,255));
	CPen *pOldPen = dc.SelectObject(&pen);
	int nHeight = 12;
	fntArial.CreateFont(nHeight, 0, 0, 0, 
		FW_NORMAL, FALSE, FALSE, 0, 
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY,
		VARIABLE_PITCH | 0x04 | FF_DONTCARE,
		_T("Arial"));
	pOldFont = dc.SelectObject(&fntArial);

	COLORREF clrOldColor = dc.SetTextColor(RGB(0,0,0));
	int nOldMode = dc.SetBkMode(TRANSPARENT);
	for (int i = 0; i < 11; i++)
	{
		
		pt.x = m_leftPos;
		pt.y = m_bottomPos - m_chartHeight* i/10;

		dc.MoveTo(pt);
		pt.x += 5;
		dc.LineTo(pt);
		pt.x -= 5;
		
		CRect rc;
		rc.left = pt.x - 60;
		rc.top = pt.y;
		rc.right = rc.left + 55;
		rc.bottom = rc.top+ 15;
		CString str;
		str.Format(_T("%d元"), minPrice + (maxPrice-minPrice)*i/10);
		dc.DrawText(str, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}
	dc.SelectObject(pOldPen);
	dc.SetTextColor(clrOldColor);
	dc.SetBkMode(nOldMode);
	dc.SelectObject(pOldFont);
	DeleteObject(fntArial);
}

void CDlgPriceChart::DrawChart( CPaintDC& dc )
{
	int num1 = m_xCoords.size();
	int num2 = m_yCoords.size();
	if(num2 != num1)
	{
		MessageBox(_T("图像参数出差！"));
		return;
	}
	CPen pen(PS_SOLID, 1, RGB(255,0,0));
	CPen *pOldPen = dc.SelectObject(&pen);

	CPoint pt;
	pt.x = m_xCoords[0].x;
	pt.y = m_yCoords[0].y;
	dc.MoveTo(pt);

	CFont	fntArial;
	CFont	*pOldFont = NULL;
	int nHeight = 10;
	fntArial.CreateFont(nHeight, 0, 0, 0, 
		FW_NORMAL, FALSE, FALSE, 0, 
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY,
		VARIABLE_PITCH | 0x04 | FF_DONTCARE,
		_T("Arial"));
	pOldFont = dc.SelectObject(&fntArial);

	COLORREF clrOldColor = dc.SetTextColor(RGB(0,0,0));
	int nOldMode = dc.SetBkMode(TRANSPARENT);
	CRect rc;
	rc.left = pt.x + 4;
	rc.top = pt.y - 20;
	rc.right = rc.left + 60;
	rc.bottom = rc.top+ 15;
	CString str;
	PriceMap::iterator iter = m_pMapPrice->begin();
	str.Format(_T("(%d, %d)"), iter->first, iter->second);
	dc.DrawText(str, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	for (int i = 1; i < num1; i++)
	{
		pt.x = m_xCoords[i].x;
		pt.y = m_yCoords[i].y;
		dc.LineTo(pt);

		rc.left = pt.x + 2;
		rc.top = pt.y - 15;
		rc.right = rc.left + 60;
		rc.bottom = rc.top+ 13;
		++iter;
		str.Format(_T("(%d, %d)"), iter->first, iter->second);
		dc.DrawText(str, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}
	dc.SelectObject(pOldPen);

	dc.SetTextColor(clrOldColor);
	dc.SetBkMode(nOldMode);
	dc.SelectObject(pOldFont);
	DeleteObject(fntArial);
}