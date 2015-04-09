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
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileManage message handlers

BOOL CFileManage::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	SetIcon(AfxGetApp()->LoadIcon(IDI_OL_FILE), FALSE);
	CenterWindow();
	if ( !m_wndToolBar.CreateEx(this, TBSTYLE_FLAT | TBSTYLE_LIST ,   //�������Ĵ���
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
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0); //�Զ���䵽�ǿͻ�����λ��
	VERIFY(m_wndToolBar.SetButtonText(0,"����"));   
	VERIFY(m_wndToolBar.SetButtonText(1,"����"));   
	VERIFY(m_wndToolBar.SetButtonText(2,"ճ��"));
	VERIFY(m_wndToolBar.SetButtonText(3,"ɾ��"));
	VERIFY(m_wndToolBar.SetButtonText(4,"�ϴ�"));   
	VERIFY(m_wndToolBar.SetButtonText(5,"����"));   
	VERIFY(m_wndToolBar.SetButtonText(6,"ˢ��"));  
	VERIFY(m_wndToolBar.SetButtonText(7,"�鿴"));   

	m_wndStatusBar.Create(WS_CHILD|WS_VISIBLE|CCS_BOTTOM,  CRect(0,0,0,0),  this,  0x1100001);//״̬��������
	int strPartDim[2]= {400,-1};
	m_wndStatusBar.SetParts(2,strPartDim);


	//���ز˵�====================================
	m_FileMenu.LoadMenu(IDR_MENU_FILE);

	m_FileList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	//�ļ����䴰�����ļ��б�ĳ�ʼ��
	m_FileList.InsertColumn(0, ("�ļ���"), LVCFMT_LEFT, 100);
	m_FileList.InsertColumn(1, "����", LVCFMT_LEFT, 100);
	m_FileList.InsertColumn(2, "��С", LVCFMT_LEFT, 90);
	m_FileList.InsertColumn(3, "�޸�ʱ��", LVCFMT_LEFT, 130);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//���ļ����䴰����ʾ״̬��Ϣ����������̨�������ļ��б� �Լ���ʾ����������Ϣ
void CFileManage::SetConnSocket(SOCKET socket)
{
	//�ͻ��������˽���ͨ�ŵ��׽���
	m_ConnSocket = socket ;
	sockaddr_in addr;
	int sz = sizeof(addr);
	//��ȡ�Զ�������Ϣ  ͨ������ֵret�ж�һЩ����
	int ret = getpeername(m_ConnSocket, (sockaddr *)&addr, &sz); 

	CString OnlineIP;
	OnlineIP.Format("%s:%d",inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
	SetWindowText("�ļ�����" + OnlineIP);
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
		m_wndStatusBar.SetText("��ȡԶ�̵��Դ�����Ϣʧ��", 0 ,0);
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
		This->m_wndStatusBar.SetText("ͨ��ʧ��!", 0, 0);	
		return 0;
	}

	if(This->m_MsgHead.dwCmd != CMD_SUCCEED)
	{
		This->m_wndStatusBar.SetText("��ȡԶ������������Ϣʧ��", 0, 0);
		return 0;
	}

	//��ʾԶ�̴���
	This->m_FileList.DeleteAllItems();

	DWORD dwNum = This->m_MsgHead.dwSize /sizeof(DriverInfo);
	BYTE * m_DesBuf = (LPBYTE) (This->m_Buffer);
	LPDriverInfo pInfo = (LPDriverInfo) m_DesBuf;
	int iImage =0, iSelectedImage=0;

	for(DWORD i = 0; i < dwNum; i++)
	{
		//����̷�
		switch(pInfo[i].drivertype)
		{
		case DRIVE_REMOVABLE:// ���ƶ�������
			{
				iImage        = 4;
				iSelectedImage= 4;
			}
			break;
		case DRIVE_REMOTE:   // ����������
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
		case DRIVE_FIXED:    // �̶���������
			{
				iImage        = 3;
				iSelectedImage= 3;
			}
			break;
		default://����ʶ����������������ʾ
			{
				pInfo[i].display[1]=0;	
			}
			break;
		}
		//Win2000����յ�һЩ������̷�
		if (strlen(pInfo[i].display) < 2)
			continue;
		int insertPos = This->m_FileList.GetItemCount();
		This->m_FileList.InsertItem(insertPos, pInfo[i].driver, NULL);
		This->m_FileList.SetItemData(insertPos, (DWORD)pInfo[i].driver[0]);
		This->m_FileList.SetItemText(insertPos,1,"����");
	}

	This->m_wndStatusBar.SetText("��ȡԶ�̴��̳ɹ�", 0, 0);
	return 0;
}

unsigned  __stdcall ListFiles(void * pParam)
{
	CFileManage *This = (CFileManage*)pParam; 
	//���ͻ�ȡ�̷�����
	This->m_MsgHead.dwCmd  = CMD_FILEDIRECTORY;
	This->m_MsgHead.dwSize = This->m_SendPath.GetLength();
	strcpy(This->m_Buffer,This->m_SendPath);
	//���ݴ���ͬʱ��������
	if( !SendMsg(This->m_ConnSocket, This->m_Buffer, &This->m_MsgHead) ||
		!RecvMsg(This->m_ConnSocket, This->m_Buffer, &This->m_MsgHead))
	{
		//���ݴ���ʧ��
		This->m_wndStatusBar.SetText("ͨ��ʧ��", 0, 0);
		return 0;
	}
	if(This->m_MsgHead.dwCmd != CMD_SUCCEED)
	{
		//���ݴ���ʧ��
		if(This->m_MsgHead.dwCmd == CMD_DIRFLODERERR)
		{
			This->m_CurrPath = "";
			This->m_wndStatusBar.SetText("Ŀ¼���ܷ���", 0, 0); 
		}
		else
		{
			This->m_wndStatusBar.SetText("��ȡԶ��Ŀ¼ʧ��", 0, 0); 
			This->m_CurrPath = "";
		}
		return 0;
	}

	//��ʾ�ļ��б�
	This->m_FileList.DeleteAllItems();

	DWORD dwNum = This->m_MsgHead.dwSize /sizeof(FileInfo);
	BYTE * m_DesBuf = (LPBYTE)(This->m_Buffer);
	LPFileInfo pInfo = (LPFileInfo) m_DesBuf;
	for(DWORD i = 0; i < dwNum; i++)
	{
		//��ʾʱ���ļ��������棬�ļ�������
		int iIcon = 0, iInsertItem = 0;
		if(pInfo[i].iType == 2)//�ļ�
		{
			iInsertItem =This->m_FileList.GetItemCount();				
		}
		if(pInfo[i].iType == 1)//�ļ���
		{	
			strcpy(pInfo[i].cAttrib,"�ļ���");
			iInsertItem = 0;
		}
		This->m_FileList.InsertItem(iInsertItem,pInfo[i].cFileName);
		This->m_FileList.SetItemData(iInsertItem,pInfo[i].iType);
		This->m_FileList.SetItemText(iInsertItem,0,pInfo[i].cFileName);
		This->m_FileList.SetItemText(iInsertItem,1,pInfo[i].cAttrib);
		This->m_FileList.SetItemText(iInsertItem,2,pInfo[i].cSize);
		This->m_FileList.SetItemText(iInsertItem,3,pInfo[i].cTime);
	}
	This->m_wndStatusBar.SetText("��ȡԶ���ļ��б�ɹ�", 0, 0);
	CString str;
	str.Format("�����ļ����ļ��� %d��", dwNum);
	This->m_wndStatusBar.SetText(str, 1, 0);
	return 0;
}

void CFileManage::OnNMDblclkFilelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = m_FileList.GetFirstSelectedItemPosition(); // todo
	if(pos == NULL) return ;
	int iCurrSel= m_FileList.GetNextSelectedItem(pos);
	int fileCategory = m_FileList.GetItemData(iCurrSel);
	if(fileCategory !=2 ){   //˫������Ŀ¼
		if(m_CurrPath == ""){
			if (fileCategory =='A' || fileCategory=='B')//A,B�̲�����
				return;
			m_CurrPath.Format("%c:",(char)fileCategory);
			m_SendPath = m_CurrPath + "\\*";
		}else{
			m_CurrPath = m_CurrPath + "\\" + m_FileList.GetItemText(iCurrSel,0);
			m_SendPath = m_CurrPath + "\\*";
		}
		getFilesByCurrPath();	
	}
	else{  //˫�������ļ�
		::MessageBox(NULL, "����һ���ļ�����ѡ�к�ѡ������","����һ���ļ�", MB_OK);
	}
	*pResult = 0;
}


void CFileManage::OnUp()
{
	// TODO: �ڴ���������������
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
	//�о�Ŀ¼�߳�
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
		m_wndStatusBar.SetText("��ȡԶ��Ŀ¼�б�ʧ��", 0, 0);
		m_CurrPath = "";
		m_SendPath = "";
	}
	CloseHandle(m_hWorkThread);
}


void CFileManage::OnFileFresh()
{
	// TODO: �ڴ���������������
	getFilesByCurrPath();
}

CString CFileManage::chooseDirectory(){
	TCHAR           szFolderPath[MAX_PATH] = {0};  
	CString         strFolderPath = TEXT("");  
	BROWSEINFO      sInfo;  
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));  
	sInfo.pidlRoot   = 0;  
	sInfo.lpszTitle   = _T("��ѡ��һ���ļ��У�");  
	sInfo.ulFlags   = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;  
	sInfo.lpfn     = NULL;  

	// ��ʾ�ļ���ѡ��Ի���  
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);   
	if (lpidlBrowse != NULL)  
	{  
		// ȡ���ļ�����  
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
	// TODO: �ڴ���������������
	CString savePath = chooseDirectory();
	POSITION pos = m_FileList.GetFirstSelectedItemPosition(); 
	while(pos){	
		int iCurrSel= m_FileList.GetNextSelectedItem(pos);
		int fileCategory = m_FileList.GetItemData(iCurrSel);
		if((fileCategory >= 'A' && fileCategory <= 'Z') || (fileCategory >= 'a' && fileCategory <= 'z')) {  //���Ҫ�������������򲻴���   ����һ����̫������
				continue;
		}
		CString fileName = m_FileList.GetItemText(iCurrSel, 0);

		if(fileCategory !=2 ){   //��Ŀ¼
			directoryDownload(savePath, fileName);
		}
		else{
			fileDownload(savePath ,fileName);
		}
	}
}


void CFileManage::directoryDownload(CString localPath, CString fileName){
}


void CFileManage::fileDownload(CString localPath, CString fileName){
	CString remotePath = m_CurrPath + "\\" + fileName;
	localPath = localPath + "\\" + fileName;

	m_MsgHead.dwCmd = CMD_GETFILE;
	m_MsgHead.dwSize = remotePath.GetLength();
	if(!SendMsg(m_ConnSocket, (LPSTR)(LPCTSTR)remotePath, &m_MsgHead)  ) 
	{
		m_wndStatusBar.SetText("ͨ��ʧ��!", 0, 0);	
		return ;
	}
	CFile file(localPath, CFile::modeCreate | CFile::modeWrite);
	while(RecvMsg(m_ConnSocket, m_Buffer, &m_MsgHead)){
		file.Write(m_Buffer, m_MsgHead.dwSize);
		if(m_MsgHead.dwSize < MAX_FILE_DATA_BUFFER_SIZE) break;
	}
	m_wndStatusBar.SetText(remotePath + "   ���سɹ�", 0, 0);
	file.Close();
}
