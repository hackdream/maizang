#pragma once


// CWindowManagerDlg 对话框

class CWindowManagerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWindowManagerDlg)

public:
	CWindowManagerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWindowManagerDlg();
    SOCKET m_MainSocket;
// 对话框数据
	enum { IDD = IDD_WINDOW_MANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private :
	
};
