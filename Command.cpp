#include "StdAfx.h"


#ifndef _HEAD_COMMAND_H
#define _HEAD_COMMAND_H
#include  "Command.h"
#endif

BOOL RecvData(SOCKET s,char *data,int len)
{
        if(len<=0)
             return TRUE;                  	char *pData=data;///��¼���ݴ洢��ַ
	int iLeftRecv=len;//����δ����ʣ�೤��
	int iHasRecv=0;//�Ѿ����յ����ݳ���
	int iRet=0;//ÿ�ν��յ����ݳ���
	if(len<=0)  return TRUE;

	while(1)
	{
		iRet=recv(s,pData,iLeftRecv,0);
		if(iRet==0||iRet==SOCKET_ERROR)
		{
					//int err = GetLastError();
					//CString str;
					//str.Format("���ճ��� �����룺%d", err);
					//::MessageBox(NULL, str, str, MB_OK);
				return FALSE;
		}

		iHasRecv+=iRet;
		iLeftRecv-=iRet;
		pData+=iRet;//�������ݴ洢��ַҲҪ�ı�
		if(iHasRecv>=len) break;//�ܷ��ͳ��ȴ��ڵ����ܳ��� ���˳�
	}
	return TRUE;
}

BOOL RecvMsg(SOCKET s,char *pBuf,LPMsgHead lpMsgHead)// ����ָ�� �Ƚ���ָ���ͷ�� ֮�������ϸ��Ϣ
{
	if(!RecvData(s,(char *)lpMsgHead,sizeof(MsgHead)))//�Ƚ�����Ϣ��ͷ�� 
		return FALSE;
	
	if(lpMsgHead->dwSize<=0)  return TRUE;

	if(!RecvData(s,pBuf,lpMsgHead->dwSize))
		return FALSE;
	return TRUE;
}



BOOL SendData(SOCKET s,char*  data ,int len)
{
	char *pData=data;///��¼���ݴ洢��ַ
	int iLeftSend=len;//����δ����ʣ�೤��
	int iHasSend=0;//�Ѿ����͵����ݳ���	
	int iRet=0;//ÿ�η��͵����ݳ���
	if(len<=0)  return TRUE;

	while(1)
	{
		iRet=send(s,pData,iLeftSend,0);
		if(iRet==0||iRet==SOCKET_ERROR) return FALSE;

		iHasSend+=iRet;
		iLeftSend-=iRet;
		pData+=iRet;//�������ݴ洢��ַҲҪ�ı�
		if(iHasSend>=len) break;//�ܷ��ͳ��ȴ��ڵ����ܳ��� ���˳�
	}
	return TRUE;
}

BOOL SendMsg(SOCKET s,char const *pBuf,LPMsgHead lpMsgHead)// ����ָ�� �ȷ���ָ���ͷ�� ֮������ϸ��Ϣ
{
	if(!SendData(s,(char *)lpMsgHead,sizeof(MsgHead)))//�ȷ�����Ϣ��ͷ�� 
		return FALSE;
	
	if(lpMsgHead->dwSize<=0)  return TRUE;

	if(!SendData(s,(char*)pBuf,lpMsgHead->dwSize))
		return FALSE;
	return TRUE;
}