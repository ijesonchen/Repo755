
// gSoapWebServiceDlg.h : ͷ�ļ�
//

#pragma once


// CgSoapWebServiceDlg �Ի���
class CgSoapWebServiceDlg : public CDialog
{
// ����
public:
	CgSoapWebServiceDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GSOAPWEBSERVICE_DIALOG };

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
	CString w_strRecid;
	CString w_strClueid;
	CString w_strUser;
	CString w_strFileName;
	CString w_strRemark;
	afx_msg void OnBnClickedButton1();
	void ConnectShare(const CString& strUserName, 
		const CString& strPassword,
		const CString& strDstPath);
};
