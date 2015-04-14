// MessageBoxDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "maizang.h"
#include "MessageBoxDlg.h"
#include "afxdialogex.h"



// CMessageBoxDlg �Ի���

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


// CMessageBoxDlg ��Ϣ��������


void CMessageBoxDlg::OnBnClickedSendmessagebox()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	UpdateData(TRUE);
	MsgHead msgHead;
	msgHead.dwCmd = CMD_MESSAGEBOX;
	msgHead.dwSize = m_title.GetLength();
	if(m_category.Find('2') != -1){
		msgHead.dwExtend1 = 2;
	}
	SendMsg(m_MainSocket, (LPSTR)(LPCSTR)m_title, &msgHead);
	msgHead.dwSize = m_content.GetLength();
	SendMsg(m_MainSocket, (LPSTR)(LPCSTR)m_content, &msgHead);
}



void CMessageBoxDlg::OnBnClickedAllSendmessagebox()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������	
	if(::MessageBox(NULL,"ȷ�Ϸ��͵������û���", "ȷ�Ϸ��͵������û���", MB_YESNO)==IDYES)
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