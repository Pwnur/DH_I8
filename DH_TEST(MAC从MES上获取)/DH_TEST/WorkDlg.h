#pragma once
#include "DH_TESTDlg.h"



#include "afxwin.h"
#include "winsock2.h"
/*
#include "BenQGuruDll.h"
#pragma comment(lib, "MESDLL.lib")
*/

#define WM_GETMACCOMPLETE WM_USER + 102

#define SIG_MAGIC ('N'<<24)+('S'<<16)+('8'<<8)+('I'<<0)
#pragma pack(4)
typedef struct _FACTORYTEST_DATA
{
	unsigned long MAGIC;
  
	char SN[16];
	char MAC[16];
	char VER[32];

	char testSN;
	char testMAC;
	char testSD;
	char testSPK;
	char testMic;
	char testWifi;

	char testCamera;
	char testIR;
	char testIRCut;
	char testLightSensor;

	char testLED;
	char testKey;
	char testVersion;
	char UpdateSW;

	//-----------------------
	char rtSN;
	char rtMAC;
	char rtSD;
	char rtSPK;
	char rtMic;
	char rtWifi;

	char rtCamera;
	char rtIR;
	char rtIRCut;
	char rtLightSensor;
  
	char rtLED;
	char rtKey;
	char rtVersion;
	char rtUpdateSW;

	char eixttest;
}FACTORYTEST_DATA;

typedef struct
{
    BYTE  Mactemp[16];
    BYTE curnum[5];
}MACINFO;

typedef struct _NOTIFY_MSG
{
	char MAG[4];
	char IP[32];
	char Port[12];
}NOTIFY_MSG;

static MACINFO iMac;
static FACTORYTEST_DATA lFtd;

// WorkDlg 对话框

class WorkDlg : public CDialogEx
{
	DECLARE_DYNAMIC(WorkDlg)

public:
	WorkDlg(CWnd* pParent = NULL);   // 标准构造函数
	WorkDlg(CDH_TESTDlg *pLogin, CString UserCode, CString Res, CString PassWord, CWnd* pParent = NULL);   // 重载构造函数
	virtual ~WorkDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
//	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
//	afx_msg void OnPaint();
//	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	virtual void OnTimer(UINT nIDEvent);//定时器消息
	virtual HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	public:
	CIPAddressCtrl m_IP;
	CEdit m_Scan_Sn;
	CEdit m_Sn;
	CEdit m_Iperf;
	CEdit m_Tips;
	CButton m_ButtonOk;
	CButton m_Check1;
	CButton m_Check2;
	afx_msg void OnBnClickedButton1();

	void EnableWindows (BOOL b);

	CString ｍ_ExePath;//应用程序路径

	CFont font;			//改变字体大小

	DWORD m_dwCnt;

	CString m_UserCode;
	CString m_Res;
	CString m_PassWord;
public:
	HANDLE m_Event;
	HANDLE m_Event_UDP;
	static DWORD WINAPI ThreadFunc(LPVOID lpParam);
	static DWORD WINAPI ThreadFunc2(LPVOID lpParam); //UDP服务器
	LRESULT OnGetMacComplete(WPARAM wParam, LPARAM lParam);
	CWnd *m_MainWnd;
	

	int iperfTest();
	int PhasePerfLog(CString pPath);

	int DoRun ();
//	BOOL PreTranslateMessage(MSG* pMsg);
	BOOL socket_client();
	BOOL GetMac();
	BOOL RecordMac ();
	void Add(BYTE pos);
	afx_msg void OnEnChangeEditScanSn();
	afx_msg void OnBnClickedCheck();
	afx_msg void OnBnClickedButton2();
};
