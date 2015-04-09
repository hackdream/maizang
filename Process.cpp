#include "stdafx.h"
#include "maizang.h"
#include "Process.h"
#include "afxdialogex.h"
#include "Command.h"
#include <vector>
using std::vector;
vector<ProcsInfo> m_ProcsInfo;
CProcess::CProcess(CWnd *pParent) :CDialog(CProcess::IDD,pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
//CProcess::~CProcess(){
//	/*if (m_Buffer != NULL){
//		delete [] m_Buffer;
//	}*/
//}

BEGIN_MESSAGE_MAP(CProcess,CDialog)
	//ON_WM_SYSCOMMAND()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_REFRESH,OnRefresh)
	ON_BN_CLICKED(ID_ENDTASK,OnEndtask)
END_MESSAGE_MAP()

HCURSOR CProcess::OnQueryDragIcon(){
	return (HCURSOR) m_hIcon;
}

void CProcess::DoDataExchange(CDataExchange *pDX){
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROCESS, m_CtrlProcessList);
}

BOOL EnumProcessProc(void *lpParam,char *chBuffer,DWORD lenInfo)
{
	CWnd *cwnd=(CWnd*) lpParam;
	CListCtrl *p_ListCtrl;
	DWORD x;
	m_ProcsInfo.clear();
	ProcsInfo temp;
	for (x = 0; x < lenInfo; x++){
		temp.dwProcessID[0] = 0; temp.szFileName[0] = 0; temp.szFilePath[0] = 0;
		DWORD lenName;
		DWORD lenPIDs;
		DWORD lenPath;
		for (lenName = 0; chBuffer[x] != ';'&&x < lenInfo; x++)
			temp.szFileName[lenName++] = chBuffer[x]; 
		temp.szFileName[lenName] = 0;
		for (lenPIDs = 0,x=x+1; chBuffer[x] != ';'&&x < lenInfo; x++)
			temp.dwProcessID[lenPIDs++] = chBuffer[x];
		temp.dwProcessID[lenPIDs] = 0;
		for (lenPath = 0,x=x+1; chBuffer[x] != ';'&&x < lenInfo; x++)
			temp.szFilePath[lenPath++] = chBuffer[x];
		temp.szFilePath[lenPath] = 0;
		m_ProcsInfo.push_back(temp);
	}
	p_ListCtrl = NULL;
	p_ListCtrl = (CListCtrl*) cwnd->GetDlgItem(IDC_LIST_PROCESS);
	p_ListCtrl->DeleteAllItems();
	for (auto &x : m_ProcsInfo){
		p_ListCtrl = NULL;
		p_ListCtrl = (CListCtrl*) cwnd->GetDlgItem(IDC_LIST_PROCESS);
		//p_ListCtrl = (CListCtrl*) AfxGetMainWnd()->GetDlgItem(IDC_LIST_PROCESS);
		if ((p_ListCtrl != NULL) && (strlen(x.szFilePath) != 0)){
			p_ListCtrl->InsertItem(0, x.szFileName, 0);
			p_ListCtrl->SetItemText(0, 1, x.dwProcessID);
			p_ListCtrl->SetItemText(0, 2, x.szFilePath);
		}
	}
	p_ListCtrl = NULL;
	p_ListCtrl = (CListCtrl*)cwnd->GetDlgItem(IDC_LIST_PROCESS);
	//p_ListCtrl = (CListCtrl*) AfxGetMainWnd()->GetDlgItem(IDC_LIST_PROCESS);
	p_ListCtrl->SetItem(0, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED, LVIS_SELECTED, 0);
	p_ListCtrl->SetSelectionMark(0);
	p_ListCtrl->SetFocus();
	return TRUE;
}
BOOL CProcess::OnInitDialog(){
	//return TRUE;
	CDialog::OnInitDialog();
	SetWindowText("Process");
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	InitializeViews();
	HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, RecvProcessDataThread, this, 0, NULL);
	CloseHandle(handle);
	OnRefresh();
	return TRUE;
}

unsigned __stdcall RecvProcessDataThread(void *lpParam){
	CProcess *This = (CProcess *) (lpParam);
	char *buf=new char[25005];
	MsgHead msg;
	memset(buf, 0, sizeof buf);
	while (RecvMsg(This->m_ConnSocket, buf, &msg)){
		switch (msg.dwCmd){
			case CMD_REFRESH:
			{
				EnumProcessProc(lpParam, buf, msg.dwSize);
				break;
			}
			case CMD_ENDTASK:
			{
				EnumProcessProc(lpParam, buf, msg.dwSize);
				break;
			}
		}
	}
	return 0;
}

int CProcess::OnCreate(LPCREATESTRUCT lpCreateStruct){
	CDialog::OnCreate(lpCreateStruct);
	CClientDC dc(this);
	return 0;
}

void CProcess::OnSize(UINT nType,int cx,int cy){
	CDialog::OnSize(nType, cx, cy);
	return;
}

void CProcess::OnPaint(){
	CPaintDC dc(this);
	return;
}

void CProcess::InitializeViews(){
	//m_CtrlProcessList.Create(LVS_LIST | LVS_SHOWSELALWAYS | WS_BORDER | WS_TABSTOP, CRect(8, 10, 164, 275), this, IDC_LIST_PROCESS);
	m_CtrlProcessList.InsertColumn(0, _T("Process Name"),LVCFMT_LEFT, 100);
	m_CtrlProcessList.InsertColumn(1, _T("Process ID"), LVCFMT_LEFT, 75);
	m_CtrlProcessList.InsertColumn(2, _T("Path"), LVCFMT_LEFT, 825);
	
	m_CtrlProcessList.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	m_CtrlProcessList.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
	m_CtrlProcessList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

}

void CProcess::OnRefresh(){
	MsgHead MsgSend;
	MsgSend.dwCmd = CMD_REFRESH;
	MsgSend.dwSize = 0;
	SendMsg(this->m_ConnSocket, NULL, &MsgSend);
}

void CProcess::OnEndtask(){
	MsgHead MsgSend;
	MsgSend.dwCmd = CMD_ENDTASK;
	MsgSend.dwSize = 0;
	CString strSelected;
	int nCount = m_CtrlProcessList.GetItemCount();
	for (int i = 0; i < nCount; i++){
		if (m_CtrlProcessList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED){
			strSelected = m_CtrlProcessList.GetItemText(i, 1);
			MsgSend.dwExtend1 = atoi(strSelected.operator LPCTSTR());
			break;
		}
	}
	SendMsg(this->m_ConnSocket, NULL, &MsgSend);
}


//void CProcess::OnClose(){
//	MsgHead msg;
//	msg.dwSize = 0;
//	msg.dwCmd = 88;
//	SendMsg(m_ConnSocket, NULL, &msg);
//	CDialog::OnClose();
//}