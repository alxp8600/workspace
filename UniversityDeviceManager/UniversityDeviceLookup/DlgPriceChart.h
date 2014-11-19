#pragma once


// CDlgPriceChart �Ի���

class CDlgPriceChart : public CDialog
{
	DECLARE_DYNAMIC(CDlgPriceChart)

public:
	CDlgPriceChart(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgPriceChart();

	void SetPriceData(PriceMap* pMap);

// �Ի�������
	enum { IDD = IDD_DLG_PRICE_CHART };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
