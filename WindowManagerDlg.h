#pragma once


// CWindowManagerDlg �Ի���

class CWindowManagerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWindowManagerDlg)

public:
	CWindowManagerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWindowManagerDlg();
    SOCKET m_MainSocket;
// �Ի�������
	enum { IDD = IDD_WINDOW_MANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private :
	
};
