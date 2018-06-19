
// DH_TESTDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DH_TEST.h"
#include "DH_TESTDlg.h"
#include "afxdialogex.h"
#include "WorkDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CDH_TESTDlg �Ի���




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


// CDH_TESTDlg ��Ϣ�������

BOOL CDH_TESTDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_Mode = 0;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDH_TESTDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	DWORD ThreadID;
	HANDLE hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)RunThread,this,0,&ThreadID);
	m_Section.Lock(); 
	//SetTimer(8,3000,NULL);//������ʱ��1,��ʱʱ����1��
	m_Section.Unlock(); 

}
void CDH_TESTDlg::OnOK ()
{
	
	DWORD ThreadID;
	HANDLE hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)RunThread,this,0,&ThreadID);
	m_Section.Lock(); 
	//SetTimer(8,3000,NULL);//������ʱ��1,��ʱʱ����1��
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
     if( IDC_STATIC_TIPS  ==  pWnd->GetDlgCtrlID())//�жϷ�����Ϣ�Ŀռ��Ƿ��Ǹþ�̬�ı���
     {
         pDC->SetTextColor(RGB(255,0,0));//�����ı���ɫΪ��ɫ
         hbr=CreateSolidBrush(RGB(240, 240, 240));//�ؼ��ı���ɫΪ��ɫ
     }
     return hbr;//���������ػ�ؼ������Ļ�ˢ
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
		pDlg->m_Tips.SetWindowTextA ("�������û�����"); 
		return;
	}
	else if (ResCode.GetLength () <= 0)
	{
		pDlg->m_Tips.SetWindowTextA ("��������Դ����");
		return;
	}
	else if (PassWord.GetLength () <= 0)
	{
		pDlg->m_Tips.SetWindowTextA ("����������");
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
		CString tips = "���Ӵ���:";
		tips += Err;
		pDlg->m_Tips.SetWindowText (tips);
	}
}

void CDH_TESTDlg::OnTimer(UINT nIDEvent)//��ʱ����Ϣ
{
	static int times = 0;
	if (nIDEvent == 8)
	{
		times++;
		if (times >= 3)
		{
			KillTimer (8);
			times = 0;

			CString tips = "���ӳ�ʱ,��ȷ������״̬��";
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDH_TESTDlg::EndDialog( IDCCANCEL );

}
