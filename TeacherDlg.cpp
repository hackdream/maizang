// TeacherDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "maizang.h"
#include "TeacherDlg.h"
#include "afxdialogex.h"
#include "MessageBoxDlg.h"


// CTeacherDlg �Ի���

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


// CTeacherDlg ��Ϣ�������


void CTeacherDlg::OnBnClickedAllOpenMessagedlg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMessageBoxDlg *pMessageBoxDlg = new CMessageBoxDlg;
	pMessageBoxDlg->pMaizangDlg = pMaizangDlg;
	pMessageBoxDlg->Create(IDD_MESSAGEBOX, GetDesktopWindow());//����һ����ģ̬�Ի���
	pMessageBoxDlg->ShowWindow(SW_SHOW);
	pMessageBoxDlg->setSendButton(FALSE);
	pMessageBoxDlg->setAllSendButton(TRUE);
	pMessageBoxDlg->UpdateData(TRUE);
}
