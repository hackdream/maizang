#pragma once
#include "afxcmn.h"


// CWindowManagerDlg 对话框

class CWindowManagerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWindowManagerDlg)

public:
	CWindowManagerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWindowManagerDlg();
    SOCKET m_MainSocket;
// 对话框数据
	enum { IDD = IDD_WINDOW_MANAGER };

   void windowListShow();

protected:
	// Generated message map functions
	//{{AFX_MSG(CFileManage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
		
	DECLARE_MESSAGE_MAP()
private :
	
public:
	CListCtrl m_WindowListCtrl;	
};
