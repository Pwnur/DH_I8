#pragma once
#include "DH_TESTDlg.h"
//#include "FRAME.h"


#include "afxwin.h"
#include "winsock2.h"
#include "fanplayer.h"

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

typedef struct _TEST_RES
{
	int	light;
	int picture;
	int	ir;
	int speak;
}TEST_RES;

static TEST_RES iRES;
static int sockfd = 0;
static MACINFO iMac;
static FACTORYTEST_DATA lFtd;
static char iVersion[32];

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
	CEdit m_Scan_Sn;
	CEdit m_Sn;
	CEdit m_Tips;

	CStatic m_SnTips;
	CStatic m_Key;
	CStatic m_ResSn;
	CStatic m_ResMac;
	CStatic m_Version;
	CStatic m_ResVersion;



	CButton m_ButtonOK;
	CButton m_ButtonRED;
	CButton m_ButtonGREEN;
	CButton m_ButtonBLUE;
	CButton m_ButtonIR;
	CButton m_ButtonIRCUT;
//	CButton m_ButtonSPK;


	CButton m_Buttonlight;
	CButton m_Buttonpicture;
	CButton m_Buttonir;
	CButton m_Buttonspeak;


	CButton m_StaticLED;
	CButton m_StaticPICTURE;


	CButton m_Check_SELECT;
	CButton m_Check_RED;
	CButton m_Check_GREEN;
	CButton m_Check_BLUE;
	CButton m_Check_SPK;
	CButton m_Check_IR;
	CButton m_Check_IRCUT;
	
	CIPAddressCtrl m_IP;

	void EnableWindows (BOOL b);

	CString ｍ_ExePath;//应用程序路径

	CFont font;			//改变字体大小
	CFont ResFont;			//测试结果字体大小
	CFont TestFont;			//测试结果字体大小

	CString m_UserCode;
	CString m_Res;
	CString m_PassWord;
public:
	HANDLE m_Event;
	HANDLE m_Event2;
	HANDLE m_Event_UDP;
	static DWORD WINAPI ThreadFunc(LPVOID lpParam); 
	static DWORD WINAPI ThreadFunc2(LPVOID lpParam); //UDP服务器
	LRESULT OnGetMacComplete(WPARAM wParam, LPARAM lParam);
	CWnd *m_MainWnd;
	
	int PhasePerfLog(CString pPath);

	//与机器通信
	int numbytes;

	int DoRun ();
//	BOOL PreTranslateMessage(MSG* pMsg);
	BOOL socket_client();
	BOOL GetMac();
	BOOL GetVersion();

	void TestResult();

	afx_msg void OnEnChangeEditScanSn();
	afx_msg void OnBnClickedLedRed();
	afx_msg void OnBnClickedLedGreen();
	afx_msg void OnBnClickedLedBlue();
	afx_msg void OnBnClickedLedIr();
	afx_msg void OnBnClickedIrcut();
	afx_msg void OnBnClickedupload();
	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedCheckSelect();
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedSpk();
	afx_msg void OnBnClickedLightRes();
	afx_msg void OnBnClickedPictureRes();
	afx_msg void OnBnClickedIrRes();
	afx_msg void OnBnClickedSpakeRes();
	afx_msg void OnBnClickedKey();
	afx_msg void OnBnClickedIr();
};
