#pragma once
#include "afxcmn.h"

// CProcess 对话框

class CProcess : public CDialogEx
{
	DECLARE_DYNAMIC(CProcess)

public:
	CProcess(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProcess();
	SOCKET m_MainSocket;
	// 对话框数据
	enum { IDD = IDD_PROCESS };

	void processListShow();

protected:
	// Generated message map functions
	//{{AFX_MSG(CFileManage)
	virtual BOOL OnInitDialog();
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);//当右击列表处应该起到的反应
	//}}AFX_MSG

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private :

public:
	CListCtrl m_ProcessListCtrl;	
	afx_msg void OnCloseprocess();
	afx_msg void OnFresh();
};
