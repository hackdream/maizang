#include "StdAfx.h"


#ifndef _HEAD_COMMAND_H
#define _HEAD_COMMAND_H
#include  "Command.h"
#endif

BOOL RecvData(SOCKET s,char *data,int len)
{
        if(len<=0)
             return TRUE;                  	char *pData=data;///记录数据存储地址
	int iLeftRecv=len;//数据未接收剩余长度
	int iHasRecv=0;//已经接收的数据长度
	int iRet=0;//每次接收的数据长度
	if(len<=0)  return TRUE;

	while(1)
	{
		iRet=recv(s,pData,iLeftRecv,0);
		if(iRet==0||iRet==SOCKET_ERROR)
		{
					//int err = GetLastError();
					//CString str;
					//str.Format("接收出错 错误码：%d", err);
					//::MessageBox(NULL, str, str, MB_OK);
				return FALSE;
		}

		iHasRecv+=iRet;
		iLeftRecv-=iRet;
		pData+=iRet;//发送数据存储地址也要改变
		if(iHasRecv>=len) break;//总发送长度大于等于总长度 就退出
	}
	return TRUE;
}

BOOL RecvMsg(SOCKET s,char *pBuf,LPMsgHead lpMsgHead)// 对于指令 先接收指令的头部 之后接收详细信息
{
	if(!RecvData(s,(char *)lpMsgHead,sizeof(MsgHead)))//先接收消息的头部 
		return FALSE;
	
	if(lpMsgHead->dwSize<=0)  return TRUE;

	if(!RecvData(s,pBuf,lpMsgHead->dwSize))
		return FALSE;
	return TRUE;
}



BOOL SendData(SOCKET s,char*  data ,int len)
{
	char *pData=data;///记录数据存储地址
	int iLeftSend=len;//数据未发送剩余长度
	int iHasSend=0;//已经发送的数据长度	
	int iRet=0;//每次发送的数据长度
	if(len<=0)  return TRUE;

	while(1)
	{
		iRet=send(s,pData,iLeftSend,0);
		if(iRet==0||iRet==SOCKET_ERROR) return FALSE;

		iHasSend+=iRet;
		iLeftSend-=iRet;
		pData+=iRet;//发送数据存储地址也要改变
		if(iHasSend>=len) break;//总发送长度大于等于总长度 就退出
	}
	return TRUE;
}

BOOL SendMsg(SOCKET s,char const *pBuf,LPMsgHead lpMsgHead)// 对于指令 先发送指令的头部 之后发送详细信息
{
	if(!SendData(s,(char *)lpMsgHead,sizeof(MsgHead)))//先发送消息的头部 
		return FALSE;
	
	if(lpMsgHead->dwSize<=0)  return TRUE;

	if(!SendData(s,(char*)pBuf,lpMsgHead->dwSize))
		return FALSE;
	return TRUE;
}