#pragma once

// VoiceManage �Ի���

class VoiceManage : public CDialogEx
{
	DECLARE_DYNAMIC(VoiceManage)

public:
	VoiceManage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~VoiceManage();

	// �Ի�������
	enum { IDD = IDD_VOICE };
	void VoiceTransmit();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedDisconnect();
	afx_msg void OnBnClickedCancel();
};
