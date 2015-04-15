#pragma once
#include "afxcmn.h"

// CProcess �Ի���

class CProcess : public CDialogEx
{
	DECLARE_DYNAMIC(CProcess)

public:
	CProcess(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProcess();
	SOCKET m_MainSocket;
	// �Ի�������
	enum { IDD = IDD_PROCESS };

	void processListShow();

protected:
	// Generated message map functions
	//{{AFX_MSG(CFileManage)
	virtual BOOL OnInitDialog();
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);//���һ��б�Ӧ���𵽵ķ�Ӧ
	//}}AFX_MSG

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private :

public:
	CListCtrl m_ProcessListCtrl;	
	afx_msg void OnCloseprocess();
	afx_msg void OnFresh();
};
