#include "pch.h"
#include "framework.h"
#include "KWater.h"
#include "KWaterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

CKWaterDlg::CKWaterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KWATER_DIALOG, pParent)
	, m_nMinMonth(1)
	, m_nMaxMonth(12)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_KWATER); 
}

void CKWaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHARTCTRL, m_ChartCtrl);
	DDX_Control(pDX, IDC_COMBO_MONTH, m_CbMonth);
	DDX_Control(pDX, IDC_COMBO_YEAR, m_CbYear);
}

BEGIN_MESSAGE_MAP(CKWaterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(MSG_UPDATE, &CKWaterDlg::UpdateInform)
	ON_COMMAND(ID_DB_IMPORT, &CKWaterDlg::On_DB_Import)
	ON_COMMAND(ID_DB_EXPORT, &CKWaterDlg::On_DB_Export)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CKWaterDlg::OnBnClickedBtnSearch)
	ON_CBN_SELCHANGE(IDC_COMBO_YEAR, &CKWaterDlg::OnCbnSelchangeComboYear)
	ON_CBN_SELCHANGE(IDC_COMBO_MONTH, &CKWaterDlg::OnCbnSelchangeComboMonth)
END_MESSAGE_MAP()

void CKWaterDlg::CalcEvapotranspiration(vector<double>* func, vector<int>nvX)
{
	for (int x : nvX)
	{
		double y = -0.0001 * pow(x, 2) + 0.0545 * x - 1.9475;
		func->push_back(y);
	}
}

BOOL CKWaterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	tb.SetHwnd(m_hWnd);

	CString strYear;
	strYear.Format(_T("2022"));
	this->m_CbYear.AddString(strYear);
	strYear.Format(_T("2023"));
	this->m_CbYear.AddString(strYear);
	strYear.Format(_T("2024"));
	this->m_CbYear.AddString(strYear);
	strYear.Format(_T("2025"));
	this->m_CbYear.AddString(strYear);
	strYear.Format(_T("2026"));
	this->m_CbYear.AddString(strYear);

	CString strMonth;
	strMonth.Format(_T("January"));
	this->m_CbMonth.AddString(strMonth);
	strMonth.Format(_T("Feburary"));
	this->m_CbMonth.AddString(strMonth);
	strMonth.Format(_T("March"));
	this->m_CbMonth.AddString(strMonth);
	strMonth.Format(_T("April"));
	this->m_CbMonth.AddString(strMonth);
	strMonth.Format(_T("May"));
	this->m_CbMonth.AddString(strMonth);
	strMonth.Format(_T("June"));
	this->m_CbMonth.AddString(strMonth);
	strMonth.Format(_T("July"));
	this->m_CbMonth.AddString(strMonth);
	strMonth.Format(_T("August"));
	this->m_CbMonth.AddString(strMonth);
	strMonth.Format(_T("September"));
	this->m_CbMonth.AddString(strMonth);
	strMonth.Format(_T("October"));
	this->m_CbMonth.AddString(strMonth);
	strMonth.Format(_T("November"));
	this->m_CbMonth.AddString(strMonth);
	strMonth.Format(_T("December"));
	this->m_CbMonth.AddString(strMonth);
	

	return TRUE;
}

void CKWaterDlg::DrawGraph()
{
	CChartCtrl ref;
	ref.RemoveAllSeries();
	m_ChartCtrl.EnableRefresh(true);

	m_pBottomAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	m_pLeftAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);

	vector<INT>vn_xVar;
	vector<DOUBLE>vd_yEvapotranspiration;
	m_nSize = 600;

	for (int i = 0; i < m_nSize; i++)
		vn_xVar.push_back(i);
	CalcEvapotranspiration(&vd_yEvapotranspiration, vn_xVar);

	m_fMinVar = m_fMaxVar = vd_yEvapotranspiration.at(0);
	for (double y : vd_yEvapotranspiration)
	{
		m_fMaxVar = max(m_fMaxVar, y);
		m_fMinVar = min(m_fMinVar, y);
	}

	CRect rt;
	GetDlgItem(IDC_CHARTCTRL)->GetWindowRect(&rt);
	GetClientRect(&rt);

	vn_xVar.clear();
	double dhStep = rt.Width() / 13;
	double x_start = rt.left + dhStep;
	double x_end = rt.left + dhStep * 13;

	for (int i = 0; i < m_nSize; i++)
		vn_xVar.push_back((x_end - x_start) / m_nSize * i);

	double* nXVar, * dYVar;
	nXVar = new double[m_nSize];
	dYVar = new double[m_nSize];

	std::copy(vd_yEvapotranspiration.begin(), vd_yEvapotranspiration.end(), dYVar);
	std::copy(vn_xVar.begin(), vn_xVar.end(), nXVar);

	m_pBottomAxis->SetMinMax(1, m_nSize);		//가로축
	m_pLeftAxis->SetMinMax(m_fMinVar - 3, m_fMaxVar + 3);

	m_pBottomAxis->SetTickIncrement(false, ((FLOAT)m_nSize / 12));
	m_pBottomAxis->SetDiscrete(false);

	m_pSeries = m_ChartCtrl.CreateLineSerie();
	m_pSeries->SetWidth(5); // line Width
	m_pSeries->SetColor(RGB(200, 150, 0));

	m_pSeries->SetPoints(nXVar, dYVar, m_nSize);
	m_ChartCtrl.EnableRefresh(true);

	delete[] nXVar;
}

void CKWaterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CKWaterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CKWaterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CKWaterDlg::On_DB_Import() // DB 가져오기
{
}

void CKWaterDlg::On_DB_Export() // DB 내보내기
{
	/*CFileDialog  fdlg(FALSE, _T("*.jpg"), _T("snapshot.jpg"),
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("JPG Files (*.jpg)|*.jpg|PNG Files (*.png)|*.png|Bitmap Files (*.bmp)|*.bmp||"));
	fdlg.m_ofn.lpstrTitle = _T("Save Sanpshot");
	if (fdlg.DoModal() != IDOK)    return;

	std::deque<INT> q;
	POSITION pos = CListCtrl::GetFirstSelectedItemPosition();
	while (pos) q.push_back(GetNextSelectedItem(pos));

	TCHAR cwd[_MAX_PATH];
	GetCurrentDirectory(_MAX_PATH, cwd);

	CString csPathname = fdlg.GetPathName();
	PathRemoveFileSpec(csPathname.GetBuffer());
	SetCurrentDirectory(csPathname);
	CString csFormat; csFormat.Format(_T("%s-#%%d.%s"), fdlg.GetFileTitle(), fdlg.GetFileExt());
	ExportArgs args =
	{
	  csFormat,
	  &q,
	};

	SetCurrentDirectory(cwd);*/
}

LRESULT CKWaterDlg::UpdateInform(WPARAM wparam, LPARAM lparam)
{
	SetDlgItemText(IDC_EDIT_AREA,(LPCTSTR)tb.GetArea());
	SetDlgItemText(IDC_EDIT_HEIGHT,(LPCTSTR)tb.GetHeight());
	SetDlgItemText(IDC_EDIT_LENGTH, (LPCTSTR)tb.GetLength());
	SetDlgItemText(IDC_EDIT_DAM, (LPCTSTR)tb.GetName());

	return LRESULT();
}

void CKWaterDlg::OnBnClickedBtnSearch()
{
	CString cstcode;
	cstcode.Format(_T("%d"), GetDlgItemInt(IDC_EDIT_CODE));
	tb.SetFindDamcode(cstcode);
	tb.ConnectToDB();
	tb.GetColumns();
}


void CKWaterDlg::OnCbnSelchangeComboYear()
{
	DrawGraph();
}


void CKWaterDlg::OnCbnSelchangeComboMonth()
{
	INT nCursel;
	//nCursel = ((CComboBox*)GetDlgItem(IDC_COMBO_COLORSET))->GetCurSel();
}
