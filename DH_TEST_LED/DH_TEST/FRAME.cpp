// FRAME.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DH_TEST.h"
#include "FRAME.h"
#include "afxdialogex.h"


// FRAME �Ի���

IMPLEMENT_DYNAMIC(FRAME, CDialogEx)

FRAME::FRAME(CWnd* pParent /*=NULL*/)
	: CDialogEx(FRAME::IDD, pParent)
{

}

FRAME::~FRAME()
{
}

void FRAME::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(FRAME, CDialogEx)
END_MESSAGE_MAP()


// FRAME ��Ϣ�������
