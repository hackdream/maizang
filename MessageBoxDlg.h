#pragma once
#include "afxwin.h"


// CMessageBoxDlg 对话框

class CMessageBoxDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMessageBoxDlg)

public:
	CMessageBoxDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMessageBoxDlg();

// 对话框数据
	enum { IDD = IDD_MESSAGEBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_title;
	CString m_content;
	CString m_category;
	SOCKET m_MainSocket;
	afx_msg void OnBnClickedSendmessagebox();
};
