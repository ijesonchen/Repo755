// WebServiceDlgDlg.h : ͷ�ļ�
//

#pragma once


// CWebServiceDlgDlg �Ի���
class CWebServiceDlgDlg : public CDialog
{
// ����
public:
	CWebServiceDlgDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_WEBSERVICEDLG_DIALOG };

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
public:
	afx_msg void OnBnClickedButton1();
	CString m_service;
	CString m_addr;
	CString m_recid;
	CString m_clueid;
	CString m_user;
	CString m_filepath;
	CString m_result;
	CString m_method;
	int m_count;
	BOOL m_isDynamic;
	BOOL m_isObj;
	BOOL m_isString;
	BOOL m_isDataSet;
};
