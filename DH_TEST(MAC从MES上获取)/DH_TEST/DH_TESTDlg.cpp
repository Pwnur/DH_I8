
// DH_TESTDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DH_TEST.h"
#include "DH_TESTDlg.h"
#include "afxdialogex.h"
#include "WorkDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDH_TESTDlg 对话框




CDH_TESTDlg::CDH_TESTDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDH_TESTDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDH_TESTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_USERCODE, m_UserCodeEdit);
	DDX_Control(pDX, IDC_EDITRESCODE, m_ResEdit);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_PassWordEdit);
	DDX_Control(pDX, IDC_STATIC_TIPS, m_Tips);
	DDX_Control(pDX, IDCOK, m_Ok);
//	DDX_Control(pDX, IDC_BUTTON_JUMP, m_Jump);
}

BEGIN_MESSAGE_MAP(CDH_TESTDlg, CDialogEx)
	ON_WM_TIMER()   
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_MESSAGE (WM_MYMSG,OnMyMsg)
	ON_WM_QUERYDRAGICON()
//	ON_BN_CLICKED(IDC_BUTTON_JUMP, &CAITDlg::OnBnJump)
ON_BN_CLICKED(IDCOK, &CDH_TESTDlg::OnBnClickedCok)
ON_BN_CLICKED(IDCCANCEL, &CDH_TESTDlg::OnBnClickedCcancel)
END_MESSAGE_MAP()


// CDH_TESTDlg 消息处理程序

BOOL CDH_TESTDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_Mode = 0;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDH_TESTDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDH_TESTDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDH_TESTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/*
BOOL CDH_TESTDlg::PreTranslateMessage(MSG* pMsg)    
{  
    if (pMsg->message == WM_KEYDOWN)  
    {  
        switch(pMsg->wParam)  
        {  
        case VK_RETURN:  
            return TRUE;  

        case VK_ESCAPE:  
            return TRUE;  

        default:  
            break;  
        }  
    }  

    return CDH_TESTDlg::PreTranslateMessage(pMsg);  
}  
*/
void CDH_TESTDlg::OnBnClickedCok()
{
	// TODO: 在此添加控件通知处理程序代码
	
	DWORD ThreadID;
	HANDLE hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)RunThread,this,0,&ThreadID);
	m_Section.Lock(); 
	//SetTimer(8,3000,NULL);//启动定时器1,定时时间是1秒
	m_Section.Unlock(); 

}
void CDH_TESTDlg::OnOK ()
{
	
	DWORD ThreadID;
	HANDLE hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)RunThread,this,0,&ThreadID);
	m_Section.Lock(); 
	//SetTimer(8,3000,NULL);//启动定时器1,定时时间是1秒
	m_Section.Unlock(); 

	m_Ok.EnableWindow (FALSE);
//	m_Jump.EnableWindow (FALSE);
	
}

/*
void CDH_TESTDlg::OnBnJump()
{
	this->ShowWindow (SW_HIDE);
	this->m_Mode = 1;
	CWorkDlg dlg(this, NULL, NULL, NULL);
	theApp.m_pMainWnd = &dlg;
	dlg.DoModal();
}
*/

HBRUSH CDH_TESTDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
 {
     HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
     if( IDC_STATIC_TIPS  ==  pWnd->GetDlgCtrlID())//判断发出消息的空间是否是该静态文本框
     {
         pDC->SetTextColor(RGB(255,0,0));//设置文本颜色为红色
         hbr=CreateSolidBrush(RGB(240, 240, 240));//控件的背景色为绿色
     }
     return hbr;//返回用于重绘控件背景的画刷
}

void CDH_TESTDlg::RunThread (LPVOID pParam)
{
	CDH_TESTDlg *pDlg = (CDH_TESTDlg *)pParam;
	CString UserCode;
	CString ResCode;
	CString PassWord;
	CString J;
	CString Err;

	pDlg->m_UserCodeEdit.GetWindowTextA (UserCode);
	pDlg->m_ResEdit.GetWindowTextA (ResCode);
	pDlg->m_PassWordEdit.GetWindowTextA (PassWord);

	if (UserCode.GetLength () <= 0)
	{
		pDlg->m_Tips.SetWindowTextA ("请输入用户代码"); 
		return;
	}
	else if (ResCode.GetLength () <= 0)
	{
		pDlg->m_Tips.SetWindowTextA ("请输入资源代码");
		return;
	}
	else if (PassWord.GetLength () <= 0)
	{
		pDlg->m_Tips.SetWindowTextA ("请输入密码");
		return;
	}
	pDlg->m_Tips.SetWindowTextA ("");
	
	BOOL b = MesDLL::GetInstance ().CheckUserAndResourcePassed (UserCode, ResCode, PassWord, J, Err);
	pDlg->m_Ok.EnableWindow ();
//	pDlg->m_Jump.EnableWindow ();
	if (b == 1)
	{
		Info info;
		info.pDlg = pDlg;
		info.UserCode = UserCode;
		info.ResCode = ResCode;
		info.PassWord = PassWord;
		::SendMessage(pDlg->m_hWnd, WM_MYMSG, (WPARAM)&info, 0);
#if 0
		pDlg->ShowWindow (SW_HIDE);
		CWorkDlg dlg(pDlg, pUserCode, pResCode, pPassWord);
		theApp.m_pMainWnd = &dlg;
		dlg.DoModal();
#endif
	}
	else
	{
		CString tips = "连接错误:";
		tips += Err;
		pDlg->m_Tips.SetWindowText (tips);
	}
}

void CDH_TESTDlg::OnTimer(UINT nIDEvent)//定时器消息
{
	static int times = 0;
	if (nIDEvent == 8)
	{
		times++;
		if (times >= 3)
		{
			KillTimer (8);
			times = 0;

			CString tips = "连接超时,请确认网络状态！";
			m_Tips.SetWindowTextA (tips);
			m_Ok.EnableWindow ();
		}
	}
}

LRESULT  CDH_TESTDlg::OnMyMsg(WPARAM wParam, LPARAM lParam)
{
	Info *pInfo = (Info *)wParam;
	ShowWindow (SW_HIDE);
	WorkDlg dlg(this, pInfo->UserCode, pInfo->ResCode, pInfo->PassWord);
	theApp.m_pMainWnd = &dlg;
	dlg.DoModal();
	return 0;
}


void CDH_TESTDlg::OnBnClickedCcancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDH_TESTDlg::EndDialog( IDCCANCEL );

}
