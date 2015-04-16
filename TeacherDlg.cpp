// TeacherDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "maizang.h"
#include "TeacherDlg.h"
#include "afxdialogex.h"
#include "MessageBoxDlg.h"
#include "./zlib.h"
#pragma comment(lib,"./zlib.lib")	//ͼ����������ѹ��ʹ��zlib�⺯

// CTeacherDlg �Ի���

IMPLEMENT_DYNAMIC(CTeacherDlg, CDialogEx)

CTeacherDlg::CTeacherDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTeacherDlg::IDD, pParent)
{

}

CTeacherDlg::~CTeacherDlg()
{
}

void CTeacherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTeacherDlg, CDialogEx)
	ON_BN_CLICKED(IDC_ALL_OPEN_MESSAGEDLG, &CTeacherDlg::OnBnClickedAllOpenMessagedlg)
	ON_BN_CLICKED(IDC_ALL_SEND_SCREE, &CTeacherDlg::OnBnClickedAllSendScree)
END_MESSAGE_MAP()


// CTeacherDlg ��Ϣ�������


void CTeacherDlg::OnBnClickedAllOpenMessagedlg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMessageBoxDlg *pMessageBoxDlg = new CMessageBoxDlg;
	pMessageBoxDlg->pMaizangDlg = pMaizangDlg;
	pMessageBoxDlg->Create(IDD_MESSAGEBOX, GetDesktopWindow());//����һ����ģ̬�Ի���
	pMessageBoxDlg->ShowWindow(SW_SHOW);
	pMessageBoxDlg->setSendButton(FALSE);
	pMessageBoxDlg->setAllSendButton(TRUE);
	pMessageBoxDlg->UpdateData(TRUE);
}

int endScreen;
//���ͽ�ʦ����Ļ������������
void CTeacherDlg::OnBnClickedAllSendScree()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
    endScreen = 0;
	list<SOCKET> *pSockets = pMaizangDlg->getAllSocket();
	list<SOCKET>::iterator it;
	for(it = pSockets->begin(); it != pSockets->end(); it++){
		unsigned dwThreadId; 
		SOCKET sock = *it;
		HANDLE hThread = (HANDLE)_beginthreadex(NULL,				 
			0,					 
			SendScreenThread,  //����ThreadAccept�߳�
			&sock,
			0,
			&dwThreadId);
		CloseHandle(hThread);
	}
	
}


unsigned  __stdcall  SendScreenThread(void * pParam){
	SOCKET *pSocket = (SOCKET  *)pParam;

	MsgHead msgHead;
	msgHead.dwCmd = CMD_SHOW_RECV_SCREEN_DLG;
	msgHead.dwSize = 0;
	SendMsg(*pSocket, NULL, &msgHead);

	Sleep(100);
	SendScreen(pSocket);
	return 0;
}





DWORD SendScreen(LPVOID lparam)//�̴߳�����Ļ����
{

	DWORD *pParam   =   (DWORD   *)lparam; 
	SOCKET MainSocket =*pParam; 
	DWORD dwLastSend;

	int ScreenBits = 8;
	BYTE * pLastData = new BYTE [2048 * 1536 *3 * 2];//Ϊ�ֱ���2048 * 1536 ��λΪ24λ��3���ֽڵĴ�С�� 2��  
	BYTE * pChanged = new BYTE [2048 *1536 * 3 * 2];

	HWND hWnd = GetDesktopWindow();//�����Ļ��HWND.
	HDC hScreenDC = GetDC(hWnd);   //�����Ļ��HDC.

	RECT rect; 
	//�ú�������ָ�����ڵı߿���εĳߴ硣�óߴ����������Ļ�������Ͻǵ���Ļ���������
	GetWindowRect(hWnd,&rect);
	SIZE screensize;
	screensize.cx=rect.right-rect.left;
	screensize.cy=rect.bottom-rect.top;
	//CreateCompatibleBitmap�ú���������ָ�����豸hScreenDC������ص��豸���ݵ�λͼ��
	HBITMAP hBitmap = ::CreateCompatibleBitmap(hScreenDC,screensize.cx,screensize.cy);

	HDC MemDC = CreateCompatibleDC(hScreenDC);//��ǰ��Ļ��DC

	BOOL  IsFirst = TRUE;

	memset(pLastData, 0, 2048 * 1536 *3 * 2);

	IsFirst = TRUE;
	while(1)
	{
		if(endScreen) break;
		/*if(ScreenBits != g_ScreenBits)
		{
			ScreenBits = g_ScreenBits;
			IsFirst = TRUE;
		}*/
		HWND hWnd = GetDesktopWindow(); 
		GetWindowRect(hWnd,&rect);
		int ChangedSize = 0;//�ı��˵����ݵĴ�С

		//����Ļͼ���͵�MemDC�е�λͼ��
		HGDIOBJ hOldBMP = ::SelectObject(MemDC,hBitmap);
		::BitBlt(MemDC,0,0,screensize.cx,screensize.cy,hScreenDC,rect.left,rect.top,SRCCOPY);
		::SelectObject(MemDC,hOldBMP);
		/*********��λͼINFOHEADER ������ɫ�壩��ͼ������ ����pData***********************/
		dwLastSend = GetTickCount();
		HDC hDC =::CreateDC("DISPLAY",NULL,NULL,NULL);
		int iBits = ::GetDeviceCaps(hDC, BITSPIXEL) * ::GetDeviceCaps(hDC, PLANES);//��ǰ�ֱ�����ÿ��������ռ�ֽ���
		::DeleteDC(hDC);

		DWORD   dwPaletteSize=0;	//��ɫ���С�� λͼ�������ֽڴ�С 
		if (ScreenBits<= 8)		
			dwPaletteSize = (1 <<  ScreenBits) *	sizeof(RGBQUAD);	

		BITMAP  bm;        //λͼ���Խṹ
		::GetObject(hBitmap, sizeof(bm), (LPSTR)&bm);

		BITMAPINFOHEADER   bi;       //λͼ��Ϣͷ�ṹ     
		bi.biSize            = sizeof(BITMAPINFOHEADER);  
		bi.biWidth           = bm.bmWidth;
		bi.biHeight          = bm.bmHeight;
		bi.biPlanes          = 1;
		bi.biBitCount        = ScreenBits;
		bi.biCompression     = BI_RGB; //BI_RGB��ʾλͼû��ѹ��
		bi.biSizeImage       = 0;
		bi.biXPelsPerMeter   = 0;
		bi.biYPelsPerMeter   = 0;
		bi.biClrUsed         = 0;
		bi.biClrImportant    = 0;

		DWORD dwBmBitsSize = ((bm.bmWidth *  ScreenBits+31)/32) * 4 * bm.bmHeight;//λͼ���ݵĴ�С

		int DataPalInfoSize = dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER);//λͼ����FILEHDEAR��  ����3���ִ�С�ĺ�

		//	HANDLE hDib = ::GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));  //1Ϊλͼ���ݷ����ڴ�  
		LPBITMAPINFOHEADER pData = (LPBITMAPINFOHEADER)new BYTE [DataPalInfoSize];  //2
		//LPBITMAPINFOHEADER pData = (LPBITMAPINFOHEADER)GlobalLock(hDib);  //new BYTE [DataPalInfoSize];//3
		*pData = bi;//2
		HANDLE hPal = ::GetStockObject(DEFAULT_PALETTE);  // �����ɫ�� 
		HANDLE  hOldPal=NULL; 
		if (hPal) 
		{
			hDC = ::GetDC(NULL);
			hOldPal = SelectPalette(hDC,(HPALETTE)hPal, FALSE);
			RealizePalette(hDC);
		}
		//�����ݱ�����ָ��(LPSTR)lpbi + sizeof(BITMAPINFOHEADER)+dwPaletteSizeָ���λ��
		//::GetDIBits(hDC, hBitmap, 0, (UINT) bm.bmHeight,(LPSTR)pData + sizeof(BITMAPINFOHEADER)+dwPaletteSize,(LPBITMAPINFO)&bi,DIB_RGB_COLORS);// ��ȡ�õ�ɫ�����µ�����ֵ
		::GetDIBits(hDC, hBitmap, 0, (UINT) bm.bmHeight,(LPSTR)pData + sizeof(BITMAPINFOHEADER)+dwPaletteSize,(BITMAPINFO*)pData,DIB_RGB_COLORS);// ��ȡ�õ�ɫ�����µ�����ֵ

		//���ļ���Ϣͷ���浽bi
		bi = *pData;
		int DataSize = bi.biSizeImage;//λͼ�����ݵ��ֽ���  ����������
		//memcpy(pData, &bi, sizeof(bi));
		if (hOldPal)//�ָ���ɫ��
		{
			SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
			RealizePalette(hDC);
			::ReleaseDC(NULL, hDC);
		}	
		//(BITMAPINFO*)pData
		/****************************�Ƚϵõ��仯������***********************************/



		BYTE *p1, *p2;
		int OneLineByte = DataSize / bm.bmHeight;  //һ�е��ֽ���
		BYTE *NowScreenOff = (BYTE*)pData + sizeof(BITMAPINFOHEADER)+dwPaletteSize;
		BYTE *LastScreenOff = pLastData + sizeof(BITMAPINFOHEADER)+dwPaletteSize;
		if(!IsFirst)
		{

			rect.top = 99999; rect.left = 99999; rect.bottom = -99999; rect.right = -99999; 

			for(int i = 0; i< bm.bmHeight ; i += 5)//û���5�� ���һ������  ע�����ﵥλ������
			{
				for(int j = 0; j < OneLineByte; j += 2)//bm.bmWidthBytesһ�е��ֽ��� 
				{
					p1 = (BYTE*)(NowScreenOff + i * OneLineByte + j);
					p2 = (BYTE*)(LastScreenOff + i * OneLineByte + j);
					if(*p1 != *p2)
					{
						if(rect.top > i)
						{ 
							rect.top = i;
							rect.top =  rect.top > 5 ? (rect.top - 5) : 0; 
						}
						if(rect.bottom < i)
						{
							rect.bottom = i;
							if( i+ 5 >= bm.bmHeight) rect.bottom = bm.bmHeight -1;
						}

						//j / (g_ScreenBits/8) Ϊj�ֽ����ڵ����ص�ı��  leftӦ��΢��ǰЩ �ò��������΢������ȷ ����-2 
						if(rect.left > (j-2) * 8 /ScreenBits )//��ʵ��(j-2) / (ScreenBits/8)) 
						{
							rect.left = (j-2) * 8 /ScreenBits;
							if(rect.left < 0) rect.left = 0;
						}
						//left��΢����Щ�� ���Բ���2 ���Ҽ�1������ ע�������� rect����������
						if(rect.right < j * 8 /ScreenBits + 1)//��ʵ��j/(ScreenBits/8) +1)
						{
							rect.right = j * 8 /ScreenBits + 1;
							if(rect.right >= bm.bmWidth) rect.right = bm.bmWidth - 1;
						}


					}

				}
			}
			//�������Ĳ��Ǻܾ�׼  �������⼸����΢�����·�Χ 
			rect.top = rect.top - 5;
			if(rect.top < 0) rect.top =0;
			rect.bottom += 5;
			if(rect.bottom >= bm.bmHeight) rect.bottom = bm.bmHeight -1;
			rect.left -= 5;
			if(rect.left < 0) rect.left =0;
			rect.right += 5;
			if(rect.right >= bm.bmWidth) rect.right = bm.bmWidth - 1;

			//����bmp��Ϣͷ ��ɫ�� �Լ� �仯����Ĵ�С�����ݵ�pChanged
			memcpy(pChanged, pData, sizeof(BITMAPINFOHEADER)+dwPaletteSize);
			ChangedSize = sizeof(BITMAPINFOHEADER)+dwPaletteSize;
			memcpy(pChanged + ChangedSize, &rect, sizeof(rect));
			ChangedSize += sizeof(rect);
			BYTE *Dst, *Src;
			int BitSz;
			BitSz = (rect.right - rect.left + 1) * ScreenBits / 8;
			for(int i = rect.top; i <= rect.bottom  ; i++) 
			{
				Dst = pChanged + ChangedSize;
				Src = NowScreenOff + i * OneLineByte + rect.left * ScreenBits / 8;
				memcpy(Dst, Src, BitSz);//����ÿһ�б仯������
				ChangedSize += BitSz;
			}
			//һ�д�����Ϻ� �����֡��ͼ����������һ֡���бȽ�
			memcpy(pLastData, pData, DataPalInfoSize); //�����ݱ��浽pLastData
		}


		/*************************������Ϣ******************************/
		BITMAPFILEHEADER   bmfHdr; //λͼ�ļ�ͷ�ṹ     
		bmfHdr.bfType = 0x4D42;  // "BM"  	// ����λͼ�ļ�ͷ
		DWORD dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;  
		bmfHdr.bfSize = dwDIBSize;
		bmfHdr.bfReserved1 = 0;
		bmfHdr.bfReserved2 = 0;
		bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

		//ѹ����pDataCompressd
		DWORD CompressdSize;
		BYTE* pDataCompressd;
		if(!IsFirst)
		{
			CompressdSize = compressBound(ChangedSize); // ѹ����ĳ����ǲ��ᳬ��CompressdSize  
			pDataCompressd = new BYTE [CompressdSize];
			pDataCompressd[0] = IsFirst;
			compress(pDataCompressd + 1, &CompressdSize, (BYTE*)pChanged, ChangedSize);
		}
		else
		{
			CompressdSize = compressBound(DataPalInfoSize); // ѹ����ĳ����ǲ��ᳬ��CompressdSize  
			pDataCompressd = new BYTE [CompressdSize];
			pDataCompressd[0] = IsFirst;
			compress(pDataCompressd + 1, &CompressdSize, (BYTE*)pData, DataPalInfoSize);		 
		}
		MsgHead MsgSend;
		MsgSend.dwCmd = CMD_SCREEN_TO_SERVER;
		MsgSend.dwSize = CompressdSize + 1;
		MsgSend.dwExtend1 = bmfHdr.bfSize;
		MsgSend.dwExtend2 = bmfHdr.bfOffBits;
		if(!SendMsg(MainSocket, (char*)pDataCompressd, &MsgSend))
		{
			::DeleteObject(MemDC);
			::ReleaseDC(hWnd,hScreenDC);
			DeleteObject(hBitmap);
			if(pLastData != NULL)
			{
				delete [] pLastData;   pLastData = NULL;
			}
			if(pDataCompressd != NULL){
				delete [] pDataCompressd;  pDataCompressd = NULL;
			}
			if(pData != NULL){
				delete [] pData;  pData = NULL;
			}
			if(pChanged != NULL){
				delete[] pChanged; pChanged = NULL;
			}
			return 0;
		}
		IsFirst = FALSE;
		if(pDataCompressd != NULL){
			delete [] pDataCompressd;  pDataCompressd = NULL;
		}
		if(pData != NULL){
			delete[] pData; pData = NULL;
		}
		if ((GetTickCount() - dwLastSend) < 110)
			Sleep(100);
	}

	if(pLastData != NULL)
		delete [] pLastData;
	if(pChanged != NULL)
		delete [] pChanged;
	return 0;
}

