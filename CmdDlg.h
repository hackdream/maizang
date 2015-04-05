#pragma once
#include "afxwin.h"


// CCmdDlg 对话框

#define KEY_SREEN   0x1002
#define KEY_NO_CTRL   0x1003
#define KEY_CTRL_C   0x1004
#define KEY_CTRL_BRAK 0x1005

class CCmdDlg : public CDialog
{
	DECLARE_DYNAMIC(CCmdDlg)

public:
	CCmdDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCmdDlg();
	SOCKET m_MainSocket;
	UCHAR cmdBuffer[6001];
	CString Output;
	CString m_RemString;
	 

// 对话框数据
	enum { IDD = IDD_CMD_SHELL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Edit;
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString m_CommandString;
	afx_msg void OnBnClickedExecute();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnClose();
};

unsigned __stdcall   RecvCmdDataThread(void *);
 