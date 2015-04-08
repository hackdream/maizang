// FileManage.cpp : implementation file
//

#include "stdafx.h"
#include "maizang.h"
#include "FileManage.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileManage dialog


CFileManage::CFileManage(CWnd* pParent /*=NULL*/)
	: CDialog(CFileManage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileManage)
	//}}AFX_DATA_INIT
	m_CurrPath="";
	m_SendPath="";
	m_Buffer = new char[1536 * 1024];
}
CFileManage::~CFileManage()
{
	if(m_Buffer != NULL)
		delete []m_Buffer;
}

void CFileManage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileManage)
	DDX_Control(pDX, IDC_FILELIST, m_FileList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileManage, CDialog)
	//{{AFX_MSG_MAP(CFileManage)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_RDBLCLK, IDC_FILELIST, &CFileManage::OnNMRDblclkFilelist)
	ON_NOTIFY(NM_DBLCLK, IDC_FILELIST, &CFileManage::OnNMDblclkFilelist)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileManage message handlers

BOOL CFileManage::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	SetIcon(AfxGetApp()->LoadIcon(IDI_OL_FILE), FALSE);
	CenterWindow();
	if ( !m_wndToolBar.CreateEx(this, TBSTYLE_FLAT | TBSTYLE_LIST ,   //工具栏的创建
		WS_CHILD | WS_VISIBLE	| CBRS_ALIGN_TOP |CBRS_GRIPPER | CBRS_TOOLTIPS ,
		CRect(2, 2, 0, 0)) || !m_wndToolBar.LoadToolBar(IDR_TOOLBAR_FILE))
	{
		return FALSE;
	}

	m_wndToolBar.LoadTrueColorToolBar(
		16,
		IDB_FileToolBarNormal,
		IDB_FileToolBarHot,
		IDB_FileToolBarDisable
		);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0); //自动填充到非客户区的位置
	VERIFY(m_wndToolBar.SetButtonText(0,"向上"));   
	VERIFY(m_wndToolBar.SetButtonText(1,"复制"));   
	VERIFY(m_wndToolBar.SetButtonText(2,"粘贴"));
	VERIFY(m_wndToolBar.SetButtonText(3,"删除"));
	VERIFY(m_wndToolBar.SetButtonText(4,"上传"));   
	VERIFY(m_wndToolBar.SetButtonText(5,"下载"));   
	VERIFY(m_wndToolBar.SetButtonText(6,"刷新"));  
	VERIFY(m_wndToolBar.SetButtonText(7,"查看"));   

	m_wndStatusBar.Create(WS_CHILD|WS_VISIBLE|CCS_BOTTOM,  CRect(0,0,0,0),  this,  0x1100001);//状态栏的设置
	int strPartDim[2]= {400,-1};
	m_wndStatusBar.SetParts(2,strPartDim);


	//加载菜单====================================
	m_FileMenu.LoadMenu(IDR_MENU_FILE);

	//文件传输窗口中树形控件的初始化
	pSmallImage=m_FileList.GetImageList(LVSIL_SMALL);     //指向listctrl现在使用的ImageList
	pBigImage=m_FileList.GetImageList(LVSIL_NORMAL);  //指向listctrl现在使用的ImageList

	fileListImage.Create(18, 18, ILC_COLOR24 | ILC_MASK, 0, 7);
	HBITMAP hBitMap = (HBITMAP)LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_FOLDERS), IMAGE_BITMAP,
		0, 0, LR_DEFAULTSIZE | LR_CREATEDIBSECTION);
	fileListImage.Add(CBitmap::FromHandle(hBitMap), RGB(255, 255, 255));
	::DeleteObject(hBitMap);

	m_FileList.SetImageList(&fileListImage, TVSIL_NORMAL);
	//文件传输窗口中文件列表的初始化
	m_FileList.InsertColumn(0, ("文件名"), LVCFMT_LEFT, 100);
	m_FileList.InsertColumn(1, "类型", LVCFMT_LEFT, 100);
	m_FileList.InsertColumn(2, "大小", LVCFMT_LEFT, 90);
	m_FileList.InsertColumn(3, "修改时间", LVCFMT_LEFT, 130);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//让文件传输窗口显示状态信息标明属于哪台主机的文件列表 以及显示主机磁盘信息
void CFileManage::SetConnSocket(SOCKET socket)
{
	//客户端与服务端进行通信的套接字
	m_ConnSocket = socket ;
	sockaddr_in addr;
	int sz = sizeof(addr);
	//获取对端名称信息  通过返回值ret判定一些错误
	int ret = getpeername(m_ConnSocket, (sockaddr *)&addr, &sz); 

	CString OnlineIP;
	OnlineIP.Format("%s:%d",inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
	SetWindowText("文件管理" + OnlineIP);
	GetRootDrivers();
}

void CFileManage::GetRootDrivers()
{
	unsigned dwThreadId;
	m_hWorkThread =
		(HANDLE)_beginthreadex(NULL,				 
		0,					 
		ListDriver,  
		this,   
		0, 		 
		&dwThreadId);
	if(m_hWorkThread == NULL)
	{
		m_wndStatusBar.SetText("获取远程电脑磁盘信息失败", 0 ,0);
	}
}


unsigned  __stdcall  ListDriver(void * pParam)	
{
	CFileManage *This = (CFileManage*)pParam;  
	This->OnWorkBegin();
	This->m_MsgHead.dwCmd = CMD_FILEDRIVER;
	This->m_MsgHead.dwSize = 0;
	if(!SendMsg(This->m_ConnSocket, This->m_Buffer, &This->m_MsgHead) || 
		!RecvMsg(This->m_ConnSocket, This->m_Buffer, &This->m_MsgHead)  ) 
	{

		This->m_wndStatusBar.SetText("通信失败!", 0, 0);
		This->OnWorkEnd();
		return 0;
	}

	if(This->m_MsgHead.dwCmd != CMD_SUCCEED)
	{
		This->m_wndStatusBar.SetText("获取远程主机磁盘信息失败", 0, 0);
		This->OnWorkEnd();
		return 0;
	}

	//显示远程磁盘
	This->m_FileList.DeleteAllItems();

	DWORD dwNum = This->m_MsgHead.dwSize /sizeof(DriverInfo);
	BYTE * m_DesBuf = (LPBYTE) (This->m_Buffer);
	LPDriverInfo pInfo = (LPDriverInfo) m_DesBuf;
	int iImage =0, iSelectedImage=0;

	for(DWORD i = 0; i < dwNum; i++)
	{
		//添加盘符
		switch(pInfo[i].drivertype)
		{
		case DRIVE_REMOVABLE:// 可移动驱动器
			{
				iImage        = 4;
				iSelectedImage= 4;
			}
			break;
		case DRIVE_REMOTE:   // 网络驱动器
			{
				iImage        = 6;
				iSelectedImage= 6;
			}
			break;
		case DRIVE_CDROM:    // CD-ROM
			{
				iImage        = 5;
				iSelectedImage= 5;
			}
			break;
		case DRIVE_FIXED:    // 固定的驱动器
			{
				iImage        = 3;
				iSelectedImage= 3;
			}
			break;
		default://不认识的类型跳过，不显示
			{
				pInfo[i].display[1]=0;	
			}
			break;
		}
		//Win2000会接收到一些错误的盘符
		if (strlen(pInfo[i].display) < 2)
			continue;
		int insertPos = This->m_FileList.GetItemCount();
		This->m_FileList.InsertItem(insertPos, pInfo[i].driver, NULL);
		This->m_FileList.SetItemData(insertPos, (DWORD)pInfo[i].driver[0]);
		This->m_FileList.SetItemText(insertPos,1,"磁盘");
	}

	This->m_wndStatusBar.SetText("获取远程磁盘成功", 0, 0);
	This->OnWorkEnd();
	return 0;
}
void CFileManage::OnWorkBegin()
{
	m_FileList.EnableWindow(FALSE);//将列表设置为不可用 变灰
}
void CFileManage::OnWorkEnd()
{
	m_FileList.EnableWindow(TRUE);//将列表设置为不可用 变灰
	CloseHandle(m_hWorkThread);
	m_hWorkThread = NULL;
}

unsigned  __stdcall ListDirectory(void * pParam)
{
	CFileManage *This = (CFileManage*)pParam; 
	This->OnWorkBegin();

	//发送获取盘符命令
	This->m_MsgHead.dwCmd  = CMD_FILEDIRECTORY;
	This->m_MsgHead.dwSize = This->m_SendPath.GetLength();
	strcpy(This->m_Buffer,This->m_SendPath);

	//数据传输同时接收数据
	if( !SendMsg(This->m_ConnSocket, This->m_Buffer, &This->m_MsgHead) ||
		!RecvMsg(This->m_ConnSocket, This->m_Buffer, &This->m_MsgHead))
	{
		//数据传输失败
		This->m_wndStatusBar.SetText("通信失败", 0, 0);
		This->OnWorkEnd();
		return 0;
	}
	if(This->m_MsgHead.dwCmd != CMD_SUCCEED)
	{
		//数据传输失败
		if(This->m_MsgHead.dwCmd == CMD_DIRFLODERERR)
			This->m_wndStatusBar.SetText("目录不能访问", 0, 0); 
		else
			This->m_wndStatusBar.SetText("获取远程目录失败", 0, 0);  
		This->OnWorkEnd();
		return 0;
	}

	//显示文件列表
	This->m_FileList.DeleteAllItems();

	DWORD dwNum = This->m_MsgHead.dwSize /sizeof(FileInfo);
	BYTE * m_DesBuf = (LPBYTE)(This->m_Buffer);
	LPFileInfo pInfo = (LPFileInfo) m_DesBuf;
	for(DWORD i = 0; i < dwNum; i++)
	{
		//显示时候，文件夹在上面，文件在下面
		int iIcon = 0, iInsertItem = 0;
		if(pInfo[i].iType == 2)//文件
		{
			iIcon = This->GetIconIndex(pInfo[i].cFileName, FALSE);
			iInsertItem =This->m_FileList.GetItemCount();				
		}
		if(pInfo[i].iType == 1)//文件夹
		{
			iIcon = This->GetIconIndex(pInfo[i].cFileName, TRUE);
			strcpy(pInfo[i].cAttrib,"文件夹");
			iInsertItem = 0;
		}

		This->m_FileList.InsertItem(iInsertItem,pInfo[i].cFileName,iIcon);
		This->m_FileList.SetItemData(iInsertItem,pInfo[i].iType);
		This->m_FileList.SetItemText(iInsertItem,0,pInfo[i].cFileName);
		This->m_FileList.SetItemText(iInsertItem,1,pInfo[i].cAttrib);
		This->m_FileList.SetItemText(iInsertItem,2,pInfo[i].cSize);
		This->m_FileList.SetItemText(iInsertItem,3,pInfo[i].cTime);
	}
	This->m_wndStatusBar.SetText("获取远程文件列表成功", 0, 0);
	CString str;
	str.Format("共有文件、文件夹 %d个", dwNum);
	This->m_wndStatusBar.SetText(str, 1, 0);
	This->OnWorkEnd();

	return 0;
}

int CFileManage::GetIconIndex(LPCTSTR lpszPath, BOOL bIsDir, BOOL bSelected)
{
	SHFILEINFO sfi;
	memset(&sfi, 0, sizeof(sfi));

	SHGetFileInfo(lpszPath, 
		FILE_ATTRIBUTE_NORMAL,
		&sfi, sizeof(sfi),
		SHGFI_ICON|SHGFI_USEFILEATTRIBUTES|SHGFI_TYPENAME );  
	//indeximage=pSmallImage->Add(sfi.hIcon);//向ImageList里面添加图标
	//return  sfi.iIcon;
	return  3;
}


void CFileManage::OnNMRDblclkFilelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_FileList.GetFirstSelectedItemPosition(); // todo
	if(pos == NULL) return ;
	int iCurrSel= m_FileList.GetNextSelectedItem(pos);
	DWORD ch = m_FileList.GetItemData(iCurrSel);//ch保存选中的磁盘
	if (ch =='A' || ch=='B')//A,B盘不处理
		return;

	//设置当前目录
	m_CurrPath.Format("%c:",(char)ch);
	UpdateData(FALSE);
	//拼接列举路径
	m_SendPath = m_CurrPath + "\\*";
	//列举目录

	unsigned dwThreadId;
	m_hWorkThread  =
		(HANDLE)_beginthreadex(NULL,				 
		0,					 
		ListDirectory,  
		this,   
		0, 		 
		&dwThreadId);

	if (m_hWorkThread == NULL)
		m_wndStatusBar.SetText("获取远程目录列表失败", 0, 0);
	*pResult = 0;
}


void CFileManage::OnNMDblclkFilelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);	
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_FileList.GetFirstSelectedItemPosition(); // todo
	if(pos == NULL) return ;
	int iCurrSel= m_FileList.GetNextSelectedItem(pos);
	DWORD ch = m_FileList.GetItemData(iCurrSel);//ch保存选中的磁盘
	if (ch =='A' || ch=='B')//A,B盘不处理
		return;

	//设置当前目录
	m_CurrPath.Format("%c:",(char)ch);
	UpdateData(FALSE);
	//拼接列举路径
	m_SendPath = m_CurrPath + "\\*";
	//列举目录

	unsigned dwThreadId;
	m_hWorkThread  =
		(HANDLE)_beginthreadex(NULL,				 
		0,					 
		ListDirectory,  
		this,   
		0, 		 
		&dwThreadId);

	if (m_hWorkThread == NULL)
		m_wndStatusBar.SetText("获取远程目录列表失败", 0, 0);
	*pResult = 0;
}
