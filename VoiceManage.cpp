// VoiceManage.cpp : 实现文件
//

#include "stdafx.h"
#include "maizang.h"
#include "VoiceManage.h"
#include "afxdialogex.h"

#include <mmeapi.h>
#include <Windows.h>
#include <conio.h>
#include <stdio.h>
#include "Command.h"
#pragma comment ( lib, "Winmm.lib" )

// VoiceManage 对话框

IMPLEMENT_DYNAMIC(VoiceManage, CDialogEx)

VoiceManage::VoiceManage(CWnd* pParent /*=NULL*/)
	: CDialogEx(VoiceManage::IDD, pParent)
{

}

void VoiceManage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
static const double		RecordTime						=	0.2;	// 录音长度（秒）
static const unsigned	SampleRate						=	16000;	// 比特率

const int buff_num=2;
#define	CANNELS							2
#define BITS_PER_SAMPLE					16
#define WAVEBUFFER_SIZE					((unsigned)(SampleRate*BITS_PER_SAMPLE/8*CANNELS*RecordTime))


void CALLBACK waveInProc (HWAVEIN hwi, UINT uMsg, UINT , DWORD_PTR dwParam1, DWORD_PTR dwParam2);
void CALLBACK waveOutProc (HWAVEOUT hwo, UINT uMsg, UINT, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
DWORD WINAPI Thread_WaveIn (LPVOID);
DWORD WINAPI Thread_WaveOut (LPVOID);

HWAVEIN wIn = 0;
WAVEFORMATEX wf = {0};
HWAVEOUT wOut = 0;

int DataSize = 0;

WAVEHDR waveInHeader[16] = {0};
WAVEHDR waveOutHeader[16] = {0};
unsigned waveInSafeBuffIndex = 0;


int ReadyIndex, NextIndex, SafeIndex;

HANDLE Event_WaveInFull;
HANDLE Event_WaveOutDone;
HANDLE T_WaveIn;
HANDLE T_WaveOut;

#define RECV_PORT 8010
SOCKET socket_server;
int init_socket()
{
	WSADATA WSAdata;
	if(WSAStartup(MAKEWORD(1,1),&WSAdata)!=0)
	{
		printf("初始化失败\n");
		return -1;
	}
	return 0;
}
int create_socket()
{
	socket_server=socket(AF_INET,SOCK_STREAM,0);
	if(socket_server == SOCKET_ERROR)
	{
		printf("创建监听socket失败\n");
		WSACleanup();
		return -1;
	}
	sockaddr_in ServerAddr;
	ServerAddr.sin_family      = AF_INET;
	ServerAddr.sin_port        = htons(8100);
	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(socket_server,(struct sockaddr*)&ServerAddr,sizeof(struct sockaddr_in))==SOCKET_ERROR)
	{
		printf("socket监听绑定失败\n");
		return -1;
	}
	if(listen(socket_server,50) < 0)
	{
		printf("socket监听失败\n");
		return -1;
	}
	return 0;
}
int DoAudio ()
{
	init_socket();
	create_socket();
	wf.cbSize = sizeof(wf);
	wf.wFormatTag = WAVE_FORMAT_PCM;
	wf.nChannels = CANNELS;
	wf.wBitsPerSample = BITS_PER_SAMPLE;
	wf.nBlockAlign = wf.nChannels  * wf.wBitsPerSample / 8;
	wf.nSamplesPerSec = SampleRate;
	wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;

	int retv = 0;

	{
		ZeroMemory (&waveInHeader, sizeof(waveInHeader));
		DataSize = WAVEBUFFER_SIZE;
		int i;
		for(i=0;i<buff_num;i++)
		{
			waveInHeader[i].dwBufferLength = WAVEBUFFER_SIZE;
	    	waveInHeader[i].dwFlags = 0;
			waveInHeader[i].lpData = new char[WAVEBUFFER_SIZE];
		}
		

		retv = waveInOpen (&wIn, WAVE_MAPPER, &wf, (DWORD_PTR)waveInProc, 0, CALLBACK_FUNCTION);
		for(i=0;i<buff_num;i++)
			waveInPrepareHeader (wIn, &waveInHeader[i], sizeof( WAVEHDR ));

		Event_WaveInFull = CreateEvent ( 0 , 0 , 0 , 0 );
		T_WaveIn = CreateThread ( 0, 0, Thread_WaveIn, 0, 0, 0);
	}

	waveInStart (wIn);
	//SetEvent ( Event_WaveInFull );
	SetEvent ( Event_WaveOutDone );
	return retv;
}
void CALLBACK waveInProc (HWAVEIN hwi, UINT uMsg, UINT, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	if ( uMsg == WIM_DATA)
	{
		SetEvent ( Event_WaveInFull );
	}
}
SOCKET socket_client[50];
int num = 0;
int tm = 0;
DWORD WINAPI Thread_WaveIn (LPVOID)
{
	SetThreadPriority (GetCurrentThread (), THREAD_PRIORITY_TIME_CRITICAL);
	static int index = 0;
	
	sockaddr_in ClientAddr;
	int Addrlen = sizeof(sockaddr_in);
	socket_client[num]=accept(socket_server,(struct sockaddr FAR *)&ClientAddr,&Addrlen);
	int nowNum = num;
	num++;
	while ( 1 )
	{
		if (index-1==-1)
		{
			char x[100000]="0";
			waveInAddBuffer ( wIn , &waveInHeader[buff_num-1] , sizeof( WAVEHDR ) );
			WaitForSingleObject ( Event_WaveInFull , -1 );
			memcpy ( x , waveInHeader[buff_num-1].lpData , WAVEBUFFER_SIZE );
			if(send(socket_client[nowNum],(char*)&x,WAVEBUFFER_SIZE,0)<=0)
			{
				closesocket(socket_client[nowNum]);
				return 0; 
			}
		}
		else
		{
			char xx[100000]="0";
			waveInAddBuffer ( wIn , &waveInHeader[index-1] , sizeof( WAVEHDR ) );
			WaitForSingleObject ( Event_WaveInFull , -1 );
			memcpy ( xx , waveInHeader[index-1].lpData , WAVEBUFFER_SIZE );
			if(send(socket_client[nowNum],(char*)&xx,WAVEBUFFER_SIZE,0)<=0)
			{
				closesocket(socket_client[nowNum]);
				return 0; 
			}
		} 
		index = (index+1)%buff_num;
		if (tm==1)
		{
			closesocket(socket_client[nowNum]);
			closesocket(socket_server);
			TerminateThread(T_WaveIn, 0);
			int i;
			for(i=0;i<buff_num;i++)
			{
				delete waveInHeader[i].lpData;
			}
			waveInClose(wIn);
		}
	}
}
void VoiceManage::VoiceTransmit()
{
	
	SetPriorityClass ( GetCurrentProcess () , REALTIME_PRIORITY_CLASS );
	DoAudio ();
	tm = 0;
	num = 0;

}

BEGIN_MESSAGE_MAP(VoiceManage, CDialogEx)
	ON_BN_CLICKED(IDC_DISCONNECT, &VoiceManage::OnBnClickedDisconnect)
	ON_BN_CLICKED(IDCANCEL, &VoiceManage::OnBnClickedCancel)
END_MESSAGE_MAP()



void VoiceManage::OnBnClickedDisconnect()
{
	CDialogEx::OnCancel();
	// TODO: 在此添加控件通知处理程序代码
}


void VoiceManage::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
VoiceManage::~VoiceManage()
{
	/*TerminateThread(T_WaveIn, 0);
	TerminateThread(T_WaveOut, 0);*/
	tm = 1;
}