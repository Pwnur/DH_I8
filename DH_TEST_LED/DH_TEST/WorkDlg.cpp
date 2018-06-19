// WorkDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DH_TEST.h"
#include "WorkDlg.h"
#include "afxdialogex.h"

#define TIMER1 1000
#define TIMER2 80

// WorkDlg �Ի���
void printFD(FACTORYTEST_DATA lFtD, char* str);

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
	��_ExePath.Format(_T("%s"), tempExePath);
	��_ExePath = ��_ExePath.Left(��_ExePath.ReverseFind('\\')+1) ;

	/*
	HANDLE CreateEvent(
		LPSECURITY_ATTRIBUTES lpEventAttributes,   // ��ȫ����
		BOOL bManualReset,   // ��λ��ʽ  //ΪFALSE�����ֶ�ResetEvent()
		BOOL bInitialState,   // ��ʼ״̬
		LPCTSTR lpName   // ��������
		);
*/
	m_Event = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_Event2 = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_Event_UDP = CreateEvent(NULL, FALSE, FALSE, NULL);
	DWORD dw;
	CreateThread(NULL, 0, ThreadFunc, this, 0, &dw);
	DWORD dw2;
	CreateThread(NULL, 0, ThreadFunc2, this, 0, &dw2);

	m_UserCode = UserCode;
	m_Res = Res;
	m_PassWord = PassWord;
//	m_pLogin = pLogin;
	
}

WorkDlg::~WorkDlg()
{
	BOOL b = DBDLL::GetInstance().EXITMES(m_Res);
}

void WorkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_IP);
	DDX_Control(pDX, IDC_EDIT_SCAN_SN, m_Scan_Sn);
	DDX_Control(pDX, IDC_EDIT_SN, m_Sn);
	DDX_Control(pDX, IDC_EDIT_TIPS, m_Tips);

	DDX_Control(pDX, IDC_BUTTON3, m_StaticLED);	
	DDX_Control(pDX, IDC_BUTTON4, m_StaticPICTURE);
	DDX_Control(pDX, IDC_BUTTON1, m_ButtonOK);
	DDX_Control(pDX, IDC_IR, m_ButtonIR);

	DDX_Control(pDX, IDC_LIGHT_RES, m_Buttonlight);	
	DDX_Control(pDX, IDC_PICTURE_RES, m_Buttonpicture);
	DDX_Control(pDX, IDC_IR_RES, m_Buttonir);
	DDX_Control(pDX, IDC_SPAKE_RES, m_Buttonspeak);

	DDX_Control(pDX, IDC_SN_TIPS, m_SnTips);
	DDX_Control(pDX, IDC_STATIC_KEY, m_Key);
	DDX_Control(pDX, IDC_STATIC_SN, m_ResSn);
	DDX_Control(pDX, IDC_STATIC_MAC, m_ResMac);
	DDX_Control(pDX, IDC_STATIC_VERSION, m_Version);
	DDX_Control(pDX, IDC_STATIC_VERSIONRES, m_ResVersion);


	DDX_Control(pDX, IDC_CHECKSELECT, m_Check_SELECT);
}


BEGIN_MESSAGE_MAP(WorkDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_MESSAGE(WM_GETMACCOMPLETE, OnGetMacComplete)
	ON_BN_CLICKED(IDC_LED_RED, &WorkDlg::OnBnClickedLedRed)
	ON_BN_CLICKED(IDC_LED_RED, &WorkDlg::OnBnClickedLedRed)
	ON_BN_CLICKED(IDC_LED_GREEN, &WorkDlg::OnBnClickedLedGreen)
	ON_BN_CLICKED(IDC_LED_BLUE, &WorkDlg::OnBnClickedLedBlue)
	ON_BN_CLICKED(IDC_LED_IR, &WorkDlg::OnBnClickedLedIr)
	ON_BN_CLICKED(IDC_IRCUT, &WorkDlg::OnBnClickedIrcut)
	ON_BN_CLICKED(IDC_upload, &WorkDlg::OnBnClickedupload)
	ON_BN_CLICKED(IDC_EXIT, &WorkDlg::OnBnClickedExit)
	ON_BN_CLICKED(IDC_CHECKSELECT, &WorkDlg::OnBnClickedCheckSelect)
	ON_BN_CLICKED(IDC_SEND, &WorkDlg::OnBnClickedSend)
	ON_BN_CLICKED(IDC_SPK, &WorkDlg::OnBnClickedSpk)
	ON_BN_CLICKED(IDC_LIGHT_RES, &WorkDlg::OnBnClickedLightRes)
	ON_BN_CLICKED(IDC_PICTURE_RES, &WorkDlg::OnBnClickedPictureRes)
	ON_BN_CLICKED(IDC_IR_RES, &WorkDlg::OnBnClickedIrRes)
	ON_BN_CLICKED(IDC_SPAKE_RES, &WorkDlg::OnBnClickedSpakeRes)
	ON_BN_CLICKED(IDC_KEY, &WorkDlg::OnBnClickedKey)
	ON_BN_CLICKED(IDC_IR, &WorkDlg::OnBnClickedIr)
END_MESSAGE_MAP()


// WorkDlg ��Ϣ�������
BOOL WorkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Check_SELECT.SetCheck (1);
	m_Scan_Sn.SetFocus ();

	SetTimer(1, TIMER1, NULL);//������ʱ��1,��ʱʱ����1��
	SetTimer(2, TIMER2, NULL);//������ʱ��1,��ʱʱ����1��

	iRES.light = 0;
	iRES.picture = 0;
	iRES.ir = 0;
	iRES.speak = 0;


	m_StaticLED.EnableWindow (FALSE);
	m_StaticPICTURE.EnableWindow (FALSE);

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

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);//������

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	AllocConsole();
	freopen( "CONOUT$","w",stdout);
	m_MainWnd = AfxGetThread()->GetMainWnd();

	font.CreatePointFont(880, _T("����"));
	GetDlgItem(IDC_EDIT_TIPS)->SetFont(&font);

	ResFont.CreatePointFont(200, _T("����"));
	TestFont.CreatePointFont(100, _T("����"));
	GetDlgItem(IDC_STATIC_KEY)->SetFont(&TestFont);
	GetDlgItem(IDC_LIGHT_RES)->SetFont(&ResFont);
	GetDlgItem(IDC_PICTURE_RES)->SetFont(&ResFont);
	GetDlgItem(IDC_IR_RES)->SetFont(&ResFont);
	GetDlgItem(IDC_SPAKE_RES)->SetFont(&ResFont);
	GetDlgItem(IDC_BUTTON3)->SetFont(&TestFont);
	GetDlgItem(IDC_BUTTON4)->SetFont(&TestFont);


	return FALSE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

//���ػس����ĺ���
/*
BOOL WorkDlg::PreTranslateMessage(MSG* pMsg)   
{  
    // ˳�δ��ݽ���,���ǵ�ǰ������ȷ����ť��ʱ����Ӧ������Ϣ  
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
     if( IDC_EDIT_TIPS  ==  pWnd->GetDlgCtrlID()) //�жϷ�����Ϣ�Ŀռ��Ƿ��Ǹþ�̬�ı���
     {
		 CString str;
		 m_Tips.GetWindowTextA(str);
		 if( str == "NG")
		 {
			pDC->SetTextColor(RGB(255,0,0));//�����ı���ɫΪ��ɫ
//         hbr=CreateSolidBrush(RGB(240, 240, 240));//�ؼ��ı���ɫΪ��ɫ
		 }
		 else
		 {
			pDC->SetTextColor(RGB(0,240,0));
		 }
     }
	 if( IDC_SN_TIPS  ==  pWnd->GetDlgCtrlID()) //�жϷ�����Ϣ�Ŀռ��Ƿ��Ǹþ�̬�ı���
     {
		pDC->SetTextColor(RGB(255,0,0));//�����ı���ɫΪ��ɫ
     }
     return hbr;//���������ػ�ؼ������Ļ�ˢ
}

void* g_ffPlayer;

void WorkDlg::TestResult()
{
	printf("SN:%d,MAC:%d,SD:%d,SPK:%d,MIC:%d,WIFI:%d,CAMERA:%d,IR:%d,IRCUT:%d,LIGHTSENSOR:%d,LED:%d,KEY:%d,VERSION:%d\n",lFtd.rtSN,lFtd.rtMAC,lFtd.rtSD,lFtd.rtSPK,lFtd.rtMic,lFtd.rtWifi,lFtd.rtCamera,lFtd.rtIR,lFtd.rtIRCut,lFtd.rtLightSensor,lFtd.rtLED,lFtd.rtKey,lFtd.rtVersion);
	if(lFtd.rtSN==0||lFtd.rtMAC==0||lFtd.rtSD==0||lFtd.rtSPK==0||lFtd.rtMic==0||lFtd.rtWifi==0||lFtd.rtCamera==0||lFtd.rtIR==0||lFtd.rtIRCut==0||lFtd.rtLightSensor==0||lFtd.rtLED==0||lFtd.rtKey==0||lFtd.rtVersion==0)
	{
		m_Tips.SetWindowTextA("NG");
	}
	else
	{
		m_Tips.SetWindowTextA("OK");
	}
}

BOOL WorkDlg::GetVersion ()
{
	CString filename;
	filename = ��_ExePath + "version.txt";
	CFile VerFile;
	CFileException fileException;

	if (!PathFileExists(filename))//д��־�ļ�
	{
		AfxMessageBox ("version.txt�����ڣ�");
		return FALSE;
	}
	VerFile.Open(filename,CFile::typeText|CFile::modeReadWrite);
	VerFile.Read(&iVersion,sizeof(iVersion));

	m_Version.SetWindowTextA(iVersion);

	memset(&lFtd.VER,0,32);
	memcpy(&lFtd.VER, iVersion, 32);


	VerFile.Close();
	return TRUE;
}


DWORD WINAPI WorkDlg::ThreadFunc2(LPVOID lpParam)
{

	WorkDlg *pDlg = (WorkDlg *)lpParam;

	SOCKET socket1;
	WSADATA wsaData;
	NOTIFY_MSG msg;

	DWORD g_dwOldAddress = 0;
	DWORD g_dwNewAddress = 0;
	char cNewIP[32];


	struct sockaddr_in local;
	struct sockaddr_in from;
	int fromlen =sizeof(from);
	local.sin_family=AF_INET;
	local.sin_port=htons(8989); ///�����˿�
	local.sin_addr.s_addr=INADDR_ANY; ///����
	socket1=socket(AF_INET,SOCK_DGRAM,0);

	int nNetTimeout=5000;//1��
	//����ʱ��
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
				char cmd[128];

				PLAYER_INIT_PARAMS params;
				memset(&params, 0, sizeof(params));
				sprintf(cmd, "rtsp://%s:8554//main", cNewIP);
				//strcpy(cmd, "rtsp://184.72.239.149/vod/mp4://BigBuckBunny_175k.mov");
				printf("play %s\r\n", cmd);
				if(g_ffPlayer)
					player_close(g_ffPlayer);
				g_ffPlayer = player_open(cmd, pDlg->GetDlgItem(IDC_FRAME)->GetSafeHwnd(), &params);
				//g_ffPlayer = player_open(cmd, pDlg->m_MainWnd->m_hWnd, &params);
				//g_ffPlayer = player_open(cmd, pDlg->GetSafeHwnd(), &params);

				g_dwOldAddress = g_dwNewAddress;
				SetEvent (pDlg->m_Event_UDP);
				printf("Get New Device ADDR\n");
			}
		}
		else
		{
			g_dwNewAddress = g_dwOldAddress = 0;
			//AfxMessageBox("udp recv err");
			pDlg->m_Buttonlight.SetWindowTextA("NG");
			pDlg->m_Buttonpicture.SetWindowTextA("NG");
			pDlg->m_Buttonir.SetWindowTextA("NG");
			pDlg->m_Buttonspeak.SetWindowTextA("NG");
			iRES.light = 0;
			iRES.picture = 0;
			iRES.ir = 0;
			iRES.speak = 0;
			printf("�����ѶϿ�\n");
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

BOOL WorkDlg::socket_client()
{ 
    char sendbuf[256];
	char revbuf[256];
	memset(sendbuf,0,256);
	memset(revbuf,0,256);

    struct hostent *he;
    struct sockaddr_in their_addr;
    unsigned int myport;

    myport = 7838;

	//��ȡip��ַ
	CString ip;
	m_IP.GetWindowTextA(ip);
	printf(ip.GetBuffer());

	

    if((he=gethostbyname(ip.GetBuffer()))==NULL) 
	{
        AfxMessageBox("gethostbyname error");
		closesocket(sockfd);
		return FALSE;
    }
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) 
	{
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
		AfxMessageBox("��ȡMACʧ��");
		return FALSE;
	}

	b = GetVersion();
	if(!b)
	{
		AfxMessageBox("��ȡ�汾��ʧ��");
		return FALSE;
	}


	memset(&lFtd.testSN, 0, 6);
	str3 = "22";
	memcpy(&lFtd.testSN, str3.GetBuffer(), 2);
	printf("the test is:%d\n",lFtd.testSN);
	lFtd.testVersion = '1';
	lFtd.rtWifi = '1';

	if (send(sockfd, (const char *)&lFtd, sizeof(lFtd), 0) == -1) 
	{
		AfxMessageBox("send error");
		closesocket(sockfd);
		return FALSE;
	}

    if ((numbytes=recv(sockfd, (char *)&lFtd, sizeof(lFtd), 0)) == -1) 
	{
        AfxMessageBox("recv error");
        
		closesocket(sockfd);
		return FALSE;
    } 
	if(lFtd.rtSN == 1)
	{
		m_ResSn.SetWindowTextA("SN:OK");
	}
	else
	{
		m_ResSn.SetWindowTextA("SN:NG");
	}
	if(lFtd.rtMAC == 1)
	{
		m_ResMac.SetWindowTextA("MAC:OK");
	}
	else
	{
		m_ResMac.SetWindowTextA("MAC:NG");
	}
	if(lFtd.rtVersion == 1)
	{
		m_ResVersion.SetWindowTextA("�汾�ţ�OK");
	}
	else
	{
		m_ResVersion.SetWindowTextA("�汾�ţ�NG");
	}
	TestResult();
	return TRUE;					
}
void WorkDlg::OnBnClickedCheckSelect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_Scan_Sn.SetFocus ();
	if (m_Check_SELECT.GetCheck () == 1)
	{
		m_Scan_Sn.SetFocus ();
	}
	
	UINT ID = LOWORD(GetCurrentMessage()->wParam);
	{
		if (ID == IDC_CHECK1)
		{
			if (m_Check_SELECT.GetCheck () == 0)
			{
				KillTimer (1);
				KillTimer (2);
				m_Scan_Sn.EnableWindow (FALSE);
			}
			else if (m_Check_SELECT.GetCheck () == 1)
			{
				SetTimer(1, TIMER1, NULL);		//������ʱ��1,��ʱʱ����1��
				SetTimer(2, TIMER2, NULL);		//������ʱ��1,��ʱʱ����0.8����

				m_Scan_Sn.EnableWindow ();
			}
		}
		else if (ID == IDC_CHECK2)
		{

		}
	}

	if (m_Check_SELECT.GetCheck () == 1)
	{
		m_Scan_Sn.SetFocus ();
	}
}

void WorkDlg::EnableWindows (BOOL b)
{
	m_Scan_Sn.EnableWindow (b);
	m_Sn.EnableWindow (b);

	m_ButtonRED.EnableWindow (b);
	m_ButtonGREEN.EnableWindow (b);
	m_ButtonBLUE.EnableWindow (b);
	m_ButtonIR.EnableWindow (b);
	m_ButtonIRCUT.EnableWindow (b);
//	m_ButtonSPK.EnableWindow (b);

	m_Check_SELECT.EnableWindow (b);
	m_Check_RED.EnableWindow (b);
	m_Check_GREEN.EnableWindow (b);
	m_Check_BLUE.EnableWindow (b);
	m_Check_IR.EnableWindow (b);
	m_Check_IRCUT.EnableWindow (b);
	m_Check_SPK.EnableWindow (b);

}

int t = 0;
SYSTEMTIME time1; 
SYSTEMTIME time2; 

void WorkDlg::OnEnChangeEditScanSn()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	t = 1;
	GetLocalTime (&time1);
}


/*��ʱ����Ϣ*/
void WorkDlg::OnTimer(UINT nIDEvent)//��ʱ����Ϣ 1��
{
	if (nIDEvent == 1)//��ȡɨ��ǹ������
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
				if (n2 - n1 > 800)//ȷ��ɨ����ɺ���ִ��
				{					
					m_Sn.SetWindowTextA (str);
					str = "";					
					m_Scan_Sn.SetWindowTextA ("");

//					OnBnClickedupload();
//					EnableWindows (FALSE);
					SetEvent (m_Event);
				}
				else
				{
					
				}
			}
		}
	}
	if (nIDEvent == 2)//����λ(ɨ��ǹ������)
	{
		t = 0;
	}
}

int WorkDlg::DoRun ()
{
	CString SN,ErrMessage;
	if (m_Sn.GetWindowTextLengthA () <= 0)
	{
		m_SnTips.SetWindowTextA("��ɨ��SN");	
		WaitForSingleObject(m_Event, INFINITE);	//�����ȴ��ö����Ϊ��֪ͨ״̬
		//return 0;
	}
//	socket_client();
	//���SN�Ƿ���;������
	m_Sn.GetWindowTextA(SN);
	BOOL b = MesDLL::GetInstance().CheckRoutePassed(SN,m_Res,ErrMessage);																																
	if(!b)
	{
		AfxMessageBox ("�����кŲ����ڵ�ǰ����");
		m_Tips.SetWindowTextA("NG");
		return FALSE;
			
	}
	
	b = socket_client();
	if(!b)
	{
		AfxMessageBox ("����ʧ�ܣ������ԣ�");
		m_Tips.SetWindowTextA("NG");
		return FALSE;
	}

	return TRUE;
	
}

DWORD WINAPI WorkDlg::ThreadFunc(LPVOID lpParam)
{
	WorkDlg *pDlg = (WorkDlg *)lpParam;
	while (1)
	{
//WaitForSingleObject(pDlg->m_Event, INFINITE);	//�����ȴ��ö����Ϊ��֪ͨ״̬
		CString ErrMessage;	//������Ϣ
		CString Result;			//���Խ����ֻ�ܴ�NG/OK��
		CString ErrCode = " ";		//������� ������Ϊ��NG��ʱ��Ҫ���벻�����룬�����������֮����Ӣ�Ķ��Ÿ���
//		CString ResCode = "BZ02";		//iResCode��;���иù����Ӧ����Դ���룬�ǿ�
//		CString Operator = "160421261";		//iOperator��Ҫ���������Ա���û����룬�ǿ�
		//Sleep (3000);

		CString SN;
		
		WaitForSingleObject(pDlg->m_Event_UDP, INFINITE);
//		WaitForSingleObject(pDlg->m_Event, INFINITE);	//�����ȴ��ö����Ϊ��֪ͨ״̬
		int nRet = 0;
		nRet = pDlg->DoRun ();
		
//		WaitForSingleObject(pDlg->m_Event2, INFINITE);
/*		
		if(nRet == 1)
		{
			pDlg->m_Sn.GetWindowTextA(SN); ////////////////////////////////�п��ܳ���///////////

			if(lFtd.rtSN == 0)
			{
				Result = "NG";
				ErrCode = "I001";
			}
			if(lFtd.rtMAC == 0)
			{
				Result = "NG";
				if(ErrCode != " ")
			{				
				ErrCode = ErrCode + ",I002";
			}
			else
				ErrCode = "I002";
			}
			if(pDlg->m_Check_RED.GetCheck () == 0)
			{
				Result = "NG";
				if(ErrCode != " ")
				{
					ErrCode = ErrCode + ",I003";
				}
				else
					ErrCode = "I003";
			}
			if(pDlg->m_Check_GREEN.GetCheck () == 0)
			{
				Result = "NG";
				if(ErrCode != " ")
				{
					ErrCode = ErrCode + ",I004";
				}
				else
					ErrCode = "I004";
			}
			if(pDlg->m_Check_BLUE.GetCheck () == 0)
			{
				Result = "NG";
				if(ErrCode != " ")
				{
					ErrCode = ErrCode + ",I005";
				}
				else
					ErrCode = "I005";
			}
			if(pDlg->m_Check_IR.GetCheck () == 0)
			{
				Result = "NG";
				if(ErrCode != " ")
				{
					ErrCode = ErrCode + ",I006";
				}
				else
					ErrCode = "I006";
			}
			if(pDlg->m_Check_IRCUT.GetCheck () == 0)
			{
				Result = "NG";
				if(ErrCode != " ")
				{
					ErrCode = ErrCode + ",I007";
				}
				else
					ErrCode = "I007";
			}
			if(lFtd.rtKey == 0)
			{
				Result = "NG";
				if(ErrCode != " ")
				{
					ErrCode = ErrCode + ",I007";
				}
				else
					ErrCode = "I007";
			}
			if(ErrCode == " ")
			{
				Result = "OK";
			}
			
			
			//�������ݴ���MESϵͳ
			BOOL b = MesDLL::GetInstance().SetMobileData(SN,pDlg->m_Res,pDlg->m_UserCode,Result,ErrCode,ErrMessage);																																
			if(!b)
			{
				pDlg->m_Tips.SetWindowTextA("NG");
				
			}
			else if(ErrCode == " ")
			{
				pDlg->m_Tips.SetWindowTextA("OK");
			}
			else
			{
				pDlg->m_Tips.SetWindowTextA("NG");
			}
		}
		::SendMessage (pDlg->m_MainWnd->m_hWnd, WM_GETMACCOMPLETE, NULL, NULL);*/
	}
	return 0;
}

LRESULT WorkDlg::OnGetMacComplete(WPARAM wParam, LPARAM lParam)
{
	CString res;
	m_Tips.GetWindowTextA (res);
	if(res == "OK")
	{
		FACTORYTEST_DATA Destroy;
		memset(&Destroy, 0, sizeof(Destroy));
		Destroy.MAGIC = SIG_MAGIC;
		Destroy.eixttest = 'a';

		printFD(Destroy, "destory");
		if (send(sockfd, (const char *)&Destroy, sizeof(Destroy), 0) == -1) 
		{
			AfxMessageBox("����ʧ�ܣ�");
		}
	}
	if(sockfd)
	{
		closesocket(sockfd);		//�Ͽ������ͨ��
		sockfd = 0;
	}

	EnableWindows (TRUE);
	m_Scan_Sn.SetFocus ();

	return 1;
}

void WorkDlg::OnBnClickedLedRed()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	FACTORYTEST_DATA TestLED;
	memset(&TestLED, 0, sizeof(TestLED));
	TestLED.MAGIC = SIG_MAGIC;
	CString str;


	str = "2";									//�����
	memcpy(&TestLED.testLED, str.GetBuffer(), 1);
	printf("socket is :%d\n",socket);
	if (send(sockfd, (const char *)&TestLED, sizeof(TestLED), 0) == -1) 
	{
		AfxMessageBox("����ʧ�ܣ�������!");
	}
	if ((numbytes=recv(sockfd, (char *)&TestLED, sizeof(TestLED), 0)) == -1) 
	{
        AfxMessageBox("recv error");
        
    }
	TestResult();
}


void WorkDlg::OnBnClickedLedGreen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	FACTORYTEST_DATA TestLED;
	memset(&TestLED, 0, sizeof(TestLED));
	TestLED.MAGIC = SIG_MAGIC;
	CString str;

	str = "4";									//���̵�
	memcpy(&TestLED.testLED, str.GetBuffer(), 1);
	printf("socket is :%d\n",socket);
	if (send(sockfd, (const char *)&TestLED, sizeof(TestLED), 0) == -1) 
	{
		AfxMessageBox("����ʧ�ܣ�������!");
	}
	if ((numbytes=recv(sockfd, (char *)&TestLED, sizeof(TestLED), 0)) == -1) 
	{
        AfxMessageBox("recv error");
        
    }
	TestResult();
}


void WorkDlg::OnBnClickedLedBlue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	FACTORYTEST_DATA TestLED;
	memset(&TestLED, 0, sizeof(TestLED));
	TestLED.MAGIC = SIG_MAGIC;
	CString str;

	str = "1";									//������
	memcpy(&TestLED.testLED, str.GetBuffer(), 1);
	printf("socket is :%d\n",socket);
	if (send(sockfd, (const char *)&lFtd, sizeof(TestLED), 0) == -1) 
	{
		AfxMessageBox("����ʧ�ܣ�������!");
	}
	if ((numbytes=recv(sockfd, (char *)&TestLED, sizeof(TestLED), 0)) == -1) 
	{
        AfxMessageBox("recv error");
        
    }
}


void WorkDlg::OnBnClickedLedIr()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	FACTORYTEST_DATA TestIR;
	memset(&TestIR, 0, sizeof(TestIR));
	TestIR.MAGIC = SIG_MAGIC;
	CString str;
	
	str = "22";									//�������
	memcpy(&lFtd.testIR, str.GetBuffer(), 2);
	printf("socket is :%d\n",socket);
	if (send(sockfd, (const char *)&TestIR, sizeof(TestIR), 0) == -1) 
	{
		AfxMessageBox("����ʧ�ܣ�������!");
	}
	if ((numbytes=recv(sockfd, (char *)&TestIR, sizeof(TestIR), 0)) == -1) 
	{
        AfxMessageBox("recv error");
        
    }
	TestResult();
}


void WorkDlg::OnBnClickedIrcut()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	FACTORYTEST_DATA TestIRCUT;
	memset(&TestIRCUT, 0, sizeof(TestIRCUT));
	TestIRCUT.MAGIC = SIG_MAGIC;
	CString str;
	
	str = "1";									//�����˾�
	memcpy(&TestIRCUT.testIRCut, str.GetBuffer(), 1);
	printf("socket ircut is :%d\n",socket);
	if (send(sockfd, (const char *)&TestIRCUT, sizeof(TestIRCUT), 0) == -1) 
	{
		AfxMessageBox("����ʧ�ܣ�������!");
	}
	if ((numbytes=recv(sockfd, (char *)&TestIRCUT, sizeof(TestIRCUT), 0)) == -1) 
	{
        AfxMessageBox("recv error");
        
    }
	TestResult();
}

/*
void WorkDlg::OnBnClickedSpk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	FACTORYTEST_DATA TestSPK;
	TestSPK.MAGIC = SIG_MAGIC;
	CString str;
	memset(&TestSPK.testSPK, 0, 1);
	str = "1";									//������&��
	memcpy(&lFtd.testSPK, str.GetBuffer(), 1);
	if (send(sockfd, (const char *)&TestSPK, sizeof(TestSPK), 0) == -1) 
	{
		AfxMessageBox("����ʧ�ܣ�������!");
	}
}
*/

void WorkDlg::OnBnClickedupload()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	EnableWindows (FALSE);	
	m_Scan_Sn.SetFocus ();
	SetEvent (m_Event_UDP);			//ʹ�¼���Ϊ���źţ���֪ͨ���߳�����
}


void WorkDlg::OnBnClickedExit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString ErrMessage;	//������Ϣ
	WSACleanup();
	BOOL b = MesDLL::GetInstance().ATELogOut(m_Res,ErrMessage);
	if(!b)
	{
		AfxMessageBox ("�ǳ�ʧ�ܣ�");
	}
	WorkDlg::EndDialog( IDC_BUTTON2 );
}





void WorkDlg::OnBnClickedSend()
{
/*	// TODO: �ڴ���ӿؼ�֪ͨ����������
	EnableWindows (FALSE);	
	m_Scan_Sn.SetFocus ();
	SetEvent (m_Event2);		//ʹ�¼���Ϊ���źţ���֪ͨ���߳�����
*/
	CString ErrMessage;	//������Ϣ
	CString Result;			//���Խ����ֻ�ܴ�NG/OK��
	CString ErrCode = " ";
	CString SN;
	m_Sn.GetWindowTextA(SN); ////////////////////////////////�п��ܳ���///////////

	if(lFtd.rtSN == 0)
	{
		Result = "NG";
		ErrCode = "I001";
		m_ResSn.SetWindowTextA("NG");
	}
	else
		m_ResSn.SetWindowTextA("OK");
	if(lFtd.rtMAC == 0)
	{
		Result = "NG";
		if(ErrCode != " ")
		{				
			ErrCode = ErrCode + ",I002";
		}
		else
			ErrCode = "I002";
		m_ResMac.SetWindowTextA("NG");
	}
	else
		m_ResMac.SetWindowTextA("OK");

	if(iRES.light == 0)
	{
		Result = "NG";
		if(ErrCode != " ")
		{
			ErrCode = ErrCode + ",I003";
		}
		else
			ErrCode = "I003";
	}
	if(iRES.light == 0)
	{
		Result = "NG";
		if(ErrCode != " ")
		{
			ErrCode = ErrCode + ",I004";
		}
		else
			ErrCode = "I004";
	}
	/*
	if(m_Check_BLUE.GetCheck () == 0)
	{
		Result = "NG";
		if(ErrCode != " ")
		{
			ErrCode = ErrCode + ",I005";
		}
		else
			ErrCode = "I005";
	}
	*/
	if(iRES.ir == 0)
	{
		Result = "NG";
		if(ErrCode != " ")
		{
			ErrCode = ErrCode + ",I006";
		}
		else
			ErrCode = "I006";
	}
	if(iRES.ir == 0)
	{
		Result = "NG";
		if(ErrCode != " ")
		{
			ErrCode = ErrCode + ",I007";
		}
		else
			ErrCode = "I007";
	}
	if(iRES.picture == 0)
	{
		Result = "NG";
		if(ErrCode != " ")
		{
			ErrCode = ErrCode + ",I007";
		}
		else
			ErrCode = "I007";
	}
	if(lFtd.rtKey == 0)
	{
		Result = "NG";
		if(ErrCode != " ")
		{
			ErrCode = ErrCode + ",I007";
		}
		else
			ErrCode = "I007";
		m_Key.SetWindowTextA("NG");
	}
	else
		m_Key.SetWindowTextA("OK");
	if(lFtd.rtVersion == 0)
	{
		Result = "NG";
		if(ErrCode != " ")
		{
			ErrCode = ErrCode + ",I007";
		}
		else
			ErrCode = "I007";
		m_ResVersion.SetWindowTextA("NG");
	}
	else
		m_ResVersion.SetWindowTextA("OK");
	if(ErrCode == " ")
	{
		Result = "OK";
	}
			
			
	//�������ݴ���MESϵͳ
	BOOL b = MesDLL::GetInstance().SetMobileData(SN,m_Res,m_UserCode,Result,ErrCode,ErrMessage);																																
	if(!b)
	{
		m_Tips.SetWindowTextA("NG");
				
	}
	else if(ErrCode == " ")
	{
		m_Tips.SetWindowTextA("OK");
	}
	else
	{
		m_Tips.SetWindowTextA("NG");
	}
	
	::SendMessage (m_MainWnd->m_hWnd, WM_GETMACCOMPLETE, NULL, NULL);
}

void printFD(FACTORYTEST_DATA lFtD, char* str)
{
	char buf[256];
	int cnt = 0;
	
	memcpy(buf, &lFtD.MAGIC, 64+32);
	printf("%s:\r\n", str);
	for(cnt=0; cnt<4; cnt++)
		printf("%02x ", buf[cnt]);
	printf("\r\n");
	for(; cnt<(4+16); cnt++)
		printf("%02x ", buf[cnt]);
	printf("\r\n");
	for(; cnt<(4+16+16); cnt++)
		printf("%02x ", buf[cnt]&0xFF);
	printf("\r\n");
	for(; cnt<(4+16+16+32); cnt++)
		printf("%02x", buf[cnt]&0xFF);
	printf("\r\n");
	for(; cnt<(4+16+16+32+14); cnt++)
		printf("%02x ", buf[cnt]&0xFF);
	printf("\r\n");
	for(; cnt<(4+16+16+32+14+15); cnt++)
		printf("%02x ", buf[cnt]&0xFF);
	printf("\r\n");		
}

void WorkDlg::OnBnClickedSpk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	FACTORYTEST_DATA TestSPK;
	memset(&TestSPK, 0, sizeof(TestSPK));
	TestSPK.MAGIC = SIG_MAGIC;
	
	TestSPK.testMic = '2';									//�����˾�
	printf("socket xxxxx is :%d\n",socket);
	
	if (send(sockfd, (const char *)&TestSPK, sizeof(TestSPK), 0) == -1) 
	{
		AfxMessageBox("����ʧ�ܣ�������!");
	}
	if ((numbytes=recv(sockfd, (char *)&TestSPK, sizeof(TestSPK), 0)) == -1) 
	{
        AfxMessageBox("recv error");       
    }
}


void WorkDlg::OnBnClickedLightRes()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	if(iRES.light == 1)
	{
		iRES.light = 0;
		lFtd.rtLED = 0;
		m_Buttonlight.SetWindowTextA("NG");
	}
		
	if(iRES.light == 0)
	{
		iRES.light = 1;
		lFtd.rtLED = 1;
		m_Buttonlight.SetWindowTextA("OK");
	}

	TestResult();
		
}


void WorkDlg::OnBnClickedPictureRes()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	if(iRES.picture==1)
	{
		iRES.picture = 0;
		lFtd.rtCamera = 0;
		m_Buttonpicture.SetWindowTextA("NG");
	}
	if(iRES.picture==0)
	{
		iRES.picture = 1;
		lFtd.rtCamera = 1;
		m_Buttonpicture.SetWindowTextA("OK");
	}
	TestResult();	
}


void WorkDlg::OnBnClickedIrRes()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(iRES.ir == 1)
	{
		iRES.ir = 0;
		lFtd.rtIR = 0;
		lFtd.rtIRCut = 0;
		m_Buttonir.SetWindowTextA("NG");
	}
		
	if(iRES.ir == 0)
	{
		iRES.ir = 1;
		lFtd.rtIR = 1;
		lFtd.rtIRCut = 1;
		m_Buttonir.SetWindowTextA("OK");
	}
	TestResult();	
}


void WorkDlg::OnBnClickedSpakeRes()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	if(iRES.speak == 1)
	{
		iRES.speak = 0;
		lFtd.rtSPK = 0;
		lFtd.rtMic = 0;
		m_Buttonspeak.SetWindowTextA("NG");
	}
		
	if(iRES.speak==0)
	{
		iRES.speak = 1;
		lFtd.rtSPK = 1;
		lFtd.rtMic = 1;
		m_Buttonspeak.SetWindowTextA("OK");
	}
	TestResult();	
}


void WorkDlg::OnBnClickedKey()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	FACTORYTEST_DATA TestKEY;
	memset(&TestKEY, 0, sizeof(TestKEY));
	TestKEY.MAGIC = SIG_MAGIC;
	CString str;
	
	str = "1";									//�����˾�
	memcpy(&TestKEY.testKey, str.GetBuffer(), 1);
	TestKEY.testLightSensor = '1';
	printf("socket is :%d\n",socket);
	if (send(sockfd, (const char *)&TestKEY, sizeof(TestKEY), 0) == -1) 
	{
		AfxMessageBox("����ʧ�ܣ�������!");
	}
	if ((numbytes=recv(sockfd, (char *)&TestKEY, sizeof(TestKEY), 0)) == -1) 
	{
        AfxMessageBox("recv error");
        
    }
	if(TestKEY.rtKey == 1)
	{
		if(TestKEY.rtLightSensor == 1)
		{
			m_Key.SetWindowTextA("������OK   ��У�OK");
		}
		else
		{
			m_Key.SetWindowTextA("������OK   ��У�NG");
		}
		
		lFtd.rtKey = 1;
		lFtd.rtLightSensor = 1;
	}		
	else
	{
		if(TestKEY.rtLightSensor == 1)
		{
			m_Key.SetWindowTextA("������NG   ��У�OK");
		}
		else
		{
			m_Key.SetWindowTextA("������NG   ��У�NG");
		}
		
		lFtd.rtKey = 0;
		lFtd.rtLightSensor = 0;
	}

	TestResult();	
}


void WorkDlg::OnBnClickedIr()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	FACTORYTEST_DATA TestIR;
	memset(&TestIR, 0, sizeof(TestIR));
	TestIR.MAGIC = SIG_MAGIC;
	CString str;
	
	str = "22";									//�������
	memcpy(&TestIR.testIR, str.GetBuffer(), 2);
	printf("socket is :%d\n",socket);
	if (send(sockfd, (const char *)&TestIR, sizeof(TestIR), 0) == -1) 
	{
		AfxMessageBox("����ʧ�ܣ�������!");
	}
	if ((numbytes=recv(sockfd, (char *)&TestIR, sizeof(TestIR), 0)) == -1) 
	{
        AfxMessageBox("recv error");
        
    }
	TestResult();
}
