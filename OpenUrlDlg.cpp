// OpenUrlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "maizang.h"
#include "OpenUrlDlg.h"
#include "afxdialogex.h"
#include "Command.h"


// COpenUrlDlg 对话框

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


// COpenUrlDlg 消息处理程序


void COpenUrlDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	MsgHead msgHead;
	msgHead.dwCmd = CMD_OPEN_URL;
	msgHead.dwSize = m_Url.GetLength();
	if(m_Url == "")
	{
			::MessageBox(NULL, "url为空", "url为空",MB_OK);
			return;
	}
	/*if(m_MainSocket == INVALID_SOCKET)
	{
			::MessageBox(NULL, "你还没有选择任何主机", "你还没有选择任何主机",MB_OK);
			return;
	}*/
	SendMsg(m_MainSocket, (LPSTR)(LPCSTR)m_Url, &msgHead);
}