// TeacherDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "maizang.h"
#include "TeacherDlg.h"
#include "afxdialogex.h"
#include "MessageBoxDlg.h"


// CTeacherDlg 对话框

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
END_MESSAGE_MAP()


// CTeacherDlg 消息处理程序


void CTeacherDlg::OnBnClickedAllOpenMessagedlg()
{
	// TODO: 在此添加控件通知处理程序代码
	CMessageBoxDlg *pMessageBoxDlg = new CMessageBoxDlg;
	pMessageBoxDlg->pMaizangDlg = pMaizangDlg;
	pMessageBoxDlg->Create(IDD_MESSAGEBOX, GetDesktopWindow());//创建一个非模态对话框
	pMessageBoxDlg->ShowWindow(SW_SHOW);
	pMessageBoxDlg->setSendButton(FALSE);
	pMessageBoxDlg->setAllSendButton(TRUE);
	pMessageBoxDlg->UpdateData(TRUE);
}
