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
    ON_NOTIFY(NM_RCLICK, IDC_WINDOW_LIST, OnRclick)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_CLOSEWINDOW, &CWindowManagerDlg::OnClosewindow)
	ON_COMMAND(ID_FRESH, &CWindowManagerDlg::OnFresh)
END_MESSAGE_MAP()

BOOL CWindowManagerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_WindowListCtrl.InsertColumn(0, ("已打开窗口名"), LVCFMT_LEFT, 500);
	m_WindowListCtrl.InsertColumn(1, "id", LVCFMT_LEFT, 100);
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
	    temp.Format("%d", lpWindowInfo->dwProcessID);
		m_WindowListCtrl.InsertItem(i, str);
		m_WindowListCtrl.SetItemText(i, 1, temp);
		m_WindowListCtrl.SetItemData(i, lpWindowInfo->dwProcessID);
	}
}


void CWindowManagerDlg::OnRclick(NMHDR* pNMHDR, LRESULT* pResult)
{
	CMenu	popup;
	popup.LoadMenu(IDR_MENU_WINDOW_LIST);
	CMenu*	pM = popup.GetSubMenu(0);
	CPoint	p;
	GetCursorPos(&p);
	pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);
}

void CWindowManagerDlg::OnClosewindow()
{
	// TODO: 在此添加命令处理程序代码
	POSITION pos = m_WindowListCtrl.GetFirstSelectedItemPosition(); 	
	LPMsgHead lpMsgHead = new MsgHead;
        int iCurrSel= m_WindowListCtrl.GetNextSelectedItem(pos);
		int Id = m_WindowListCtrl.GetItemData(iCurrSel);
		lpMsgHead->dwCmd  = CMD_WINDOW_DELETE;
		lpMsgHead->dwSize = 0;
		lpMsgHead->dwExtend1 = Id;
		SendMsg(m_MainSocket, NULL, lpMsgHead);
	delete lpMsgHead;
}


void CWindowManagerDlg::OnFresh()
{
	// TODO: 在此添加命令处理程序代码
	windowListShow();
}
