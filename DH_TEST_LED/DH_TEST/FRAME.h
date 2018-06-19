#pragma once


// FRAME 对话框

class FRAME : public CDialogEx
{
	DECLARE_DYNAMIC(FRAME)

public:
	FRAME(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~FRAME();

	CWnd *m_FrameWnd;

// 对话框数据
	enum { IDD = IDD_FRAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
