// OpenUrlDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "maizang.h"
#include "OpenUrlDlg.h"
#include "afxdialogex.h"
#include "Command.h"


// COpenUrlDlg �Ի���

IMPLEMENT_DYNAMIC(COpenUrlDlg, CDialogEx)

COpenUrlDlg::COpenUrlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COpenUrlDlg::IDD, pParent)
	, m_Url(_T(""))
{
	m_MainSocket = INVALID_SOCKET;
}

COpenUrlDlg::~COpenUrlDlg()
{
}

void COpenUrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Url);
}


BEGIN_MESSAGE_MAP(COpenUrlDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &COpenUrlDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// COpenUrlDlg ��Ϣ�������


void COpenUrlDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	MsgHead msgHead;
	msgHead.dwCmd = CMD_OPEN_URL;
	msgHead.dwSize = m_Url.GetLength();
	if(m_Url == "")
	{
			::MessageBox(NULL, "urlΪ��", "urlΪ��",MB_OK);
			return;
	}
	/*if(m_MainSocket == INVALID_SOCKET)
	{
			::MessageBox(NULL, "�㻹û��ѡ���κ�����", "�㻹û��ѡ���κ�����",MB_OK);
			return;
	}*/
	SendMsg(m_MainSocket, (LPSTR)(LPCSTR)m_Url, &msgHead);
}