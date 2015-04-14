#pragma once
#include "afxwin.h"
#include "maizangDlg.h"

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
	CMaizangDlg *pMaizangDlg;
	afx_msg void OnBnClickedSendmessagebox();
	afx_msg void OnBnClickedAllSendmessagebox();
	void setAllSendButton(BOOL b);
	void setSendButton(BOOL b);
};
