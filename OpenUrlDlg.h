#pragma once


// COpenUrlDlg �Ի���

class COpenUrlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COpenUrlDlg)

public:
	COpenUrlDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COpenUrlDlg();

// �Ի�������
	enum { IDD = IDD_OPENURL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString m_Url;
	SOCKET m_MainSocket;
};
