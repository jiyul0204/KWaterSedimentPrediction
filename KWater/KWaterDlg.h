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

enum MODE
{
	EVE,
	SOIL
};

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
	CDamTable m_CDamTable;
	CPredictionsoilerosionTable m_CPredictionsoilerosionTable;

	CChartCtrl m_ChartCtrl;
	CChartStandardAxis* m_pBottomAxis;
	CChartStandardAxis* m_pLeftAxis;
	CChartLineSerie* m_pSeries;

public:
	INT m_nMinMonth;		//X축 초기값
	INT m_nMaxMonth;		//X축 끝값
	INT m_nSize;		//데이터 갯수

	FLOAT m_fPrecip;	//총강수량
	FLOAT m_fRfIntensity;	//강우강도
	FLOAT m_fRfSoil	;		//토사유발 강우량
	FLOAT m_fRfSoil_Intensity;	// 토사유발 강우강도
	FLOAT m_fSunshine;		//증발산량
	FLOAT m_feva;		//일조량 (값이 없어도 예측에 무방함 )	

	FLOAT m_fMinVar;		//Y축 초기값
	FLOAT m_fMaxVar;		 //Y축 끝값

	INT m_nReset;			//리셋시 초기화되는 월
	vector<FLOAT>m_vfEvaperMon;	//월평균 증발산량
public:
	BOOL ISCode();
	void DrawGraph();
	vector<FLOAT> CalcMonthEvapotranspiration(vector<DOUBLE>* vec, INT nMonth);
	void CalcPredict(vector<double>* func, vector<int>nvX, INT nMonth, INT nMode);

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
	afx_msg void OnBnClickedBtnReset();
};
