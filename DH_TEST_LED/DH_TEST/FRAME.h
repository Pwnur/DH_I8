#pragma once


// FRAME �Ի���

class FRAME : public CDialogEx
{
	DECLARE_DYNAMIC(FRAME)

public:
	FRAME(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~FRAME();

	CWnd *m_FrameWnd;

// �Ի�������
	enum { IDD = IDD_FRAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
