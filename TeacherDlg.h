#pragma once

#include "maizangDlg.h"
// CTeacherDlg �Ի���

class CTeacherDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTeacherDlg)

public:
	CTeacherDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTeacherDlg();

// �Ի�������
	enum { IDD = IDD_TeacherDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAllOpenMessagedlg();	
	CMaizangDlg *pMaizangDlg;
	afx_msg void OnBnClickedAllSendScree();
	
};

DWORD SendScreen(LPVOID lparam);//�̴߳�����Ļ����
unsigned  __stdcall  SendScreenThread(void * pParam);