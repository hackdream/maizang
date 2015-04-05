// CmdDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "maizang.h"
#include "CmdDlg.h"
#include "afxdialogex.h"
#include "Command.h"

// CCmdDlg �Ի���

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


// CCmdDlg ��Ϣ�������


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


/**************************���ܱ��ض���Ӧ���ص���Ϣ***************************/
unsigned __stdcall RecvCmdDataThread(void *lpParam)
{
	CCmdDlg *This = (CCmdDlg *)(lpParam);
	char buf[6000];

	// ::MessageBox(NULL, "׼������", "woca", MB_OK);
	Sleep(500);//�ȴ�һ��  �ñ��ض˴�cmd  shell
	MsgHead msg;
	memset(buf, 0, sizeof(buf));
	while( RecvMsg( This->m_MainSocket, buf , &msg))
	{
		This->Output = buf;
		This->m_RemString += This->Output;
		This->m_RemString += "\r\n";
		CString str;
		str.Format("����Ϊ%d", This->m_RemString.GetLength());
		//::MessageBox(NULL,str, str, MB_OK);
		if(This->m_RemString.GetLength() > 100000 ) 
		{
			This->m_RemString = This->Output;
		}

		//	m_Edit.SetWindowText(m_RemString);
		This->m_Edit.SetWindowText(This->m_RemString);

		CEdit* pedit = (CEdit*)This->GetDlgItem(IDC_Shell);//ÿ����������  ������ֹͣ��������
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
	//���������С���������ʵĲ����
	CreateSolidCaret(tm.tmAveCharWidth / 8, tm.tmHeight);
	ShowCaret();

	return 0;
}


void CCmdDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

}


HBRUSH CCmdDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if ((pWnd->GetDlgCtrlID() == IDC_Shell) && (nCtlColor == CTLCOLOR_EDIT))
	{
		COLORREF clr = RGB(255, 255, 255);
		pDC->SetTextColor(clr);   //��ɫ�ı�
		clr = RGB(0,0,0);
		pDC->SetBkColor(clr);     //��ɫ����
		return CreateSolidBrush(clr);
	}
	else
	{
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
}


BOOL CCmdDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���


	return CDialog::PreTranslateMessage(pMsg);
}


void CCmdDlg::OnBnClickedExecute()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	m_CommandString += "\r\n";
	char *pMyCommand = (char*)(LPCTSTR)m_CommandString;
	MsgHead msg;
	msg.dwSize = strlen(pMyCommand);

	SendMsg(m_MainSocket, pMyCommand, &msg);

}


BOOL CCmdDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return CDialog::OnEraseBkgnd(pDC);

}


void CCmdDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	MsgHead msg;
	msg.dwSize = 0;
	msg.dwCmd = 88;// 88 == �ݰ�   �رձ��ض�cmd�ӽ���
	SendMsg(m_MainSocket, NULL, &msg);
	 
	CDialog::OnClose();
}
									