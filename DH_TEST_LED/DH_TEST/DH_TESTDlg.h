
// DH_TESTDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "fanplayer.h"
#pragma comment(lib, "fanplayer.lib")

#include "BenQGuruDll.h"
#pragma comment(lib, "MESDLL.lib")

#include "DBDLL.h"
#pragma comment(lib, "ORACLEDLL.lib")

#define WM_MYMSG WM_USER + 1
// CDH_TESTDlg 对话框
class CDH_TESTDlg : public CDialogEx
{
// 构造
public:
	CDH_TESTDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DH_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual void OnOK();
	virtual HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT  OnMyMsg(WPARAM, LPARAM);
//	afx_msg BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

public:
	CEdit m_UserCodeEdit;
	CEdit m_ResEdit;
	CEdit m_PassWordEdit;
	CButton m_Ok;
	CStatic m_Tips;
	int m_Mode;

	static void RunThread (LPVOID pParam);
	CCriticalSection m_Section;
	virtual void OnTimer(UINT nIDEvent);//定时器消息
	SYSTEMTIME m_Time;
	
	afx_msg void OnBnClickedCok();
	afx_msg void OnBnClickedCcancel();
};

struct Info
{
	CDH_TESTDlg *pDlg;
	CString UserCode;
	CString ResCode;
	CString PassWord;
};