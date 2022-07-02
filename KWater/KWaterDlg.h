#pragma once
#include "ChartCtrl.h"
#include "ChartLineSerie.h"
#include "ChartLabel.h"

#include "ChartLineSerie.h"
#include "ChartPointsSerie.h"
#include "ChartGrid.h"
#include "ChartBarSerie.h"

#include "ChartAxisLabel.h"
#include "ChartStandardAxis.h"
#include "ChartDateTimeAxis.h"

#include "ChartCrossHairCursor.h"
#include "ChartDragLineCursor.h"

#include "KWaterDB.h"

#include <vector>
using namespace std;

#define SAFE_DELETE_ARRAY(ptr)    { if ((ptr)!=NULL) delete[] (ptr); (ptr) = NULL; }
class CKWaterDlg : public CDialogEx
{
public:
	CKWaterDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KWATER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	HICON m_hIcon;

protected:
	CDamTable tb;

	CChartCtrl m_ChartCtrl;
	CChartStandardAxis* m_pBottomAxis;
	CChartStandardAxis* m_pLeftAxis;
	CChartLineSerie* m_pSeries;

public:
	INT m_nMinMonth;		//X축 초기값
	INT m_nMaxMonth;		//X축 끝값
	INT m_nSize;		//데이터 갯수

	FLOAT m_fMinVar;		//Y축 초기값
	FLOAT m_fMaxVar;		 //Y축 끝값

public:
	void DrawGraph();
	void CalcEvapotranspiration(vector<double>* func, vector<int>nvX);

	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void On_DB_Import();
	afx_msg void On_DB_Export();
	afx_msg LRESULT UpdateInform(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()
	CComboBox m_CbMonth;
	CComboBox m_CbYear;
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnCbnSelchangeComboYear();
	afx_msg void OnCbnSelchangeComboMonth();
};
