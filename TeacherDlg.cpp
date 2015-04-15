// TeacherDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "maizang.h"
#include "TeacherDlg.h"
#include "afxdialogex.h"
#include "MessageBoxDlg.h"


// CTeacherDlg �Ի���

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


// CTeacherDlg ��Ϣ�������


void CTeacherDlg::OnBnClickedAllOpenMessagedlg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMessageBoxDlg *pMessageBoxDlg = new CMessageBoxDlg;
	pMessageBoxDlg->pMaizangDlg = pMaizangDlg;
	pMessageBoxDlg->Create(IDD_MESSAGEBOX, GetDesktopWindow());//����һ����ģ̬�Ի���
	pMessageBoxDlg->ShowWindow(SW_SHOW);
	pMessageBoxDlg->setSendButton(FALSE);
	pMessageBoxDlg->setAllSendButton(TRUE);
	pMessageBoxDlg->UpdateData(TRUE);
}


//���ͽ�ʦ����Ļ������������
void CTeacherDlg::OnBnClickedAllSendScree()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	list<SOCKET> *pSockets = pMaizangDlg->getAllSocket();
	list<SOCKET>::iterator it;
	for(it = pSockets->begin(); it != pSockets->end(); it++){
		unsigned dwThreadId; 
		SOCKET sock = *it;
		HANDLE hThread = (HANDLE)_beginthreadex(NULL,				 
			0,					 
			SendScreenThread,  //����ThreadAccept�߳�
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