// WindowManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "maizang.h"
#include "WindowManagerDlg.h"
#include "afxdialogex.h"


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
}


BEGIN_MESSAGE_MAP(CWindowManagerDlg, CDialogEx)
END_MESSAGE_MAP()


// CWindowManagerDlg 消息处理程序
