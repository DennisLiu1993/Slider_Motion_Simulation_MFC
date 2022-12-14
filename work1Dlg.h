
// work1Dlg.h: 標頭檔
//

#pragma once


// Cwork1Dlg 對話方塊
class Cwork1Dlg : public CDialogEx
{
// 建構
public:
	Cwork1Dlg(CWnd* pParent = nullptr);	// 標準建構函式

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WORK1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援


// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedInitialize();
	afx_msg void OnBnClickedStop();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeInitialtheta();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_strTIME;
	afx_msg void OnStnClickedImg();
	afx_msg void Draw(double,double);

	HANDLE m_hBitmap;
	CImage m_image;
	BOOL m_bLoadImage;
	afx_msg void OnHScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	int m_iPos1;
	int m_iPos2;
};
