
// work1Dlg.cpp: 實作檔案
//

#include "stdafx.h"

#include "work1.h"
#include "work1Dlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <ctime> 
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

#define M_PI_180 M_PI/180.0
#define M_PI_30 M_PI/30.0
const double M_2PI = M_PI * 2;
const int icenterX = 425, icenterY = 350,iRadius=25;
//#define M_2PI M_PI/0.5

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

double g_dL1, g_dL2, g_dL3, g_dL4, g_dRPM, g_dSec, g_dH1, g_dH2, g_dTheta1, g_dTheta2;
double g_dOmega = 0,g_dTime=0, g_dTheta1now, g_dTheta2now,g_dStopTheta1,g_dStopTheta2;
clock_t g_clockStartTime,g_clockStopTime;
bool g_isStop = false;
// 對 App About 使用 CAboutDlg 對話方塊

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

// 程式碼實作
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


// Cwork1Dlg 對話方塊



Cwork1Dlg::Cwork1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WORK1_DIALOG, pParent)
	, m_strTIME(_T(""))
	, m_bLoadImage (FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cwork1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange (pDX);

	DDX_Text (pDX, IDC_EDIT_TIME, m_strTIME);
	DDX_Text (pDX, IDC_EDIT_POSITION1, m_iPos1);
	DDX_Text (pDX, IDC_EDIT_POSITION2, m_iPos2);
}

BEGIN_MESSAGE_MAP(Cwork1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &Cwork1Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &Cwork1Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_Start, &Cwork1Dlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_Initialize, &Cwork1Dlg::OnBnClickedInitialize)
	ON_BN_CLICKED(IDC_Stop, &Cwork1Dlg::OnBnClickedStop)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &Cwork1Dlg::OnNMCustomdrawSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &Cwork1Dlg::OnNMCustomdrawSlider2)
	ON_EN_CHANGE(IDC_InitialTheta, &Cwork1Dlg::OnEnChangeInitialtheta)
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_IMG, &Cwork1Dlg::OnStnClickedImg)
	ON_WM_HSCROLL ()
END_MESSAGE_MAP()


// Cwork1Dlg 訊息處理常式

BOOL Cwork1Dlg::OnInitDialog()
{
	// 將 [關於...] 功能表加入系統功能表。
	// IDM_ABOUTBOX 必須在系統命令範圍之中。
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
	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO: 在此加入額外的初始設定
	// 各edit control 之初始值
	GetDlgItem(IDC_EDIT_L1)->SetWindowText(_T("100"));
	GetDlgItem(IDC_EDIT_L2)->SetWindowText(_T("150"));
	GetDlgItem(IDC_EDIT_L3)->SetWindowText(_T("100"));
	GetDlgItem(IDC_EDIT_L4)->SetWindowText(_T("250"));
	GetDlgItem(IDC_EDIT_RPM)->SetWindowText(_T("40"));
	GetDlgItem (IDC_EDIT_THETA1)->SetWindowText (_T ("45"));
	GetDlgItem (IDC_EDIT_POSITION1)->SetWindowText (_T ("45"));
	GetDlgItem (IDC_EDIT_THETA2)->SetWindowText (_T ("135"));
	GetDlgItem (IDC_EDIT_POSITION1)->SetWindowText (_T ("328"));
	GetDlgItem(IDC_EDIT_SEC)->SetWindowText(_T("3"));
	
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->SetRange(0,360);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER2))->SetRange(0, 360);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->SetPos(45);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER2))->SetPos(135);
	
	GetDlgItem(IDC_EDIT_H1)->SetWindowText(_T("0"));
	GetDlgItem(IDC_EDIT_H2)->SetWindowText(_T("0"));
	GetDlgItem(IDC_EDIT_OMEGA)->SetWindowText(_T("0"));
	GetDlgItem(IDC_EDIT_TIME)->SetWindowText(_T("0"));
	//嵌入照片
	m_hBitmap = ::LoadImage (NULL, L"C:\\Users\\user\\source\\repos\\work1\\manual.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_COLOR);
	
	if (m_hBitmap)
	{
		m_image.Attach ((HBITMAP)m_hBitmap);
		m_bLoadImage = TRUE;
	}
	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void Cwork1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void Cwork1Dlg::OnPaint()
{
	//初始化
	OnBnClickedInitialize();
	CPaintDC dc (this); // 繪製的裝置內容
	if (IsIconic())
	{
		

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// 描繪圖示
		
		
	}
	else
	{
		Draw (g_dTheta1now, g_dTheta2now);
		if (m_bLoadImage)
		{
			CRect rect;
			GetDlgItem (IDC_IMG)->GetWindowRect (rect);
			ScreenToClient (rect);
			m_image.Draw (dc.m_hDC, rect);

		}
		CDialogEx::OnPaint();
	}
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR Cwork1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Cwork1Dlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		KillTimer(1);

		double dAlpha = g_dRPM / g_dSec;
		g_dTime = (clock() - g_clockStartTime) / 1000.0;

		if (g_dTime<g_dSec) 
		{
			g_dOmega = dAlpha * g_dTime;
			g_dTheta1now = g_dTheta1 + 0.5*g_dOmega*g_dTime;
			g_dTheta2now = g_dTheta2 + 0.5*g_dOmega*g_dTime;
		}
		else
		{
			g_dOmega = g_dRPM;
			g_dTheta1now = g_dTheta1 + (2 * g_dTime - g_dSec)*g_dOmega / 2.0;
			g_dTheta2now = g_dTheta2 + (2 * g_dTime - g_dSec)*g_dOmega / 2.0;
		}
		
		CString strOmega, strTime;
		strOmega.Format(_T("%.2f"), g_dOmega / M_PI * 30);
		strTime.Format(_T("%.2f"), g_dTime);
		GetDlgItem(IDC_EDIT_TIME)->SetWindowText(strTime);
		GetDlgItem(IDC_EDIT_OMEGA)->SetWindowText(strOmega);
		Draw(g_dTheta1now, g_dTheta2now);
		
		if (!g_isStop)
			SetTimer(1,20,NULL);
	}
	else if(nIDEvent==2)
	{
		KillTimer(2);
		
		double dAlpha = -g_dRPM / g_dSec;
		double dAccTime = -g_dOmega / dAlpha;
		double dTime = (clock() - g_clockStopTime) / 1000.0;
		double dOmega=0, dTheta1now=0, dTheta2now=0;

		if (dTime<dAccTime) 
		{
			dOmega = g_dOmega + dAlpha * dTime;
			dTheta1now = g_dTheta1now + g_dOmega * dTime + 0.5*dAlpha*dTime*dTime;
			dTheta2now = g_dTheta2now + g_dOmega * dTime + 0.5*dAlpha*dTime*dTime;
			if (dTheta1now > M_2PI)
				dTheta1now -= (int)(dTheta1now / M_2PI) * M_2PI;
			if (dTheta2now > M_2PI)
				dTheta2now -= (int)(dTheta2now / M_2PI) * M_2PI;
			if (dTheta1now < 0)
				dTheta1now += ((int)(-dTheta1now / M_2PI) + 1) * M_2PI;
			if (dTheta2now < 0)
				dTheta2now += ((int)(-dTheta2now / M_2PI) + 1) * M_2PI;
			
			g_dStopTheta1 = dTheta1now;
			g_dStopTheta2 = dTheta2now;
		}
		else
		{
			g_dOmega = 0;
			g_dTheta1 = g_dStopTheta1;//使stop後的start可以接續
			g_dTheta2 = g_dStopTheta2;
			dTheta1now = g_dStopTheta1;//使stop後的最後一次畫圖與停止前的最後一次相同
			dTheta2now = g_dStopTheta2;
			CString strStopTheta1, strStopTheta2;//停止後，顯示停止位置的角度
			strStopTheta1.Format(_T("%.2f"), g_dStopTheta1 * 180 / M_PI);
			strStopTheta2.Format(_T("%.2f"), g_dStopTheta2 * 180 / M_PI);
			GetDlgItem(IDC_EDIT_THETA1)->SetWindowText(strStopTheta1);
			GetDlgItem(IDC_EDIT_THETA2)->SetWindowText(strStopTheta2);
			g_isStop = false;
		}


		CString strOmega, strTime;
		strOmega.Format(_T("%.2f"), dOmega / M_PI * 30);
		strTime.Format(_T("%.2f"), dTime);
		GetDlgItem(IDC_EDIT_TIME)->SetWindowText(strTime);
		GetDlgItem(IDC_EDIT_OMEGA)->SetWindowText(strOmega);

		Draw(dTheta1now, dTheta2now);
		
		if(g_isStop)
			SetTimer(2, 20, NULL);
	}

	CDialogEx::OnTimer(nIDEvent);
}

void Cwork1Dlg::OnBnClickedStop()
{
	g_isStop = true;
	SetTimer(2, 10, NULL);
	g_clockStopTime = clock();
}
void Cwork1Dlg::OnBnClickedStart()
{
	SetTimer(1, 10, NULL);
	g_clockStartTime = clock();
}

void Cwork1Dlg::OnBnClickedInitialize()
{
	CString strL1, strL2, strL3, strL4, strInitialTheta1, strInitialTheta2, strH1, strH2, strRPM, strSec;
	CEdit *pEditL1 = (CEdit*)GetDlgItem(IDC_EDIT_L1);
	CEdit *pEditL2 = (CEdit*)GetDlgItem(IDC_EDIT_L2);
	CEdit *pEditL3 = (CEdit*)GetDlgItem(IDC_EDIT_L3);
	CEdit *pEditL4 = (CEdit*)GetDlgItem(IDC_EDIT_L4);
	CEdit *pH1 = (CEdit*)GetDlgItem(IDC_EDIT_H1);
	CEdit *pH2 = (CEdit*)GetDlgItem(IDC_EDIT_H2);
	CEdit *pEditTheta1 = (CEdit*)GetDlgItem(IDC_EDIT_THETA1);
	CEdit *pEditTheta2 = (CEdit*)GetDlgItem(IDC_EDIT_THETA2);
	CEdit *pEditRPM = (CEdit*)GetDlgItem(IDC_EDIT_RPM);
	CEdit *pEditSec = (CEdit*)GetDlgItem(IDC_EDIT_SEC);
	pH1->GetWindowText(strH1);
	pH2->GetWindowText(strH2);
	pEditL1->GetWindowText(strL1);
	pEditL2->GetWindowText(strL2);
	pEditL3->GetWindowText(strL3);
	pEditL4->GetWindowText(strL4);
	pEditTheta1->GetWindowText(strInitialTheta1);
	pEditTheta2->GetWindowText(strInitialTheta2);
	pEditRPM->GetWindowText(strRPM);
	pEditSec->GetWindowText(strSec);
	g_dL1 = _ttof(strL1);
	g_dL2 = _ttof(strL2);
	g_dL3 = _ttof(strL3);
	g_dL4 = _ttof(strL4);
	g_dH1 = _ttof(strH1);
	g_dH2 = _ttof(strH2);
	g_dTheta1 = _ttof(strInitialTheta1)*M_PI_180;
	g_dTheta2 = _ttof(strInitialTheta2)*M_PI_180;
	g_dRPM = _ttof(strRPM)*M_PI_30;
	g_dSec = _ttof(strSec);
	Draw(g_dTheta1,g_dTheta2);

}
void Cwork1Dlg::Draw(double Theta1,double Theta2)
{
	CClientDC showDC(this);//將顯示的DC
	CRect rectDraw(0, 170, 1100, 700);//暫存圖形的區域
	CDC drawDC;//暫存圖形的DC ; 顯示設備對象
	CBitmap bitmapTemp, *pOldBitmap;
	bitmapTemp.CreateCompatibleBitmap(&showDC, rectDraw.Width(), rectDraw.Height());// 建立螢幕顯示兼容的點陣圖
	drawDC.CreateCompatibleDC(NULL);//繪圖與顯示設備之間的緩衝
	pOldBitmap = drawDC.SelectObject(&bitmapTemp);//將點陣圖選入內存顯示設備中(選入點陣圖的顯示設備才有地方繪圖)
	drawDC.FillSolidRect(rectDraw, RGB(240, 240, 240));//設定背景色

	double horizontal = sqrt(g_dL2*g_dL2 - (g_dL1*sin(Theta1) - g_dH1)*(g_dL1*sin(Theta1) - g_dH1));

	CPoint Slider1LeftTop(icenterX-iRadius + (g_dL1 + iRadius)*cos(Theta1) + horizontal, icenterY - g_dH1 - iRadius);//右邊滑塊
	CPoint Slider1RightBottom(icenterX+iRadius + (g_dL1 + iRadius)*cos(Theta1) + horizontal, icenterY+iRadius - g_dH1);
	CRect Slider1(Slider1LeftTop, Slider1RightBottom);
	CPen penRight(PS_SOLID, 5, RGB(50, 100, 100));
	CPen* pOldPen = drawDC.SelectObject(&penRight);
	CBrush* pOldBrush = (CBrush*)drawDC.SelectStockObject(NULL_BRUSH);
	drawDC.Rectangle(Slider1);

	/*Drawdc.MoveTo(207, 500);
	Drawdc.LineTo(425, 500);*/

	CPoint L1Left(icenterX + iRadius * cos(Theta1), icenterY - iRadius * sin(Theta1));//右邊兩曲柄
	CPoint L1Right(icenterX + (g_dL1 + iRadius)*cos(Theta1), icenterY - (g_dL1 + iRadius)*sin(Theta1));
	CPoint L2Right(icenterX + (g_dL1 + iRadius)*cos(Theta1) + horizontal, icenterY - g_dH1);
	drawDC.MoveTo(L1Left);
	drawDC.LineTo(L1Right);
	drawDC.MoveTo(L1Right);
	drawDC.LineTo(L2Right);

	CPoint OrgLeftTop(icenterX-iRadius, icenterY-iRadius), OrgRightBottom(icenterX + iRadius, icenterY+iRadius);//圓中心(425,350)
	CRect CircleRect(OrgLeftTop, OrgRightBottom);
	CPen penOrg(PS_SOLID, 5, RGB(50, 50, 50));
	drawDC.SelectObject(&penOrg);
	drawDC.Ellipse(CircleRect);
	drawDC.MoveTo(icenterY + iRadius, icenterY + iRadius);
	drawDC.LineTo(icenterX + 2 * iRadius, icenterY + iRadius);

	horizontal = sqrt(g_dL4*g_dL4 - (g_dL3*sin(Theta2) - g_dH2)*(g_dL3*sin(Theta2) - g_dH2));
	CPoint Slider2LeftTop(icenterX - iRadius + (g_dL3 + iRadius)*cos(Theta2) - horizontal, icenterY - g_dH2 - iRadius);//左邊滑塊
	CPoint Slider2RightBottom(icenterX + iRadius + (g_dL3 + iRadius)*cos(Theta2) - horizontal, icenterY + iRadius - g_dH2);
	CRect Slider2(Slider2LeftTop, Slider2RightBottom);
	CPen penLeft(PS_SOLID, 5, RGB(100, 50, 50));
	drawDC.SelectObject(&penLeft);
	drawDC.SelectStockObject(NULL_BRUSH);
	drawDC.Rectangle(Slider2);

	CPoint L3Left(icenterX + iRadius * cos(Theta2), icenterY - iRadius * sin(Theta2));//左邊兩曲柄
	CPoint L3Right(icenterX + (g_dL3 + iRadius)*cos(Theta2), icenterY - (g_dL3 + iRadius)*sin(Theta2));
	CPoint L4Right(icenterX + (g_dL3 + iRadius)*cos(Theta2) - horizontal, icenterY - g_dH2);
	drawDC.MoveTo(L3Left);
	drawDC.LineTo(L3Right);
	drawDC.MoveTo(L3Right);
	drawDC.LineTo(L4Right);

	CPen penHub(PS_SOLID, 5, RGB(0, 0, 0));
	drawDC.SelectObject(&penHub);
	drawDC.SelectStockObject(BLACK_BRUSH);
	CPoint pntDiff(-5, -5);
	CRect rectHub1(L1Right + pntDiff, L1Right + (-pntDiff));
	CRect rectHub2(L2Right + pntDiff, L2Right + (-pntDiff));
	CRect rectHub3(L3Right + pntDiff, L3Right + (-pntDiff));
	CRect rectHub4(L4Right + pntDiff, L4Right + (-pntDiff));
	drawDC.Ellipse(rectHub1);
	drawDC.Ellipse(rectHub2);
	drawDC.Ellipse(rectHub3);
	drawDC.Ellipse(rectHub4);

	drawDC.Ellipse(rectHub1);
	CPoint pntDiff2(-3, -3);
	CRect rectFixed1(L1Left + pntDiff, L1Left + (-pntDiff));
	CRect rectFixed2(L3Left + pntDiff, L3Left + (-pntDiff));
	drawDC.Ellipse(rectFixed1);
	drawDC.Ellipse(rectFixed2);

	showDC.SelectObject(pOldPen);
	showDC.SelectObject(&pOldBrush);
	showDC.BitBlt(0, 170, rectDraw.Width(), rectDraw.Height(), &drawDC, 0, 170, SRCCOPY);//將內存的圖複製到螢幕上顯示
	bitmapTemp.DeleteObject();
	drawDC.DeleteDC();
}
void Cwork1Dlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	int iSlider1Value = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->GetPos();//取得目前滑塊位置
	CString strShowValue;
	strShowValue.Format(_T("%d"), iSlider1Value);//將滑塊位置轉為CString
	GetDlgItem(IDC_InitialTheta)->SetWindowText(strShowValue);

	CString strL1, strL2, strH1, strH2;
	CEdit *editL1=(CEdit*)GetDlgItem(IDC_EDIT_L1);
	CEdit *editL2 = (CEdit*)GetDlgItem(IDC_EDIT_L2);
	editL1->GetWindowText(strL1);
	editL2->GetWindowText(strL2);
	double dTheta1 = iSlider1Value * M_PI_180;
	double dL1 = _ttoi(strL1);
	double dL2 = _ttoi(strL2);
	//double dHorizontal = sqrt(dL2*dL2 - (dL1*sin(g_dTheta1now) - dH1)*(dL1*sin(g_dTheta1now) - dH1));
	
	//strShowValue.Format(_T("%d"), (int)dPosition1);
	GetDlgItem(IDC_Position1)->SetWindowText(strShowValue);
	*pResult = 0;
}


void Cwork1Dlg::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	int iSlider2Value = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER2))->GetPos();
	CString strtemp;
	strtemp.Format(_T("%d"), iSlider2Value);
	GetDlgItem(IDC_InitialTheta2)->SetWindowText(strtemp);

	CString strL3, strL4;
	CEdit *editL3 = (CEdit*)GetDlgItem(IDC_EDIT_L3);
	CEdit *editL4 = (CEdit*)GetDlgItem(IDC_EDIT_L4);
	editL3->GetWindowText(strL3);
	editL4->GetWindowText(strL4);
	double dTheta2 = iSlider2Value * M_PI_180;
	double dL3 = _ttoi(strL3);
	double dL4 = _ttoi(strL4);
	double dCosFi = sqrt(1 - dL3 * dL3*sin(dTheta2)*sin(dTheta2) / (dL4*dL4));
	double dSinFi = dL3 * sin(dTheta2) / dL4;
	double dPosition2 = (dL3 + 25)*cos(dTheta2) + dL4 * dCosFi;
	if (dTheta2>M_PI / 2) dPosition2 = -(dL3 + 25)*cos(dTheta2) + dL4 * dCosFi;
	strtemp.Format(_T("%d"), (int)dPosition2);
	GetDlgItem(IDC_Position2)->SetWindowText(strtemp);
	*pResult = 0;
}
void Cwork1Dlg::OnBnClickedOk() 
{

}
void Cwork1Dlg::OnBnClickedCancel() 
{
	DestroyWindow();
}


void Cwork1Dlg::OnEnChangeInitialtheta()
{
	// TODO:  如果這是 RICHEDIT 控制項，控制項將不會
	// 傳送此告知，除非您覆寫 CDialogEx::OnInitDialog()
	// 函式和呼叫 CRichEditCtrl().SetEventMask()
	// 讓具有 ENM_CHANGE 旗標 ORed 加入遮罩。

	// TODO:  在此加入控制項告知處理常式程式碼
	/*CString strL1, strL2,strTheta1;
	CEdit *editL1 = (CEdit*)GetDlgItem(IDC_EditL1);
	CEdit *editL2 = (CEdit*)GetDlgItem(IDC_EDITL2);
	CEdit *editL3 = (CEdit*)GetDlgItem(IDC_EDITL3);
	editL1->GetWindowText(strL1);
	editL2->GetWindowText(strL2);
	editL3->GetWindowText(strTheta1);
	double dTheta1 = _ttoi(strTheta1)* 3.14 / 180;
	double dL1 = _ttoi(strL1);
	double dL2 = _ttoi(strL2);
	double dCosFi = sqrt(1 - dL1 * dL1*sin(dTheta1)*sin(dTheta1) / (dL2*dL2));
	double dSinFi = dL1 * sin(dTheta1) / dL2;
	double dPosition1 = (dL1 + 25)*cos(dTheta1) + dL2 * dCosFi;
	CString strtemp;
	strtemp.Format(_T("%d"), (int)dPosition1);
	GetDlgItem(IDC_Position1)->SetWindowText(strtemp);*/
}


void Cwork1Dlg::OnStnClickedImg()
{
	// TODO: 在此加入控制項告知處理常式程式碼
}


void Cwork1Dlg::OnHScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	//Invalidate ();
	CDialogEx::OnHScroll (nSBCode, nPos, pScrollBar);
}
