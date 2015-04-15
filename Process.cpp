// Process.cpp : 实现文件
//

#include "stdafx.h"
#include "maizang.h"
#include "Process.h"
#include "afxdialogex.h"
#include "Command.h"

// CProcess 对话框

IMPLEMENT_DYNAMIC(CProcess, CDialogEx)

	CProcess::CProcess(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProcess::IDD, pParent)
{

}

CProcess::~CProcess()
{
}

void CProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROCESS, m_ProcessListCtrl);
}


BEGIN_MESSAGE_MAP(CProcess, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_PROCESS, OnRclick)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_CLOSEWINDOW, &CProcess::OnCloseprocess)
	ON_COMMAND(ID_FRESH, &CProcess::OnFresh)
END_MESSAGE_MAP()

BOOL CProcess::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_ProcessListCtrl.InsertColumn(0, ("已打开进程名"), LVCFMT_LEFT, 400);
	m_ProcessListCtrl.InsertColumn(1, "id", LVCFMT_LEFT, 100);
	m_ProcessListCtrl.InsertColumn(2, "文件所在路径", LVCFMT_LEFT, 100);
	m_ProcessListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	return true;
}

void CProcess::processListShow(){
	LPMsgHead lpMsgHead= new MsgHead;
	lpMsgHead->dwSize = 0;
	lpMsgHead->dwCmd = CMD_SHOW_PROCESS_LIST;
	if(!SendMsg(m_MainSocket,NULL,lpMsgHead)){
		return;
	}
	char* pBuffer = new char[1024 * 200];
	memset(pBuffer, 0, strlen(pBuffer));
	RecvMsg(m_MainSocket, pBuffer, lpMsgHead);
	m_ProcessListCtrl.DeleteAllItems();
	CString	str;
	LPProcessInfo lpProcessInfo = new ProcessInfo;
	int offset = 0;
	for (int i = 0; i < lpMsgHead->dwExtend1 ; i++)
	{
		memcpy(lpProcessInfo, pBuffer + offset , sizeof(ProcessInfo));
		offset += sizeof(ProcessInfo);
		str = lpProcessInfo->strTitle;
		CString temp;
		temp.Format("%d", lpProcessInfo->dwProcessID);
		m_ProcessListCtrl.InsertItem(i, str);
		m_ProcessListCtrl.SetItemText(i, 1, temp);
		m_ProcessListCtrl.SetItemData(i, lpProcessInfo->dwProcessID);
		m_ProcessListCtrl.SetItemText(i, 2, lpProcessInfo->strPath);
	}
}


void CProcess::OnRclick(NMHDR* pNMHDR, LRESULT* pResult)
{
	CMenu	popup;
	popup.LoadMenu(IDR_MENU_WINDOW_LIST);
	CMenu*	pM = popup.GetSubMenu(0);
	CPoint	p;
	GetCursorPos(&p);
	pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);
	
}

void CProcess::OnCloseprocess()
{
	// TODO: 在此添加命令处理程序代码
	POSITION pos = m_ProcessListCtrl.GetFirstSelectedItemPosition(); 	
	LPMsgHead lpMsgHead = new MsgHead;
	int iCurrSel= m_ProcessListCtrl.GetNextSelectedItem(pos);
	int Id = m_ProcessListCtrl.GetItemData(iCurrSel);
	lpMsgHead->dwCmd  = CMD_PROCESS_DELETE;
	lpMsgHead->dwSize = 0;
	lpMsgHead->dwExtend1 = Id;
	SendMsg(m_MainSocket, NULL, lpMsgHead);
	delete lpMsgHead;
	Sleep(100);
	OnFresh();
}


void CProcess::OnFresh()
{
	// TODO: 在此添加命令处理程序代码
	processListShow();
}
