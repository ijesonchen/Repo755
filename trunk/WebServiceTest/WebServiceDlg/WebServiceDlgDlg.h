// WebServiceDlgDlg.h : 头文件
//

#pragma once


// CWebServiceDlgDlg 对话框
class CWebServiceDlgDlg : public CDialog
{
// 构造
public:
	CWebServiceDlgDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_WEBSERVICEDLG_DIALOG };

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
