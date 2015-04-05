// SettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "maizang.h"
#include "SettingDlg.h"
#include "afxdialogex.h"


// CSettingDlg 对话框

IMPLEMENT_DYNAMIC(CSettingDlg, CDialogEx)

CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSettingDlg::IDD, pParent)
{

}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_GenerateServer, &CSettingDlg::OnBnClickedGenerateserver)
END_MESSAGE_MAP()


// CSettingDlg 消息处理程序


void CSettingDlg::OnBnClickedGenerateserver()
{
	// TODO: 在此添加控件通知处理程序代码
}
