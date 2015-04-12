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
	m_WindowListCtrl.InsertColumn(0, ("已打开窗口名"), LVCFMT_LEFT, 500);
	m_WindowListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
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
	m_WindowListCtrl.DeleteAllItems();
	CString	str;
	LPWindowInfo lpWindowInfo = new WindowInfo;
	int offset = 0;
	for (int i = 0; i < lpMsgHead->dwExtend1 ; i++)
	{
		memcpy(lpWindowInfo, pBuffer + offset , sizeof(WindowInfo));
		offset += sizeof(WindowInfo);
		str = lpWindowInfo->strTitle;
		CString temp;
		//str = str + temp.Format("%d", lpWindowInfo->dwProcessID);

		m_WindowListCtrl.InsertItem(i, str);
		//m_WindowListCtrl.SetItemText(i, 1, lpTitle);
		// ItemData 为进程ID
		m_WindowListCtrl.SetItemData(i, lpWindowInfo->dwProcessID);
	}
/*	str.Format("窗口名称");
	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT;
	lvc.pszText = str.GetBuffer(0);
	lvc.cchTextMax = str.GetLength();
	m_WindowListCtrl.SetColumn(1, &lvc);
	*/
	//delete[] pBuffer;
}
