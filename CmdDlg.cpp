// CmdDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "maizang.h"
#include "CmdDlg.h"
#include "afxdialogex.h"
#include "Command.h"

// CCmdDlg 对话框

IMPLEMENT_DYNAMIC(CCmdDlg, CDialog)

	CCmdDlg::CCmdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCmdDlg::IDD, pParent)
	, m_CommandString(_T(""))
{
	m_CommandString = "";
	m_RemString = "";
}

CCmdDlg::~CCmdDlg()
{
}

void CCmdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Shell, m_Edit);
	DDX_Text(pDX, IDC_CommandEdit, m_CommandString);
}


BEGIN_MESSAGE_MAP(CCmdDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_Execute, &CCmdDlg::OnBnClickedExecute)
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CCmdDlg 消息处理程序


BOOL CCmdDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	SetWindowText("CmdShell");
	m_Edit.SetLimitText(MAXDWORD);


	Output = "";
	_beginthreadex(NULL, 0, RecvCmdDataThread, this, 0, NULL);



	return TRUE;  // return TRUE unless you set the focus to a control

}


/**************************接受被控端响应返回的消息***************************/
unsigned __stdcall RecvCmdDataThread(void *lpParam)
{
	CCmdDlg *This = (CCmdDlg *)(lpParam);
	char buf[6000];

	// ::MessageBox(NULL, "准备接受", "woca", MB_OK);
	Sleep(500);//等待一会  让被控端打开cmd  shell
	MsgHead msg;
	memset(buf, 0, sizeof(buf));
	while( RecvMsg( This->m_MainSocket, buf , &msg))
	{
		This->Output = buf;
		This->m_RemString += This->Output;
		This->m_RemString += "\r\n";
		CString str;
		str.Format("长度为%d", This->m_RemString.GetLength());
		//::MessageBox(NULL,str, str, MB_OK);
		if(This->m_RemString.GetLength() > 100000 ) 
		{
			This->m_RemString = This->Output;
		}

		//	m_Edit.SetWindowText(m_RemString);
		This->m_Edit.SetWindowText(This->m_RemString);

		CEdit* pedit = (CEdit*)This->GetDlgItem(IDC_Shell);//每次运行命令  滚动条停止在最下面
		pedit->LineScroll(pedit->GetLineCount());
		This->Invalidate(true);	
		memset(buf, 0, sizeof(buf));
		Sleep(100);
	}
	return 0;
} 





int CCmdDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	CClientDC dc(this);
	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);
	//根据字体大小，创建合适的插入符
	CreateSolidCaret(tm.tmAveCharWidth / 8, tm.tmHeight);
	ShowCaret();

	return 0;
}


void CCmdDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()

}


HBRUSH CCmdDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if ((pWnd->GetDlgCtrlID() == IDC_Shell) && (nCtlColor == CTLCOLOR_EDIT))
	{
		COLORREF clr = RGB(255, 255, 255);
		pDC->SetTextColor(clr);   //白色文本
		clr = RGB(0,0,0);
		pDC->SetBkColor(clr);     //黑色背景
		return CreateSolidBrush(clr);
	}
	else
	{
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
}


BOOL CCmdDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类


	return CDialog::PreTranslateMessage(pMsg);
}


void CCmdDlg::OnBnClickedExecute()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	m_CommandString += "\r\n";
	char *pMyCommand = (char*)(LPCTSTR)m_CommandString;
	MsgHead msg;
	msg.dwSize = strlen(pMyCommand);

	SendMsg(m_MainSocket, pMyCommand, &msg);

}


BOOL CCmdDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CDialog::OnEraseBkgnd(pDC);

}


void CCmdDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	MsgHead msg;
	msg.dwSize = 0;
	msg.dwCmd = 88;// 88 == 拜拜   关闭被控端cmd子进程
	SendMsg(m_MainSocket, NULL, &msg);
	 
	CDialog::OnClose();
}
									