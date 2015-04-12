#include<winsock2.h>
#include<mstcpip.h>
#include <vector>
#include <string>
using std::vector;
using std::string;
#define SOCKET_CONNECT WM_USER + 1//���������Ϊ����һ��ֵ ����������ϵͳ��Ϣʱ������
#define CMD_HEARTBEAT WM_USER + 2   //������
#define CMD_FILEMANAGE WM_USER + 3 //�ļ����������
#define SOKCET_FILEMANAGE WM_USER + 4 //�ļ�����
#define CMD_FILEDRIVER WM_USER + 5 //��ȡ������Ϣ��ָ��
#define CMD_SUCCEED WM_USER + 6 //��Ӧ�ɹ�
#define CMD_FILEDIRECTORY WM_USER + 7//��ȡ����Ŀ¼��Ϣ������
#define CMD_DIRFLODERERR  WM_USER + 8 //�ļ��в��ɶ�
#define CMD_SCREEN_REQUEST WM_USER + 888  //��Ļ��������
#define CMD_SCREENDLG_SHOW WM_USER+ + 889//��ʾ��Ļ����
#define CMD_GETFIRST_SCREEN WM_USER + 890 //������Ļ��һ֡
#define CMD_CHANGE_SCREEN_BITS WM_USER + 892 //�ı�λͼ����λ��
#define CMD_PROCESS WM_USER + 4
#define CMD_FAILED WM_USER + 9 //��Ӧʧ��

// ��Ļģ�����
#define CMD_CTRLALTDEL   WM_USER + 893 // Ctrl + Alt + del
#define CMD_KEYDOWN      WM_USER + 894 //WM_KEYDOWN 
#define CMD_KEYUP        WM_USER + 895 //WM_KEYUP
#define CMD_MOUSEMOVE    WM_USER + 896//WM_MOUSEMOVE
#define CMD_LBUTTONDOWN  WM_USER + 897//WM_LBUTTONDOWN
#define CMD_LBUTTONUP    WM_USER + 898//WM_LBUTTONUP
#define CMD_LBUTTONDBLCLK WM_USER + 899//WM_LBUTTONDBLCLK
#define CMD_RBUTTONDOWN   WM_USER + 900//WM_RBUTTONDOWN   
#define CMD_RBUTTONUP     WM_USER + 901//WM_RBUTTONUP
#define CMD_RBUTTONDBLCLK WM_USER + 902//WM_RBUTTONDBLCLK
#define CMD_SCREEN_PRESS WM_USER + 903


//cmd  ���������P

#define CMD_CMD_SHELL_REQUEST WM_USER + 910
#define CMD_SHELLDLG_SHOW WM_USER + 911
#define CMD_PROCESS_SHOW WM_USER + 913
#define CMD_REFRESH WM_USER+915
#define CMD_ENDTASK WM_USER+916

// 912 �Ѿ�ռ��

//  file  �ļ��������
#define CMD_GETFILE WM_USER + 1000
#define	MAX_FILE_DATA_BUFFER_SIZE			1024 * 8 // �ļ�һ����������ݳ���  
#define CMD_FILE_DELETE WM_USER + 1001
#define CMD_FILE_EXECUTE WM_USER + 1002
#define CMD_FILE_TO_CLIENT WM_USER + 1003

//���ڹ���
#define CMD_WINDOW_MANAGER_DLG_SHOW WM_USER + 1100
#define CMD_SHOW_WINDOW_LIST WM_USER + 1101
#define CMD_WINDOW_DELETE WM_USER + 1102

//��Ϣ����ͷ

struct ProcsInfo{
	char szFileName[MAX_PATH];
	char dwProcessID[15];
	char szFilePath[MAX_PATH];
};

typedef struct tagMsgHead
{
	DWORD dwCmd;
	DWORD dwSize;//С�ڵ���0 ��ʾ����Ϣ����  ֻ��������ͷ ����0 ˵������������Ϣ��Ҫ����  
	DWORD dwExtend1;//������Ϣ1 ����
	DWORD dwExtend2;//������Ϣ2 ��������
}MsgHead,*LPMsgHead;


typedef struct tagSysInfo//��õ��Ե�ϵͳ��Ϣ
{
	char computerName[64];        
	char memorySize[32];
	char osVersion[64];
	char cpuInfo[32];
	char hasCamera[10];
}SysInfo, *pSysInfo;

//Ӳ����Ϣ�ṹ��
typedef struct tagDriver
{
	char driver[4];     //�̷�����
	char display[64];   //����
	int  drivertype;    //������
}DriverInfo, *LPDriverInfo;

//�洢��õ��ļ���Ϣ�Ľṹ��
typedef struct tagFileInfo
{
    int  iType;         //1-Ŀ¼��2-�ļ���3-��ЧĿ¼
	char cFileName[261]; //�ļ���
	char cAttrib[32];   //�ļ�����
	char cTime[32];     //ʱ��
	char cSize[32];     //�ļ���С	
}FileInfo,*LPFileInfo;


typedef struct tagWindowInfo{
	DWORD dwProcessID;
	char  strTitle[512];
}WindowInfo, *LPWindowInfo;



BOOL RecvData(SOCKET s,char *data,int len);
BOOL RecvMsg(SOCKET s,char  *pBuf,LPMsgHead lpMsgHead);

BOOL SendData(SOCKET S,char*  data ,int len);
BOOL SendMsg(SOCKET s,char const *pBuf,LPMsgHead lpMsgHead);