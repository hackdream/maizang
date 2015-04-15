// TeacherDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "maizang.h"
#include "TeacherDlg.h"
#include "afxdialogex.h"
#include "MessageBoxDlg.h"


// CTeacherDlg 对话框

IMPLEMENT_DYNAMIC(CTeacherDlg, CDialogEx)

CTeacherDlg::CTeacherDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTeacherDlg::IDD, pParent)
{

}

CTeacherDlg::~CTeacherDlg()
{
}

void CTeacherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTeacherDlg, CDialogEx)
	ON_BN_CLICKED(IDC_ALL_OPEN_MESSAGEDLG, &CTeacherDlg::OnBnClickedAllOpenMessagedlg)
	ON_BN_CLICKED(IDC_ALL_SEND_SCREE, &CTeacherDlg::OnBnClickedAllSendScree)
END_MESSAGE_MAP()


// CTeacherDlg 消息处理程序


void CTeacherDlg::OnBnClickedAllOpenMessagedlg()
{
	// TODO: 在此添加控件通知处理程序代码
	CMessageBoxDlg *pMessageBoxDlg = new CMessageBoxDlg;
	pMessageBoxDlg->pMaizangDlg = pMaizangDlg;
	pMessageBoxDlg->Create(IDD_MESSAGEBOX, GetDesktopWindow());//创建一个非模态对话框
	pMessageBoxDlg->ShowWindow(SW_SHOW);
	pMessageBoxDlg->setSendButton(FALSE);
	pMessageBoxDlg->setAllSendButton(TRUE);
	pMessageBoxDlg->UpdateData(TRUE);
}


//发送教师端屏幕到所有主机！
void CTeacherDlg::OnBnClickedAllSendScree()
{
	// TODO: 在此添加控件通知处理程序代码

	list<SOCKET> *pSockets = pMaizangDlg->getAllSocket();
	list<SOCKET>::iterator it;
	for(it = pSockets->begin(); it != pSockets->end(); it++){
		unsigned dwThreadId; 
		SOCKET sock = *it;
		HANDLE hThread = (HANDLE)_beginthreadex(NULL,				 
			0,					 
			SendScreenThread,  //调用ThreadAccept线程
			&sock,
			0,
			&dwThreadId);
		CloseHandle(hThread);
	}
	
}


unsigned  __stdcall  SendScreenThread(void * pParam){
	SOCKET *pSocket = (SOCKET  *)pParam;

	MsgHead msgHead;
	msgHead.dwCmd = CMD_SHOW_RECV_SCREEN_DLG;
	msgHead.dwSize = 0;
	SendMsg(*pSocket, NULL, &msgHead);
	return 0;
}