#pragma once

#ifndef _HEAD_COMMAND_H
#define _HEAD_COMMAND_H
#include  "Command.h" 
#endif
// CScreenDlg �Ի���

class CScreenDlg : public CDialog
{
	DECLARE_DYNAMIC(CScreenDlg)

public:
	CScreenDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CScreenDlg();
    void SetSCreenSocket(SOCKET sock);//��ȡScreendlg�뱻�ض�ͨ�ŵ��׽���
	void SetChoseSocket(SOCKET sock);//ѡ�е���Ŀ��Ӧ��socket
	void GetFirstScreen();

	friend unsigned  __stdcall GetScreenThread(void * pParam);
	//void GetBmpSizeAndMemory();
// �Ի�������
	enum { IDD = IDD_SCREEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	//����
	SOCKET m_MainSocket;
	SOCKET m_ChoseSocket;
	BITMAPFILEHEADER   bmfHdr; 
	BYTE * pData;// ����λͼ������
	BYTE * pChanged;//ͼ��仯������
	BYTE * pCompress;
	int m_ChangedOff;//��ȡ�仯������ʱʹ�õ�ƫ����
	int m_DataOff;// ���±���֮ǰ����bufferʱ�õ�ƫ����
	BOOL IsFirst;//�ж��ǲ���ͼ��ĵ�һ֡
	char strFilePath[111] ;// λͼͼ�񱣴��λ��
    int m_InfoSize;
	int m_ScreenBits;//Ҫ���ȡ��ͼ���λ��
	CScrollBar m_VScrollBar;//��ֱ������
	int m_VScrollPos;//��ֱ������λ��
	CScrollBar m_HScrollBar;//ˮƽ������
	int m_HScrollPos;//ˮƽ������λ��
	BOOL m_bCtrlScreen; //�Ƿ������Ļ
	HANDLE m_hScreenThread;//��Ļ�߳�


	// ����
	HBITMAP GetBitmapFromData();
	void ChangeScreenBits();//�ı�λͼ����λ��
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
