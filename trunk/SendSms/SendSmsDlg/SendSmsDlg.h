
// SendSmsDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "atltypes.h"


// CSendSmsDlg 对话框
class CSendSmsDlg : public CDialog
{
// 构造
public:
	CSendSmsDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SENDSMS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

protected:
	CListBox m_cMsgList;
	CComboBox m_cCommCombo;
	afx_msg void OnBnClickedConnectionButton();
	afx_msg void OnBnClickedSendButton();
	afx_msg void OnBnClickedReceiveButton();
	afx_msg void OnBnClickedDeleteButton();
	afx_msg void OnBnClickedDisconnectionButton();
	afx_msg void OnBnClickedInitButton();

	pFuncSms_Connection Sms_Connect;
	pFuncSms_Disconnection Sms_Disconn;
	pFuncSms_Send Sms_Send;
	pFuncSms_Receive Sms_Receive;
	pFuncSms_Delete Sms_Delete;
	HINSTANCE m_hSmsDll;
	// 在List控件第一行插入消息
	int ShowMsg(CStringA szMsgA);
};
