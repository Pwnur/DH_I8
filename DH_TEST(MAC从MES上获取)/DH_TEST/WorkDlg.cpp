// WorkDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DH_TEST.h"
#include "WorkDlg.h"
#include "afxdialogex.h"

#define TIMER1 1000
#define TIMER2 80

// WorkDlg 对话框

IMPLEMENT_DYNAMIC(WorkDlg, CDialogEx)

WorkDlg::WorkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(WorkDlg::IDD, pParent)
{

}

WorkDlg::WorkDlg(CDH_TESTDlg *pLogin, CString UserCode, CString Res, CString PassWord, CWnd* pParent /*=NULL*/)
	: CDialogEx(WorkDlg::IDD, pParent)
{
	TCHAR tempExePath[MAX_PATH];
	::GetModuleFileName(NULL, tempExePath, MAX_PATH);
	ｍ_ExePath.Format(_T("%s"), tempExePath);
	ｍ_ExePath = ｍ_ExePath.Left(ｍ_ExePath.ReverseFind('\\')+1) ;

	/*
	HANDLE CreateEvent(
		LPSECURITY_ATTRIBUTES lpEventAttributes,   // 安全属性
		BOOL bManualReset,   // 复位方式  //为FALSE则不用手动ResetEvent()
		BOOL bInitialState,   // 初始状态
		LPCTSTR lpName   // 对象名称
		);
*/
	m_Event = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_Event_UDP = CreateEvent(NULL, FALSE, FALSE, NULL);
	DWORD dw;
	CreateThread(NULL, 0, ThreadFunc, this, 0, &dw);

	DWORD dw2;
	CreateThread(NULL, 0, ThreadFunc2, this, 0, &dw2);

	/*
	AfxInitRichEdit2 ();
	TCHAR tempExePath[MAX_PATH];
	::GetModuleFileName(NULL, tempExePath, MAX_PATH);
	ｍ_ExePath.Format(_T("%s"), tempExePath);
	ｍ_ExePath = ｍ_ExePath.Left(ｍ_ExePath.ReverseFind('\\')+1) ;
	m_nMACQREncodePath = ｍ_ExePath + "MAC_QR\\";
//	m_ErrPath = ｍ_ExePath + "ErrLog\\";
//	m_MacFile = ｍ_ExePath + "MAC.txt";
*/

	m_UserCode = UserCode;
	m_Res = Res;
	m_PassWord = PassWord;
//	m_pLogin = pLogin;

	
	
}

WorkDlg::~WorkDlg()
{
	CString ErrMessage;
	BOOL b = MesDLL::GetInstance().ATELogOut(m_Res,ErrMessage);
}

void WorkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_IP);
	DDX_Control(pDX, IDC_EDIT_SCAN_SN, m_Scan_Sn);
	DDX_Control(pDX, IDC_EDIT_SN, m_Sn);
	DDX_Control(pDX, IDC_EDIT_IPERF, m_Iperf);
	DDX_Control(pDX, IDC_EDIT_TIPS, m_Tips);
	DDX_Control(pDX, IDC_BUTTON1, m_ButtonOk);
	DDX_Control(pDX, IDC_CHECK1, m_Check1);
	DDX_Control(pDX, IDC_CHECK2, m_Check2);
}


BEGIN_MESSAGE_MAP(WorkDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_MESSAGE(WM_GETMACCOMPLETE, OnGetMacComplete)
	ON_BN_CLICKED(IDC_BUTTON1, &WorkDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT_SCAN_SN, &WorkDlg::OnEnChangeEditScanSn)
	ON_BN_CLICKED(IDC_CHECK1, &WorkDlg::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_CHECK2, &WorkDlg::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_BUTTON2, &WorkDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// WorkDlg 消息处理程序
BOOL WorkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	m_Check1.SetCheck (1);
	m_Check2.SetCheck (1);
	m_Scan_Sn.SetFocus ();

	m_Sn.EnableWindow (FALSE);
	m_Iperf.EnableWindow (FALSE);
	m_Tips.EnableWindow (FALSE);

	SetTimer(1, TIMER1, NULL);//启动定时器1,定时时间是1秒
	SetTimer(2, TIMER2, NULL);//启动定时器1,定时时间是1秒

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


	// TODO: 在此添加额外的初始化代码
	AllocConsole();
	freopen( "CONOUT$","w",stdout);
	m_MainWnd = AfxGetThread()->GetMainWnd();

	font.CreatePointFont(880, _T("宋体"));
	GetDlgItem(IDC_EDIT_TIPS)->SetFont(&font);

	return FALSE;  // 除非将焦点设置到控件，否则返回 TRUE
}
/*
void WorkDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void WorkDlg::OnPaint()
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
/*
HCURSOR WorkDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
*/
//重载回车键的函数
/*
BOOL WorkDlg::PreTranslateMessage(MSG* pMsg)   
{  
    // 顺次传递焦点,除非当前焦点在确定按钮上时才响应触发消息  
    if((pMsg->message == WM_KEYDOWN) && (VK_RETURN == (int) pMsg->wParam))  
    {  
        if(GetFocus()->GetDlgCtrlID() != IDOK)  
        {  
            pMsg->wParam = VK_TAB;  
        }  
    }    
    return CDialog::PreTranslateMessage(pMsg);  
}
*/
HBRUSH WorkDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
 {
     HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
     if( IDC_EDIT_TIPS  ==  pWnd->GetDlgCtrlID()) //判断发出消息的空间是否是该静态文本框
     {
        CString str;
		 m_Tips.GetWindowTextA(str);
		 if( str == "NG")
		 {
			pDC->SetTextColor(RGB(255,0,0));//设置文本颜色为红色
//         hbr=CreateSolidBrush(RGB(240, 240, 240));//控件的背景色为绿色
		 }
		 else
		 {
			pDC->SetTextColor(RGB(0,250,0));//设置文本颜色为绿色
		 }
	 }

	 if(IDC_EDIT_IPERF == pWnd->GetDlgCtrlID())
	 {
		pDC->SetTextColor(RGB(255,0,0));//设置文本颜色为红色
	 }
     return hbr;//返回用于重绘控件背景的画刷
}

void WorkDlg::Add(BYTE pos)
{
	
		if(iMac.Mactemp[pos] == 0x39)
		{
			iMac.Mactemp[pos] = 0x41;

		}
		else if(iMac.Mactemp[pos] == 0x46)
		{
			iMac.Mactemp[pos] = 0x30;
			Add(pos-1);
		}
			else
		iMac.Mactemp[pos]++;
}
/*
BOOL WorkDlg::GetMac ()
{
	CString filename;
	filename = ｍ_ExePath + "mac.txt";
	CFile MacFile;
	CFileException fileException;

	if (!PathFileExists(filename))//写日志文件
	{
		AfxMessageBox ("mac.txt不存在！");
		return FALSE;
	}
	MacFile.Open(filename,CFile::typeText|CFile::modeReadWrite);
	MacFile.Read(&iMac,sizeof(iMac));

	MacFile.Close();
	return TRUE;
}
*/

DWORD WINAPI WorkDlg::ThreadFunc2(LPVOID lpParam)
{

	WorkDlg *pDlg = (WorkDlg *)lpParam;

	SOCKET socket1;
	WSADATA wsaData;
	NOTIFY_MSG msg;

	DWORD g_dwOldAddress = 0;
	DWORD g_dwNewAddress = 0;
	char cNewIP[32];

	if (WSAStartup(MAKEWORD(2,2),&wsaData)) //调用Windows Sockets DLL
	{
		 AfxMessageBox("Winsock无法初始化!\n");
		 WSACleanup();
		 return 0;
	}

	struct sockaddr_in local;
	struct sockaddr_in from;
	int fromlen =sizeof(from);
	local.sin_family=AF_INET;
	local.sin_port=htons(8989); ///监听端口
	local.sin_addr.s_addr=INADDR_ANY; ///本机

	socket1=socket(AF_INET,SOCK_DGRAM,0);

	int nNetTimeout=5000;//1秒
	//接收时限
	setsockopt(socket1, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));
	bind(socket1,(struct sockaddr*)&local,sizeof(local));

	//AfxMessageBox("UDPServer start");
	while (1)
	{
		if (recvfrom(socket1,(char*)&msg,sizeof(msg),0,(struct sockaddr*)&from,&fromlen)!=SOCKET_ERROR)
		{
			CString str;
			str.Format("Received datagram from %s:%d--%s\n",inet_ntoa(from.sin_addr), (from.sin_port),msg.IP);
			//AfxMessageBox(str);
			strcpy((char*)cNewIP, inet_ntoa(from.sin_addr));
			//pDlg->m_dwAddress
			g_dwNewAddress = ntohl( inet_addr( inet_ntoa(from.sin_addr) ) );
			pDlg->m_IP.SetWindowTextA(cNewIP);
			//pDlg->UpdateIP();
			//strcpy(msg.IP, "192.168.0.201");
			if(g_dwNewAddress != g_dwOldAddress)
			{
				g_dwOldAddress = g_dwNewAddress;
				pDlg->EnableWindows (TRUE);
				SetEvent (pDlg->m_Event_UDP);				
				printf("Get New Device ADDR\n");
			}
		}
		else
		{
			g_dwNewAddress = g_dwOldAddress = 0;
			//AfxMessageBox("udp recv err");
			pDlg->EnableWindows (FALSE);
			printf("连接已断开\n");
		}
	}
	closesocket(socket1);
	//pDlg->UpdateIP();
	return 0;
}

BOOL WorkDlg::GetMac ()
{
	CString SN;
	CString MAC;
	CString BT;
	CString Code1;
	CString Code2;
	CString Code3;
	CString ErrMessage;
	m_Sn.GetWindowTextA(SN);
	BOOL b = MesDLL::GetInstance().GetAddressRangeByMO(SN,MAC,BT,Code1,Code2,Code3,ErrMessage);	

	if(!b)
	{
		return FALSE;
	}
	else
	{
		memcpy(&lFtd.MAC, MAC, 15);
		printf("the mac is:%s\n",&lFtd.MAC);
	}

	return TRUE;
}
BOOL WorkDlg::RecordMac ()
{
	CString filename;
	filename = ｍ_ExePath + "mac.txt";
	CFile MacFile;
	CFileException fileException;

	MacFile.Open(filename,CFile::modeCreate|CFile::modeWrite);
	Add(15);
	CString strnum = (CString)iMac.curnum;
	m_dwCnt = _ttoi(strnum);
	m_dwCnt++;
	sprintf((char*)iMac.curnum, "%04d", m_dwCnt);
//	AfxMessageBox(strnum+"  "+(CString)(iMac.curnum));
	MacFile.Write(&iMac,sizeof(iMac));
	MacFile.Close();
	return TRUE;
}

int WorkDlg::PhasePerfLog(CString pPath)
{
	CStdioFile file;
	CString str;
	int iGet=0;

	file.Open(pPath,CFile::modeRead); 
	while(file.ReadString(str))
	{
		int i= str.Find("sender", 0);
		CString ss;
		ss.Format("%s %d", str, i);
		if(i>0)
		{      
			i = str.Find("MBytes", 0);
			str = str.Right(str.GetLength()-i-6);
			i = str.Find("Mbits", 0);
			if(i<0)
			continue;
			str = str.Left(i);
			str.TrimLeft();
			str.TrimRight();
			//ss.Format("_%s_ %d", str, str.GetLength());
			//AfxMessageBox(ss);
			iGet = 1;

			int d = _ttoi(str);
			return d;
    }
}
  
  return 0;
}
int WorkDlg::iperfTest()
{
	CString filename1,filename2,ip,command;
	filename1 = ｍ_ExePath + "iperf-3.1.3-win32\\iperf3.exe";
	filename2 = ｍ_ExePath + "iperf-3.1.3-win32\\test.log";
	m_IP.GetWindowTextA(ip); 
	command = filename1 + " -c " + ip + " > " + filename2;

	system(command);
	//system("F:\\工厂\\登虹\\登虹\\TEST_CLIENT\\Release\\iperf-3.1.3-win32\\iperf3.exe -c 172.16.98.99 -> F:\\工厂\\登虹\\登虹\\TEST_CLIENT\\Release\\iperf-3.1.3-win32\\test.log");
	int BS = PhasePerfLog(filename2);
	CString str;
	str.Format("BS=%d", BS);
	printf("BS=%d\n",BS);

	return BS;
}
BOOL WorkDlg::socket_client()
{ 
	int sockfd, numbytes;
    char sendbuf[256];
	char revbuf[256];
	memset(sendbuf,0,256);
	memset(revbuf,0,256);

    struct hostent *he;
    struct sockaddr_in their_addr;
    unsigned int myport;

    myport = 7838;

	CString ip;
	m_IP.GetWindowTextA(ip);
	printf(ip.GetBuffer());

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if((he=gethostbyname(ip.GetBuffer()))==NULL) 
	{
        AfxMessageBox("gethostbyname error");
        WSACleanup();
		closesocket(sockfd);
		return FALSE;
    }
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) 
	{
        AfxMessageBox("socket error");
		WSACleanup();
		closesocket(sockfd);
		return FALSE;
    }
    their_addr.sin_family=PF_INET;
    their_addr.sin_port=htons(myport);
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    memset(&(their_addr.sin_zero),0, sizeof(their_addr.sin_zero));

//	connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr));

    if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1) 
	{
//        AfxMessageBox("connect error");
        WSACleanup();
		closesocket(sockfd);
		return FALSE;
    }

	lFtd.MAGIC = SIG_MAGIC;
	CString str1,str2,str3;

	memset(&lFtd.SN, 0, sizeof(lFtd.SN));	
	m_Sn.GetWindowTextA(str1);
	memcpy(&lFtd.SN, str1.GetBuffer(), 16);

	memset(&lFtd.MAC, 0, sizeof(lFtd.MAC));
//	m_Mac.GetWindowTextA(str2);
	BOOL b = GetMac();
	if(!b)
	{
		AfxMessageBox("获取MAC失败");
		return FALSE;
	}
/*
//	m_Mac.SetWindowTextA(str2);
	memcpy(&lFtd.MAC, iMac.Mactemp, 16);
	printf("the mac is:%s\n",&lFtd.MAC);
	b = RecordMac ();
	if(!b)
	{
		AfxMessageBox("写入MAC地址失败");
		return FALSE;
	}
*/

	memset(&lFtd.testSN, 0, 6);
	str3 = "111111";
	memcpy(&lFtd.testSN, str3.GetBuffer(), 6);
	printf("the test is:%d\n",lFtd.testSN);

	int B = iperfTest();  //xMbit >25Mbits
	CString iperf;
	iperf.Format(_T("%d"), B);
	m_Iperf.SetWindowTextA(iperf);

	printf("send to server!\n");
	printf("the sizeof is:%d\n",sizeof(lFtd));
	if (send(sockfd, (const char *)&lFtd, sizeof(lFtd), 0) == -1) 
	{
		AfxMessageBox("发送数据失败！");
		WSACleanup();
		closesocket(sockfd);
		return FALSE;
	}
	memset(&lFtd, 0, sizeof(lFtd));
    if ((numbytes=recv(sockfd, (char *)&lFtd, sizeof(lFtd), 0)) == -1) 
	{
        AfxMessageBox("接受数据失败！");
        WSACleanup();
		closesocket(sockfd);
		return FALSE;
    }
	printf("the recv is:%d\n",lFtd.MAGIC);
	printf("the recv is:%s\n",lFtd.SN);
	printf("the recv is:%s\n",lFtd.MAC);
	printf("the recv is:%d\n",lFtd.testSN);
	printf("the recv is:%d\n",lFtd.testMAC);
	printf("the recv is:%d\n",lFtd.testSD);
	printf("the recv is:%d\n",lFtd.testSPK);
	printf("the recv is:%d\n",lFtd.testMic);


	WSACleanup();
	closesocket(sockfd);
	return TRUE;					
}

void WorkDlg::OnBnClickedCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Scan_Sn.SetFocus ();
	if (m_Check1.GetCheck () == 1)
	{
		m_Scan_Sn.SetFocus ();
	}
	
	UINT ID = LOWORD(GetCurrentMessage()->wParam);
	{
		if (ID == IDC_CHECK1)
		{
			if (m_Check1.GetCheck () == 0)
			{
				KillTimer (1);
				KillTimer (2);
				m_Scan_Sn.EnableWindow (FALSE);
				m_Check2.EnableWindow (FALSE);
			}
			else if (m_Check1.GetCheck () == 1)
			{
				SetTimer(1, TIMER1, NULL);		//启动定时器1,定时时间是1秒
				SetTimer(2, TIMER2, NULL);		//启动定时器1,定时时间是0.8毫秒

				m_Scan_Sn.EnableWindow ();
				m_Check2.EnableWindow ();
			}
			m_Check2.SetCheck (0);
		}
		else if (ID == IDC_CHECK2)
		{
			if (m_Check2.GetCheck () == 1)
				;
			else if (m_Check2.GetCheck () == 0)
				;
		}
	}

	if (m_Check1.GetCheck () == 1)
	{
		m_Scan_Sn.SetFocus ();
	}
	
}

void WorkDlg::EnableWindows (BOOL b)
{
	m_Scan_Sn.EnableWindow (b);
	m_Check1.EnableWindow (b);
	m_Check2.EnableWindow (b);
	m_ButtonOk.EnableWindow (b);
	//m_Check3.EnableWindow (b);
}

int t = 0;
SYSTEMTIME time1; 
SYSTEMTIME time2; 

void WorkDlg::OnEnChangeEditScanSn()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	t = 1;
	GetLocalTime (&time1);
}


/*计时器消息*/
void WorkDlg::OnTimer(UINT nIDEvent)//定时器消息 1秒
{
	if (nIDEvent == 1)//读取扫码枪数据用
	{
		GetLocalTime (&time2);
		if (t == 0)
		{
			CString str;
			m_Scan_Sn.GetWindowTextA (str);
			if (str.GetLength () > 0)
			{
				int n1 = time1.wSecond * 1000 + time1.wMilliseconds;
				int n2 = time2.wSecond * 1000 + time2.wMilliseconds;
				if (n2 - n1 > 800)//确保扫码完成后再执行
				{
					if (m_Check1.GetCheck () == 1)//勾选哪个就填充哪个的数据
					{

						m_Sn.SetWindowTextA (str);
						str = "";
					
					}
					m_Scan_Sn.SetWindowTextA ("");

					OnBnClickedButton1();
					EnableWindows (FALSE);
//					SetEvent (m_Event);
				}
				else
				{
					
				}
			}
		}
	}
	if (nIDEvent == 2)//重置位(扫码枪数据用)
	{
		t = 0;
	}
}

int WorkDlg::DoRun ()
{
	CString SN;
	if (m_Sn.GetWindowTextLengthA () <= 0)
	{
		AfxMessageBox ("请输入序列号");	
		return 0;
	}
//	socket_client();

	BOOL b = socket_client();
	if(!b)
	{
		AfxMessageBox ("连接失败，请重试！");	
		return 0;
	}

	return 0;
	
}
void WorkDlg::OnBnClickedButton1()
{

	// TODO: 在此添加控件通知处理程序代码
	EnableWindows (FALSE);	
	m_Scan_Sn.SetFocus ();
	SetEvent (m_Event);			//使事件变为有信号，即通知子线程运行
				
	
}


DWORD WINAPI WorkDlg::ThreadFunc(LPVOID lpParam)
{
	WorkDlg *pDlg = (WorkDlg *)lpParam;
	while (1)
	{
		WaitForSingleObject(pDlg->m_Event, INFINITE);	//阻塞等待该对象变为已通知状态
		CString ErrMessage;	//错误信息
		CString Result;			//测试结果（只能传NG/OK）
		CString ErrCode = " ";		//错误代码 当测试为“NG”时需要传入不良代码，多个不良代码之间用英文逗号隔开
//		CString ResCode = "BZ02";		//iResCode是途程中该工序对应的资源代码，非空
//		CString Operator = "160421261";		//iOperator需要传入操作人员的用户代码，非空
		//Sleep (3000);

		CString SN,iPERF;
		
		WaitForSingleObject(pDlg->m_Event_UDP, INFINITE);   //等待客户端准备好
		int nRet = 0;
		nRet = pDlg->DoRun ();

		pDlg->m_Sn.GetWindowTextA(SN); ////////////////////////////////有可能出错///////////

		if(lFtd.rtSN == 0)
		{
			Result = "NG";
			ErrCode = "L001";
		}
		if(lFtd.rtMAC == 0)
		{
			Result = "NG";
			if(ErrCode != " ")
			{				
				ErrCode = ErrCode + ",L002";
			}
			else
				ErrCode = "L002";
		}
		if(lFtd.rtSD == 0)
		{
			Result = "NG";
			if(ErrCode != " ")
			{
				ErrCode = ErrCode + ",L003";
			}
			else
				ErrCode = "L003";
		}
		if(lFtd.rtSPK == 0)
		{
			Result = "NG";
			if(ErrCode != " ")
			{
				ErrCode = ErrCode + ",L004";
			}
			else
				ErrCode = "L004";
		}
		if(lFtd.rtMic == 0)
		{
			Result = "NG";
			if(ErrCode != " ")
			{
				ErrCode = ErrCode + ",L005";
			}
			else
				ErrCode = "L005";
		}
		if(lFtd.rtWifi == 0)
		{
			Result = "NG";
			if(ErrCode != " ")
			{
				ErrCode = ErrCode + ",L006";
			}
			else
				ErrCode = "L006";
		}
		pDlg->m_Iperf.GetWindowTextA(iPERF);
		int i = _ttoi(iPERF);
		if(i < 25)
		{
			Result = "NG";
			if(ErrCode != " ")
			{
				ErrCode = ErrCode + ",L007";
			}
			else
				ErrCode = "L007";
		}
		if(ErrCode == " ")
		{
			Result = "OK";
		}

		//在界面上显示良品/不良品
		if(Result == "NG")
		{
			pDlg->m_Tips.SetWindowTextA ("NG");
			printf("错误代码：%s\n",ErrCode);
		}
		else 
		{
			pDlg->m_Tips.SetWindowTextA ("OK");
		}

		//检查SN是否按照途程生产
		BOOL b = MesDLL::GetInstance().CheckRoutePassed(SN,pDlg->m_Res,ErrMessage);																																
		if(!b)
		{
			AfxMessageBox ("该序列号没有按照途程生产！");
			
		}
		else
		{
			//测试数据传入MES系统
			b = MesDLL::GetInstance().SetMobileData(SN,pDlg->m_Res,pDlg->m_UserCode,Result,ErrCode,ErrMessage);																																
			if(!b)
			{
				pDlg->m_Tips.SetWindowTextA ("NG");
				AfxMessageBox ("上传MES系统失败！");			
			}
		}

		::SendMessage (pDlg->m_MainWnd->m_hWnd, WM_GETMACCOMPLETE, NULL, NULL);
	}
	return 0;
}

LRESULT WorkDlg::OnGetMacComplete(WPARAM wParam, LPARAM lParam)
{
//	m_Tips.SetWindowTextA ("操作完成!");
	EnableWindows (TRUE);
	m_Scan_Sn.SetFocus ();

	return 1;
}

void WorkDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString ErrMessage;	//错误信息
	BOOL b = MesDLL::GetInstance().ATELogOut(m_Res,ErrMessage);
	if(!b)
	{
		AfxMessageBox ("登出失败");
	}
	WorkDlg::EndDialog( IDC_BUTTON2 );
}
