#pragma once
#include "afxwin.h"


// CMessageBoxDlg �Ի���

class CMessageBoxDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMessageBoxDlg)

public:
	CMessageBoxDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMessageBoxDlg();

// �Ի�������
	enum { IDD = IDD_MESSAGEBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_title;
	CString m_content;
	CString m_category;
	SOCKET m_MainSocket;
	afx_msg void OnBnClickedSendmessagebox();
};
