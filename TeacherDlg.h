#pragma once

#include "maizangDlg.h"
// CTeacherDlg 对话框

class CTeacherDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTeacherDlg)

public:
	CTeacherDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTeacherDlg();

// 对话框数据
	enum { IDD = IDD_TeacherDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAllOpenMessagedlg();	
	CMaizangDlg *pMaizangDlg;
	afx_msg void OnBnClickedAllSendScree();
	
};

DWORD SendScreen(LPVOID lparam);//线程处理屏幕传输
unsigned  __stdcall  SendScreenThread(void * pParam);