#pragma once
#include "afxcmn.h"


// CWindowManagerDlg �Ի���

class CWindowManagerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWindowManagerDlg)

public:
	CWindowManagerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWindowManagerDlg();
    SOCKET m_MainSocket;
// �Ի�������
	enum { IDD = IDD_WINDOW_MANAGER };

   void windowListShow();

protected:
	// Generated message map functions
	//{{AFX_MSG(CFileManage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
		
	DECLARE_MESSAGE_MAP()
private :
	
public:
	CListCtrl m_WindowListCtrl;	
};