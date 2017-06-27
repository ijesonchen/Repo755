
// SendSmsDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "atltypes.h"


// CSendSmsDlg �Ի���
class CSendSmsDlg : public CDialog
{
// ����
public:
	CSendSmsDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SENDSMS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	// ��List�ؼ���һ�в�����Ϣ
	int ShowMsg(CStringA szMsgA);
};
