
// DH_TESTDlg.h : ͷ�ļ�
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
// CDH_TESTDlg �Ի���
class CDH_TESTDlg : public CDialogEx
{
// ����
public:
	CDH_TESTDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DH_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	virtual void OnOK();
	virtual HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	virtual void OnTimer(UINT nIDEvent);//��ʱ����Ϣ
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