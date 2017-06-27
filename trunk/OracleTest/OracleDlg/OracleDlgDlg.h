
// OracleDlgDlg.h : header file
//

#pragma once


// COracleDlgDlg dialog
class COracleDlgDlg : public CDialog
{
// Construction
public:
	COracleDlgDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ORACLEDLG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	// db pointer

	_ConnectionPtr m_pConnection;
	_CommandPtr m_pCmd;
	_RecordsetPtr m_pRecord;

	std::deque<VocTaskInfo> m_dqVocTask;

public:
	afx_msg void OnBnClickedConnectdb();
	afx_msg void OnBnClickedReadTaskFile();
	// for dqVocTask index
	int m_nVocTaskIndex;
	afx_msg void OnBnClickedWriteDb();
	bool m_bConnected;
	bool m_bTaskRead;
	afx_msg void OnBnClickedSkipTask();
};
