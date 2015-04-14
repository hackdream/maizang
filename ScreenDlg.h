#pragma once

#ifndef _HEAD_COMMAND_H
#define _HEAD_COMMAND_H
#include  "Command.h" 
#endif
// CScreenDlg 对话框

class CScreenDlg : public CDialog
{
	DECLARE_DYNAMIC(CScreenDlg)

public:
	CScreenDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CScreenDlg();
    void SetSCreenSocket(SOCKET sock);//获取Screendlg与被控端通信的套接字
	void SetChoseSocket(SOCKET sock);//选中的条目对应的socket
	void GetFirstScreen();

	friend unsigned  __stdcall GetScreenThread(void * pParam);
	//void GetBmpSizeAndMemory();
// 对话框数据
	enum { IDD = IDD_SCREEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	//变量
	SOCKET m_MainSocket;
	SOCKET m_ChoseSocket;
	BITMAPFILEHEADER   bmfHdr; 
	BYTE * pData;// 保存位图的数据
	BYTE * pChanged;//图像变化的数据
	BYTE * pCompress;
	int m_ChangedOff;//提取变化的数据时使用的偏移量
	int m_DataOff;// 更新保存之前数据buffer时用到偏移量
	BOOL IsFirst;//判断是不是图像的第一帧
	char strFilePath[111] ;// 位图图像保存的位置
    int m_InfoSize;
	int m_ScreenBits;//要求获取的图像的位数
	CScrollBar m_VScrollBar;//竖直滚动条
	int m_VScrollPos;//竖直滚动条位置
	CScrollBar m_HScrollBar;//水平滚动条
	int m_HScrollPos;//水平滚动条位置
	BOOL m_bCtrlScreen; //是否控制屏幕
	HANDLE m_hScreenThread;//屏幕线程


	// 函数
	HBITMAP GetBitmapFromData();
	void ChangeScreenBits();//改变位图像素位数
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnPaint();
//	afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg void OnClose();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio6();
	afx_msg void OnBnClickedRadio7();

 
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButton1();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnCancel();
//	virtual void PostNcDestroy();
//	afx_msg void OnDestroy();
//	virtual void OnOK();
//	afx_msg void OnNcDestroy();
	virtual void PostNcDestroy();
};
