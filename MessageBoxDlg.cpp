// MessageBoxDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "maizang.h"
#include "MessageBoxDlg.h"
#include "afxdialogex.h"



// CMessageBoxDlg 对话框

IMPLEMENT_DYNAMIC(CMessageBoxDlg, CDialogEx)

CMessageBoxDlg::CMessageBoxDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMessageBoxDlg::IDD, pParent)
	, m_title(_T(""))
	, m_content(_T(""))
{
	
}

CMessageBoxDlg::~CMessageBoxDlg()
{
}

void CMessageBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_title);
	DDX_Text(pDX, IDC_EDIT3, m_content);
	DDX_Text(pDX, IDC_EDIT4, m_category);
}


BEGIN_MESSAGE_MAP(CMessageBoxDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SendMessageBox, &CMessageBoxDlg::OnBnClickedSendmessagebox)
	ON_BN_CLICKED(IDC_All_SendMessageBox, &CMessageBoxDlg::OnBnClickedAllSendmessagebox)
END_MESSAGE_MAP()


// CMessageBoxDlg 消息处理程序


void CMessageBoxDlg::OnBnClickedSendmessagebox()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	MyMessageBox myMessageBox;
	if(m_category.Find('2'))
		myMessageBox.category = 2;
	else myMessageBox.category = 1;
	strncpy(myMessageBox.title, m_title, 1024);
	strncpy(myMessageBox.content, m_content, 1024 * 20);
	MsgHead msgHead;
	msgHead.dwCmd = CMD_MESSAGEBOX;
	msgHead.dwSize = sizeof(MyMessageBox);
	SendMsg(m_MainSocket, (char*)&myMessageBox, &msgHead);
}



void CMessageBoxDlg::OnBnClickedAllSendmessagebox()
{
	// TODO: 在此添加控件通知处理程序代码	
	if(::MessageBox(NULL,"确认发送到所有用户！", "确认发送到所有用户！", MB_YESNO)==IDYES)
	{
		list<SOCKET>* pSocketList = pMaizangDlg->getAllSocket();
		list<SOCKET>::iterator it;
		for(it = pSocketList->begin(); it != pSocketList->end(); ++it)
		{
			m_MainSocket = *it;
		    OnBnClickedSendmessagebox();
		}
	}
}

void CMessageBoxDlg::setSendButton(BOOL b){
	GetDlgItem(IDC_SendMessageBox)->EnableWindow(b);
}

void CMessageBoxDlg::setAllSendButton(BOOL b){
	GetDlgItem(IDC_All_SendMessageBox)->EnableWindow(b);
}     