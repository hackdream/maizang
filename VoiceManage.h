#pragma once

// VoiceManage 对话框

class VoiceManage : public CDialogEx
{
	DECLARE_DYNAMIC(VoiceManage)

public:
	VoiceManage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~VoiceManage();

	// 对话框数据
	enum { IDD = IDD_VOICE };
	void VoiceTransmit();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedDisconnect();
	afx_msg void OnBnClickedCancel();
};
