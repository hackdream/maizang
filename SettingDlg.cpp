// SettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "maizang.h"
#include "SettingDlg.h"
#include "afxdialogex.h"


// CSettingDlg �Ի���

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


// CSettingDlg ��Ϣ�������


void CSettingDlg::OnBnClickedGenerateserver()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
