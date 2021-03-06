#if !defined(AFX_FILEMANAGE_H__50D21541_1D71_45D7_A10E_C64F51981682__INCLUDED_)
#define AFX_FILEMANAGE_H__50D21541_1D71_45D7_A10E_C64F51981682__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileManage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFileManage dialog
#include "TrueColorToolBar.h"
//#include <winsock.h>
#ifndef _HEAD_COMMAND_H
#define _HEAD_COMMAND_H
#include  "Command.h"
#endif

class CFileManage : public CDialog
{
	// Construction
public:
	CFileManage(CWnd* pParent = NULL);   // standard constructor
	void SetConnSocket(SOCKET socket);
	// Dialog Data
	//{{AFX_DATA(CFileManage)
	enum { IDD = IDD_FILEMANAGE };
	CListCtrl	m_FileList;
	//}}AFX_DATA

	~CFileManage();
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileManage)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFileManage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	CTrueColorToolBar m_wndToolBar;
	CStatusBarCtrl m_wndStatusBar;
	CMenu m_FileMenu;	
	SOCKET m_ConnSocket;
	HANDLE m_hWorkThread;
	MsgHead m_MsgHead;
	char * m_Buffer;
	void GetRootDrivers();

	friend unsigned  __stdcall ListDriver(void * pParam);//列出磁盘信息
	friend unsigned  __stdcall ListFiles(void * pParam);
	friend unsigned  __stdcall downLoadThread(void * pParam); 
	int  GetIconIndex(LPCTSTR lpszPath, BOOL bIsDir, BOOL bSelected = FALSE);
	CString m_CurrPath;//当前打开的文件路径
	CString m_SendPath;

public:
	afx_msg void OnNMDblclkFilelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnUp();
private:
	void getFilesByCurrPath(void);
	CString chooseDirectory(); //选中某个目录的对话框
	void fileDownload(CString remotePath, CString localPath, CString fileName);
	void directoryDownload(CString remotePath,CString localPath, CString fileName);
	void createDirectory(CString directoryPath);
	void getFiles(CString remotePath, char *pBuffer);
	void fileExecute(int hide);
	void sendFile(CString filePath);
public:
	afx_msg void OnFileFresh();
	afx_msg void OnFileDownload();
	afx_msg void OnFileDelete();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnFileExecute();
	afx_msg void OnFileExecuteHide();
	afx_msg void OnFileUpload();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEMANAGE_H__50D21541_1D71_45D7_A10E_C64F51981682__INCLUDED_)
