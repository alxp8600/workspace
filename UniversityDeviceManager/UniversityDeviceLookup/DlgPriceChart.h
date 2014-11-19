#pragma once


// CDlgPriceChart 对话框

class CDlgPriceChart : public CDialog
{
	DECLARE_DYNAMIC(CDlgPriceChart)

public:
	CDlgPriceChart(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPriceChart();

	void SetPriceData(PriceMap* pMap);

// 对话框数据
	enum { IDD = IDD_DLG_PRICE_CHART };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	void DrawXCoord(CPaintDC& dc);
	void DrawYCoord(CPaintDC& dc);
	void DrawChart(CPaintDC& dc);
private:
	PriceMap* m_pMapPrice;
	UINT m_chartWidth;
	UINT m_chartHeight;
	int m_leftPos;
	int m_bottomPos;
	typedef std::vector<CPoint> XCoordPtVec;
	typedef std::vector<CPoint> YCoordPtVec;
	XCoordPtVec m_xCoords;
	YCoordPtVec m_yCoords;
	BOOL m_bOnceInsert;
	BOOL m_bShow;
};
