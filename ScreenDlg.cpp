// ScreenDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "maizang.h"
#include "ScreenDlg.h"
#include "afxdialogex.h"

#include "./zlib.h"
#pragma comment(lib,"./zlib.lib")	//ͼ����������ѹ��ʹ��zlib�⺯��

// CScreenDlg �Ի���

IMPLEMENT_DYNAMIC(CScreenDlg, CDialog)

	CScreenDlg::CScreenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScreenDlg::IDD, pParent)
{
	pData = new BYTE [2048 * 1536 * 3 * 2];//Ϊ�ֱ���2048 * 1536 ��λΪ24λ��3���ֽڵĴ�С�� 2��  
	pChanged = new BYTE [2048 * 1536 * 3 * 2];//Ϊ�ֱ���2048 * 1536 ��λΪ24λ��3���ֽڵĴ�С�� 2��  
	pCompress = new BYTE [2048 * 1536 * 3 * 2];
	m_ScreenBits = 8;
}

CScreenDlg::~CScreenDlg()
{
	TerminateThread(m_hScreenThread, 0);
	if(pData != NULL) { delete pData; pData = NULL;}
	if(pChanged != NULL) { delete pChanged; pChanged = NULL;}
	if(pCompress != NULL) {delete pCompress;}
	MsgHead msgHead;
	msgHead.dwCmd = 88;
	SendMsg(m_MainSocket, NULL, &msgHead);
	if (m_hWnd != NULL)
	{ 
		DestroyWindow();
	}
	//	if(m_ChoseSocket != INVALID_SOCKET ) {  //�˴�������������Ϣ��ʹ�õ�socket ���Բ��ܹر�
	//	closesocket(m_ChoseSocket);
	//}
}

void CScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CScreenDlg, CDialog)

	ON_WM_PAINT()
	//	ON_WM_SIZE()
	//	ON_WM_CLOSE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_RADIO1, &CScreenDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CScreenDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CScreenDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CScreenDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, &CScreenDlg::OnBnClickedRadio5)

	//	ON_WM_CANCELMODE()
	//ON_WM_DESTROY()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON1, &CScreenDlg::OnBnClickedButton1)
	ON_WM_DESTROY()
	//ON_WM_NCDESTROY()
END_MESSAGE_MAP()


// CScreenDlg ��Ϣ�������



void CScreenDlg::SetSCreenSocket(SOCKET sock)
{
	m_MainSocket = sock;

	//�����߳�  GetScreenThread
	unsigned dwThreadId;
	m_hScreenThread =
		(HANDLE)_beginthreadex(NULL, 
		0,					 
		GetScreenThread,  
		this,   
		0, 		 
		&dwThreadId);
	//CloseHandle(m_hScreenThread);
}
void CScreenDlg::SetChoseSocket(SOCKET sock)
{
	m_ChoseSocket = sock;
}
unsigned __stdcall  GetScreenThread(void   *pvoid)
{

	CScreenDlg* This = (CScreenDlg*)pvoid;//���ݹ����ı����ڵ�thisָ�� 
	//This->GetFirstScreen();
	//return 0;
	if(This == NULL) return 0 ;
	MsgHead  MsgSend;
	MsgSend.dwCmd = CMD_GETFIRST_SCREEN;
	MsgSend.dwSize = 0;
	MsgSend.dwExtend1 = This->m_ScreenBits;
	if(!SendMsg( This->m_MainSocket, NULL, &MsgSend))//����������Ļ�����Ҫ��
	{
		::MessageBox(NULL, "��Ļ��������ʧ��", "����", MB_OK);
		closesocket( This->m_MainSocket);
		return 0;
	}
	//����Ϊ��Ļ�Ļ�ȡ�� һֱ��ȡ����ʾ  ֱ�����ղ���  
	MsgHead MsgRecv; 

	while( This->m_MainSocket != INVALID_SOCKET)
	{
		if(!RecvData( This->m_MainSocket, (char *)&MsgRecv, sizeof(MsgHead)))
		{
			//	::MessageBox(NULL, "��Ļ���ݽ��գ��������ʧ��", "����", MB_OK);
			closesocket( This->m_MainSocket);
			This->m_MainSocket = INVALID_SOCKET;
			return 0;
		}
		This->bmfHdr.bfType = 0x4D42;  // "BM"  	// ����λͼ�ļ�ͷ 
		This->bmfHdr.bfSize = MsgRecv.dwExtend1;
		This->bmfHdr.bfReserved1 = 0;
		This->bmfHdr.bfReserved2 = 0;
		This->bmfHdr.bfOffBits = MsgRecv.dwExtend2;
		This->m_InfoSize = MsgRecv.dwExtend2 - sizeof(BITMAPFILEHEADER);//m_InfoSizeΪinfo��Ϣͷ�͵�ɫ��Ĵ�С�ĺ�


		int lenthcompress = MsgRecv.dwSize;
		int lenthUncompress = MsgRecv.dwExtend1 - sizeof(BITMAPFILEHEADER);

		if(!RecvData( This->m_MainSocket,(char *)This->pCompress,lenthcompress))
		{
			closesocket( This->m_MainSocket);
			This->m_MainSocket = INVALID_SOCKET;
			return 0;			    
		}

		This->IsFirst = This->pCompress[0];//���ݹ����ĵ�һ���ֽڱ���Ƿ��ǵ�һ֡

		DWORD sz = 2048 * 1536 * 3 * 2;  //���sz��������ֵ  ��Ȼ�����
		if( This->pChanged == NULL ||  This->pData == NULL)  // ���ڹر�ʱ�����߳̿����Ծ�����һС��ʱ�� �����pDataΪNULL ���˳��߳� ���򵯳������
		{
			return 0;
		}
		if(This->pCompress == NULL) return 0;
		uncompress( This->pChanged,
			&sz,//ԭʼ����
			This->pCompress + 1,//+1 ����Ϊ�������ĵ�һ���ֽڲ���ѹ�������� �����Ƿ��ǵ�һ֡���ı�����λ����Ҳ���һ֡��
			lenthcompress);//ѹ���󳤶�
		if( This->IsFirst)
		{
			memcpy( This->pData,  This->pChanged, sz);
		}
		else
		{
			This->m_ChangedOff =0;
			memcpy( This->pData,  This->pChanged,  This->m_InfoSize); //���µ���Ϣͷ�͵�ɫ�帴�ƽ�data 
			BITMAPINFOHEADER bi;
			memcpy(&bi,  This->pData, sizeof(bi));//���ļ���Ϣͷ���浽bi

			This->m_ChangedOff =  This->m_InfoSize;
			RECT rect;
			memcpy(&rect,  This->pChanged +  This->m_ChangedOff, sizeof(rect));//�õ��仯�����ݵķ�Χ 
			This->m_ChangedOff += sizeof(rect);

			This->m_DataOff =  This->m_InfoSize;
			BYTE *Dst, *Src; 
			int BitSz;
			BitSz = (rect.right - rect.left + 1) * bi.biBitCount / 8;//ÿ����Ҫ���µ�������
			int OneLineByte =  bi.biSizeImage / bi.biHeight;
			for(int i = rect.top; i <= rect.bottom ; i++) 
			{						
				Src =  This->pChanged +  This->m_ChangedOff;
				Dst =  This->pData +  This->m_InfoSize + i * OneLineByte + rect.left * bi.biBitCount / 8;
				memcpy(Dst, Src, BitSz);//����ÿһ�б仯������
				This->m_ChangedOff += BitSz;
			}
		}

		//��bitmap����д���ļ���  ����һ��bmpͼ���ļ�
		//strcpy(strFilePath,"E://hehe.bmp");
		//if(hFile !=  INVALID_HANDLE_VALUE)  CloseHandle(hFile); 
		//hFile = CreateFile(strFilePath , GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);//����λͼ�ļ�   
		//DWORD dwWritten;
		//WriteFile(hFile, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);	// д��λͼ�ļ�ͷ
		//WriteFile(hFile, (LPSTR)pData, bmfHdr.bfSize, &dwWritten, NULL);// д��λͼ�ļ���������
		//CloseHandle(hFile); 	

		This->Invalidate(TRUE);
		This->IsFirst = FALSE;
		Sleep(10);
	}
	return 0;
}
BOOL CScreenDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_bCtrlScreen = FALSE;
	m_VScrollPos = 0;
	m_HScrollPos = 0;
	CRect rect;
	GetClientRect(rect);
	rect.top += 18;
	rect.left = rect.right - 15;
	rect.bottom -= 15;
	m_VScrollBar.Create(WS_CHILD | WS_VISIBLE | SB_VERT,//�ӿؼ� �ɼ� ��ֱ������
		rect, this , WM_USER + 1000 ); 

	GetClientRect(rect);
	rect.top = rect.bottom - 15;
	rect.right = rect.right - 15;
	m_HScrollBar.Create(WS_CHILD | WS_VISIBLE | SB_HORZ,//�ӿؼ� �ɼ� ˮƽ������
		rect, this , WM_USER + 1001 ); 
	m_VScrollBar.SetScrollPos(m_VScrollPos);
	m_HScrollBar.SetScrollPos(m_HScrollPos);


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CScreenDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	CRect rect;
	GetClientRect(rect);
	rect.top += 18;
	rect.left = rect.right - 15;
	rect.bottom -= 15;
	m_VScrollBar.MoveWindow(rect); 

	GetClientRect(rect);
	rect.top = rect.bottom - 15;
	rect.right = rect.right - 15;
	m_HScrollBar.MoveWindow(rect); 
}






BOOL CScreenDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//˫�����ֹ��˸

	CRect rect;
	CDC DCmem;
	DCmem.CreateCompatibleDC(pDC);//������pDC���ݵ��ڴ�DC
	CBitmap bitmap;
	//����Ҳ���Դ�һ���ļ�������bitmapͼ��  ��ע���е����
	bitmap.m_hObject  = GetBitmapFromData();
	CBitmap *pOldBit=DCmem.SelectObject(&bitmap);//��ͼ������ڴ�DC
	GetClientRect(&rect);

	BITMAP   bm;  
	GetObject(bitmap.m_hObject,sizeof(bm),&bm);  //�õ�bitmap ��Ϣ

	int Height, Width;//��������ʾͼ��Ĵ��ڷ�Χ
	Width = rect.right   - 15 - rect.left;//-15����Ϊ�������Ĵ�С������Ϊ15
	Height = rect.bottom - 18 - 15 - rect.top;//ͼ��ؼ���������ʾͼ��ķ�Χ ��Ϊ�й����� ������һ���ķ�Χ Ҫ -15  18�ǰ�ť�Ŀ��
	int restH = bm.bmHeight - Height;
	if(restH < 0) restH = 0;
	int restW = bm.bmWidth - Width;
	if(restW < 0) restW = 0;

	//ˮƽ����������
	SCROLLINFO Hinfo;//���û����һЩ��Ϣ�� ע���ǻ���ģ� ���ǹ����� �����϶����Ǹ�С��� 
	Hinfo.cbSize = sizeof(SCROLLINFO);  //�̶���  ���ṹ�Ĵ�С    
	Hinfo.fMask = SIF_ALL;  //һЩ����  
	Hinfo.nMin = 0;   //�������ܵ������С��Χ
	Hinfo.nMax = bm.bmWidth;//�������ܵ�������Χ ������һ��ͼ�� ��Ϊͼ��Ŀ��
	Hinfo.nPage = 10;//����Ĵ�С
	Hinfo.nPos = m_HScrollPos;//�����λ��
	m_HScrollBar.SetScrollInfo(&Hinfo);
	m_HScrollBar.SetScrollRange(0, restW);

	//��ֱ����������
	SCROLLINFO info;//���û����һЩ��Ϣ�� ע���ǻ���ģ� ���ǹ����� �����϶����Ǹ�С��� 
	info.cbSize = sizeof(SCROLLINFO);  //�̶���  ���ṹ�Ĵ�С    
	info.fMask = SIF_ALL;  //һЩ����  
	info.nMin = 0;   //�������ܵ������С��Χ
	info.nMax = bm.bmHeight;//�������ܵ�������Χ ������һ��ͼ�� ��Ϊͼ��ĸ߶�
	info.nPage = 10;//����Ĵ�С
	info.nPos = m_VScrollPos;//�����λ��
	m_VScrollBar.SetScrollInfo(&info);
	m_VScrollBar.SetScrollRange(0, restH);//������ʱ ��֪��Ϊʲô���ܹ��������� ���������˹�����Χ



	BitBlt(pDC->GetSafeHdc(), rect.left, rect.top + 18, rect.right -15 , rect.bottom - 15 , DCmem, m_HScrollPos, m_VScrollPos, SRCCOPY);

	GetClientRect(rect);
	rect.bottom = 18;
	COLORREF color=RGB(174,221,129);
	pDC->FillSolidRect(rect,color);//ʹ��ĳ����ɫ���ͻ�����ͷһĴָ�ֵĳ��������򣬲�Ȼ����а�ť�ؼ��Ļ�����ť�ؼ��ܱ߻ᱻ�����Ļ��ͼ��		 

	DCmem.DeleteDC();     //ɾ��DC
	bitmap.DeleteObject(); //ɾ��λͼ
	ReleaseDC(pDC);//pDC->GetSafeHdc()����Ҫ�ͷ� ��ΪpDCָ��dc�ͷź� ���ڴ��hdcҲ���ͷŵ�
	return TRUE;
}


HBITMAP CScreenDlg::GetBitmapFromData()
{
	HBITMAP hBitmap;
	PBITMAPINFO lpBmpInfo; //λͼ��Ϣ

	lpBmpInfo = PBITMAPINFO(pData);

	HDC hDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	// ����DDBλͼ
	hBitmap = CreateDIBitmap(
		hDC,
		&lpBmpInfo->bmiHeader,
		CBM_INIT,
		pData + m_InfoSize,
		lpBmpInfo,
		DIB_RGB_COLORS) ;

	DeleteDC(hDC);

	return hBitmap;
}

void CScreenDlg::OnBnClickedRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ScreenBits = 1;
	ChangeScreenBits();
}


void CScreenDlg::OnBnClickedRadio2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ScreenBits = 4;
	ChangeScreenBits();
}


void CScreenDlg::OnBnClickedRadio3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ScreenBits = 8;
	ChangeScreenBits();
}


void CScreenDlg::OnBnClickedRadio4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ScreenBits = 16;
	ChangeScreenBits();
}


void CScreenDlg::OnBnClickedRadio5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ScreenBits = 24;
	ChangeScreenBits();
}


void CScreenDlg::ChangeScreenBits()
{
	MsgHead MsgSend;
	MsgSend.dwCmd = CMD_CHANGE_SCREEN_BITS;//�ı�����λ��
	MsgSend.dwSize = 0;
	MsgSend.dwExtend1 = m_ScreenBits;
	if(!SendMsg(m_MainSocket, NULL, &MsgSend)) 
	{
		::MessageBox(NULL, "�ı���Ļ����λ��ʧ��", "����", MB_OK);
		closesocket(m_MainSocket);
		return ;
	}
	return ;
}










void CScreenDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int iPos = nPos;
	int TempPos = pScrollBar->GetScrollPos();
	int iMin = 0, iMax = 0;
	pScrollBar->GetScrollRange(&iMin, &iMax);
	switch (nSBCode)
	{
	case SB_THUMBTRACK://�϶�����
		if (iPos < TempPos && iPos >= iMin)
		{
			pScrollBar->SetScrollPos(iPos, TRUE);

		}
		else if (iPos > TempPos && iPos <= iMax)
		{
			pScrollBar->SetScrollPos(iPos, TRUE);

		}
		m_VScrollPos = pScrollBar->GetScrollPos();
		UpdateData(FALSE);
		break;
		//case SB_THUMBPOSITION://�϶�����
		//	pScrollBar->SetScrollPos(iPos, FALSE);
		//	
		//	break;
	case SB_LINEUP://����ϱߵļ�ͷ
		if(TempPos > iMin)
		{
			pScrollBar->SetScrollPos(TempPos-1, TRUE);
			m_VScrollPos = pScrollBar->GetScrollPos();
		}
		break;
	case SB_LINEDOWN://����±ߵļ�ͷ
		if(TempPos < iMax)
		{
			pScrollBar->SetScrollPos(TempPos+1, TRUE);
			m_VScrollPos = pScrollBar->GetScrollPos();
		}
		break;
	case SB_PAGEUP://����������Ϸ��հ�
		if(TempPos > iMin)
		{
			pScrollBar->SetScrollPos(TempPos-1, TRUE);
			m_VScrollPos = pScrollBar->GetScrollPos();
		}
		break;
	case SB_PAGEDOWN://����������·��հ�
		if(TempPos < iMax)
		{
			pScrollBar->SetScrollPos(TempPos+1, TRUE);
			m_VScrollPos = pScrollBar->GetScrollPos();
		}
		break;
	case SB_ENDSCROLL:
		m_VScrollPos = pScrollBar->GetScrollPos();
		UpdateData(FALSE);
		break;
	default:
		break;
	}
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CScreenDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int iPos = nPos;
	int TempPos = pScrollBar->GetScrollPos();
	int iMin = 0, iMax = 0;
	pScrollBar->GetScrollRange(&iMin, &iMax);
	switch (nSBCode)
	{
	case SB_THUMBTRACK://�϶�����
		if (iPos < TempPos && iPos >= iMin)
		{
			pScrollBar->SetScrollPos(iPos, TRUE);

		}
		else if (iPos > TempPos && iPos <= iMax)	
		{
			pScrollBar->SetScrollPos(iPos, TRUE);

		}
		m_HScrollPos = pScrollBar->GetScrollPos();
		UpdateData(FALSE);
		break;
		//case SB_THUMBPOSITION://�϶�����
		//	pScrollBar->SetScrollPos(iPos, FALSE);
		//	
		//	break;
	case SB_LINEUP://����ϱߵļ�ͷ
		if(TempPos > iMin)
		{
			pScrollBar->SetScrollPos(TempPos-1, TRUE);
			m_HScrollPos = pScrollBar->GetScrollPos();
		}
		break;
	case SB_LINEDOWN://����±ߵļ�ͷ
		if(TempPos < iMax)
		{
			pScrollBar->SetScrollPos(TempPos+1, TRUE);
			m_HScrollPos = pScrollBar->GetScrollPos();
		}
		break;
	case SB_PAGEUP://����������Ϸ��հ�
		if(TempPos > iMin)
		{
			pScrollBar->SetScrollPos(TempPos-1, TRUE);
			m_HScrollPos = pScrollBar->GetScrollPos();
		}
		break;
	case SB_PAGEDOWN://����������·��հ�
		if(TempPos < iMax)
		{
			pScrollBar->SetScrollPos(TempPos+1, TRUE);
			m_HScrollPos = pScrollBar->GetScrollPos();
		}
		break;
	case SB_ENDSCROLL:
		m_HScrollPos = pScrollBar->GetScrollPos();
		UpdateData(FALSE);
		break;
	default:
		break;
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CScreenDlg::OnBnClickedButton1()
{

	if(m_bCtrlScreen == FALSE)
	{
		m_bCtrlScreen = TRUE;
		SetDlgItemText(IDC_BControl,"�����У�����ֹͣ");
	}
	else
	{
		m_bCtrlScreen = FALSE;
		SetDlgItemText(IDC_BControl,"����������");
	}

	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


BOOL CScreenDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	HWND hPicWnd = this->m_hWnd;
	if(m_bCtrlScreen && pMsg->message == WM_KEYDOWN  )
	{

		int nVirtKey = pMsg->wParam;

		MsgHead msgHead;
		msgHead.dwCmd = CMD_KEYDOWN;
		msgHead.dwSize = 0;
		msgHead.dwExtend1 = nVirtKey;

		if(SendMsg(m_ChoseSocket,NULL,&msgHead))
		{
			//	::MessageBox(NULL, "���ͳɹ�", "DD", MB_OK);
		}
		else 
		{
			::MessageBox(NULL, "����¼�����ʧ��", "����¼�����ʧ��", MB_OK);
		}


		return TRUE;
	}
	if(m_bCtrlScreen && pMsg->message == WM_KEYUP  )
	{
		int nVirtKey = pMsg->wParam;

		MsgHead msgHead;
		msgHead.dwCmd = CMD_KEYUP;
		msgHead.dwSize = 0;
		msgHead.dwExtend1 = nVirtKey;

		SendMsg(m_ChoseSocket,NULL,&msgHead);

		return TRUE;
	}

	if(m_bCtrlScreen && pMsg->hwnd == hPicWnd&& pMsg->message == WM_MOUSEMOVE )
	{

		CRect rc;
		GetClientRect(&rc);
		int nXPos = m_HScrollPos;
		int nYPos = m_VScrollPos;

		int dwX = nXPos + LOWORD(pMsg->lParam);
		int dwY =  nYPos + HIWORD(pMsg->lParam);
		dwY -= 15; 

		MsgHead msgHead;
		msgHead.dwCmd = CMD_MOUSEMOVE;
		msgHead.dwSize = 0;
		msgHead.dwExtend1 = dwX;
		msgHead.dwExtend2 = dwY;

		if(!SendMsg(m_ChoseSocket,NULL,&msgHead))
		{
			::MessageBox(NULL, "mouse move����ʧ��", "" ,MB_OK);
		}

		return TRUE;
	}

	if(m_bCtrlScreen && pMsg->hwnd == hPicWnd &&pMsg->message == WM_LBUTTONDOWN  )// ������
	{
		MsgHead msgHead;
		msgHead.dwCmd = CMD_LBUTTONDOWN;
		msgHead.dwSize = 0;

		if(SendMsg(m_ChoseSocket,NULL,&msgHead))
		{
			//::MessageBox(NULL, "����D�ɹ�", "DD", MB_OK);
		}
		else 
		{

		}

		return TRUE;
	}
	if(m_bCtrlScreen && pMsg->hwnd == hPicWnd && pMsg->message == WM_LBUTTONUP)
	{
		MsgHead msgHead;
		msgHead.dwCmd = CMD_LBUTTONUP;
		msgHead.dwSize = 0;

		SendMsg(m_ChoseSocket,NULL,&msgHead);

		return TRUE;
	}
	if(m_bCtrlScreen && pMsg->hwnd == hPicWnd && pMsg->message == WM_LBUTTONDBLCLK  )
	{
		MsgHead msgHead;
		msgHead.dwCmd = CMD_LBUTTONDBLCLK;
		msgHead.dwSize = 0;

		SendMsg(m_ChoseSocket,NULL,&msgHead);

		return TRUE;
	}
	if(m_bCtrlScreen && pMsg->hwnd == hPicWnd && pMsg->message == WM_RBUTTONDOWN )
	{
		MsgHead msgHead;
		msgHead.dwCmd = CMD_RBUTTONDOWN;
		msgHead.dwSize = 0;

		SendMsg(m_ChoseSocket,NULL,&msgHead);

		return TRUE;
	}
	if(m_bCtrlScreen && pMsg->hwnd == hPicWnd && pMsg->message == WM_RBUTTONUP )
	{
		MsgHead msgHead;
		msgHead.dwCmd = CMD_RBUTTONUP;
		msgHead.dwSize = 0;

		SendMsg(m_ChoseSocket,NULL,&msgHead);

		return TRUE;
	}
	if(m_bCtrlScreen && pMsg->hwnd == hPicWnd && pMsg->message == WM_RBUTTONDBLCLK )
	{
		MsgHead msgHead;
		msgHead.dwCmd = CMD_RBUTTONDBLCLK;
		msgHead.dwSize = 0;

		SendMsg(m_ChoseSocket,NULL,&msgHead);

		return TRUE;
	}	

	return CDialog::PreTranslateMessage(pMsg);
}


void CScreenDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	//��ģʽ�Ի�����Ҫ�������ٶԻ���
	//::MessageBox(NULL, "hehe", "haha", MB_OK);
	if(pData != NULL)
	{
		delete[] pData;
		pData = NULL;
	}
	if(pChanged != NULL)
	{
		delete[] pChanged;
		pChanged = NULL;
	}


	if (m_hWnd != NULL)
	{

		DestroyWindow();
	}
}



void CScreenDlg::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialog::PostNcDestroy();
	delete this;
}


void CScreenDlg::GetFirstScreen()
{
	/*
	MsgHead  MsgSend;
	MsgSend.dwCmd = CMD_GETFIRST_SCREEN;
	MsgSend.dwSize = 0;
	MsgSend.dwExtend1 = m_ScreenBits;
	if(!SendMsg(m_MainSocket, NULL, &MsgSend))//����������Ļ�����Ҫ��
	{
	::MessageBox(NULL, "��Ļ��������ʧ��", "����", MB_OK);
	closesocket(m_MainSocket);
	return ;
	}
	//����Ϊ��Ļ�Ļ�ȡ�� һֱ��ȡ����ʾ  ֱ�����ղ���  
	MsgHead MsgRecv; 

	while(m_MainSocket != INVALID_SOCKET)
	{

	if(!RecvData(m_MainSocket, (char *)&MsgRecv, sizeof(MsgHead)))
	{
	//	::MessageBox(NULL, "��Ļ���ݽ��գ��������ʧ��", "����", MB_OK);
	closesocket(m_MainSocket);
	m_MainSocket = INVALID_SOCKET;
	return ;
	}
	bmfHdr.bfType = 0x4D42;  // "BM"  	// ����λͼ�ļ�ͷ 
	bmfHdr.bfSize = MsgRecv.dwExtend1;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = MsgRecv.dwExtend2;
	m_InfoSize = MsgRecv.dwExtend2 - sizeof(BITMAPFILEHEADER);//m_InfoSizeΪinfo��Ϣͷ�͵�ɫ��Ĵ�С�ĺ�


	int lenthcompress = MsgRecv.dwSize;
	int lenthUncompress = MsgRecv.dwExtend1 - sizeof(BITMAPFILEHEADER);
	BYTE* pCompress = new BYTE [lenthcompress];


	if(!RecvData(m_MainSocket,(char *)pCompress,lenthcompress))
	{
	if(pCompress != NULL ){
	delete [] pCompress;
	pCompress = NULL;
	}
	//	::MessageBox(NULL, "��Ļ���ݽ��գ����ݽ���ʧ��", "����", MB_OK);
	closesocket(m_MainSocket);
	m_MainSocket = INVALID_SOCKET;
	return ;			    
	}
	IsFirst = pCompress[0];//���ݹ����ĵ�һ���ֽڱ���Ƿ��ǵ�һ֡

	DWORD sz = 2048 * 1536 * 3 * 2;  //���sz��������ֵ  ��Ȼ�����
	if(pChanged == NULL || pData == NULL)  // ���ڹر�ʱ�����߳̿����Ծ�����һС��ʱ�� �����pDataΪNULL ���˳��߳� ���򵯳������
	{
	if(pCompress != NULL )
	{     delete [] pCompress;
	pCompress = NULL;
	}
	return ;
	}
	uncompress(pChanged,
	&sz,//ԭʼ����
	pCompress + 1,//+1 ����Ϊ�������ĵ�һ���ֽڲ���ѹ�������� �����Ƿ��ǵ�һ֡���ı�����λ����Ҳ���һ֡��
	lenthcompress);//ѹ���󳤶�
	if(IsFirst)
	{
	memcpy(pData, pChanged, sz);
	}
	else
	{
	m_ChangedOff =0;
	memcpy(pData, pChanged, m_InfoSize); //���µ���Ϣͷ�͵�ɫ�帴�ƽ�data 
	BITMAPINFOHEADER bi;
	memcpy(&bi, pData, sizeof(bi));//���ļ���Ϣͷ���浽bi

	m_ChangedOff = m_InfoSize;
	RECT rect;
	memcpy(&rect, pChanged + m_ChangedOff, sizeof(rect));//�õ��仯�����ݵķ�Χ 
	m_ChangedOff += sizeof(rect);

	m_DataOff = m_InfoSize;
	BYTE *Dst, *Src; 
	int BitSz;
	BitSz = (rect.right - rect.left + 1) * bi.biBitCount / 8;//ÿ����Ҫ���µ�������
	int OneLineByte =  bi.biSizeImage / bi.biHeight;
	for(int i = rect.top; i <= rect.bottom ; i++) 
	{						
	Src = pChanged + m_ChangedOff;
	Dst = pData + m_InfoSize + i * OneLineByte + rect.left * bi.biBitCount / 8;
	memcpy(Dst, Src, BitSz);//����ÿһ�б仯������
	m_ChangedOff += BitSz;
	}
	}
	delete [] pCompress;
	pCompress = NULL;

	//��bitmap����д���ļ���  ����һ��bmpͼ���ļ�
	//strcpy(strFilePath,"E://hehe.bmp");
	//if(hFile !=  INVALID_HANDLE_VALUE)  CloseHandle(hFile); 
	//hFile = CreateFile(strFilePath , GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);//����λͼ�ļ�   
	//DWORD dwWritten;
	//WriteFile(hFile, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);	// д��λͼ�ļ�ͷ
	//WriteFile(hFile, (LPSTR)pData, bmfHdr.bfSize, &dwWritten, NULL);// д��λͼ�ļ���������
	//CloseHandle(hFile); 	
	Invalidate(TRUE);
	IsFirst = FALSE;
	Sleep(10);
	}
	*/
}