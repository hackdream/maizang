// WindowManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "maizang.h"
#include "WindowManagerDlg.h"
#include "afxdialogex.h"
#include "Command.h"

// CWindowManagerDlg 对话框

IMPLEMENT_DYNAMIC(CWindowManagerDlg, CDialogEx)

CWindowManagerDlg::CWindowManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWindowManagerDlg::IDD, pParent)
{

}

CWindowManagerDlg::~CWindowManagerDlg()
{
}

void CWindowManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WINDOW_LIST, m_WindowListCtrl);
}


BEGIN_MESSAGE_MAP(CWindowManagerDlg, CDialogEx)

END_MESSAGE_MAP()

BOOL CWindowManagerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	return true;
}

void CWindowManagerDlg::windowListShow(){
	LPMsgHead lpMsgHead= new MsgHead;
	lpMsgHead->dwSize = 0;
	lpMsgHead->dwCmd = CMD_SHOW_WINDOW_LIST;
	if(!SendMsg(m_MainSocket,NULL,lpMsgHead)){
		return;
	}
	char* pBuffer = new char[1024 * 200];
	memset(pBuffer, 0, strlen(pBuffer));
	RecvMsg(m_MainSocket, pBuffer, lpMsgHead);
	LPBYTE lpBuffer = (LPBYTE)(&(pBuffer)[1]);
	DWORD	dwOffset = 0;
	char	*lpTitle = NULL;
	m_WindowListCtrl.DeleteAllItems();
	CString	str;
	for (int i = 0; lpBuffer[i] != '\0' ; i++)
	{
		LPDWORD	lpPID = LPDWORD(lpBuffer + dwOffset);
		lpTitle = (char *)lpBuffer + dwOffset + sizeof(DWORD);
		str.Format("%5u", *lpPID);
		str.Append(" ");
		str.Append(lpTitle);
		m_WindowListCtrl.InsertItem(i, str);
		//m_WindowListCtrl.SetItemText(i, 1, lpTitle);
		// ItemData 为进程ID
		m_WindowListCtrl.SetItemData(i, *lpPID);
		dwOffset += sizeof(DWORD) + lstrlen(lpTitle) + 1;
	}
	str.Format("窗口名称");
	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT;
	lvc.pszText = str.GetBuffer(0);
	lvc.cchTextMax = str.GetLength();
	m_WindowListCtrl.SetColumn(1, &lvc);
	//delete[] pBuffer;
}
