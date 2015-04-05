

/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "afxwin.h"
#include <tlHelp32.h>
#include <vdmDbg.h>
// CProcess dialogz

//#include <winsock.h>
#define KEY_SREEN 0x1002
#define KEY_NO_CTRL 0x1003
#define KEY_CTRL_C 0x1004
#define KEY_CTRL_BRAK 0x1005

class CProcess :public CDialog{
	//DECLARE_DYNAMIC(CPrcoess)
public:
	CProcess(CWnd *pParent = NULL);
	//void SetConnSocket(SOCKET socket);
	enum{IDD=IDD_PROCESS};
	//CListCtrl m_CtrlProcessPropList;
	//CListCtrl m_CtrlProcessProp;
	CListCtrl m_CtrlProcessList;
	SOCKET m_ConnSocket;
protected:
	HICON m_hIcon;
	//CProcessEnumerator m_ProcessEnumViewer;
	virtual void DoDataExchange(CDataExchange *pDX);
	//DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nId, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnRefresh();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnItemchangedListProcess(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedListProperties(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType,int cx,int cy);
	afx_msg void OnEndtask();
	DECLARE_MESSAGE_MAP()

	void InitializeViews();

private:
	BOOL CheckOSVersion(DWORD &OSVersion);
};

unsigned __stdcall RecvProcessDataThread(void *);
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
