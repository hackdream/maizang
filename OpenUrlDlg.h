#pragma once


// COpenUrlDlg 对话框

class COpenUrlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COpenUrlDlg)

public:
	COpenUrlDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COpenUrlDlg();

// 对话框数据
	enum { IDD = IDD_OPENURL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString m_Url;
	SOCKET m_MainSocket;
};
