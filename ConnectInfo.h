#pragma once
class ConnectInfo
{
public:
	ConnectInfo(void);
	~ConnectInfo(void);
	char ipAddress[15];  //����ip��ַ
	int port;      //���Ӷ˿�
	char note[50];		//���߱�ע������
	static ConnectInfo* getConnectInfo();//��ȡ������ʵ��
	static ConnectInfo *pConnectInfo;
};

