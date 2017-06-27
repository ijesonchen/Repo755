
// Hzmv2PcmMT_DlgDlg.h : header file
//

#pragma once


#include "Resource.h"
#include <deque>
using namespace std;
#include "SharePtr.h"
#include "ITask.h"
#include "afxwin.h"

#include "WorkerThread.h"


// CHzmv2PcmMT_DlgDlg dialog
class CHzmv2PcmMT_DlgDlg : public CDialog
{
// Construction
public:
	CHzmv2PcmMT_DlgDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_HZMV2PCMMT_DLG_DIALOG };

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
	bool w_bCoInited;
	virtual void PostNcDestroy();

protected:
	// 线程管理
	std::deque<CSharePtr<CWorkerThread>> w_dqSpTrainThread;
	std::deque<CSharePtr<CWinThread>> w_dqSpWinThread;
public:
	// 线程相关
	volatile LONG w_bProcessQuit;//程序退出标记，并用于决定是否输出LOG
	volatile LONG w_nRunningThread;//还在运行任务的线程计数
	CCriticalSection w_csTasks;//训练任务队列同步
	std::deque<CSharePtr<ITask>> w_dqSpTasks;

	afx_msg void OnBnClickedStartproc();

	void WriteMessage(CString strMsg);

protected:
	std::deque<CString> w_dqStrSrcFiles;

protected:
	bool m_bList1;	// 正在使用List1
	CListBox* m_pListA;
	CListBox* m_pListB;
	CListBox m_List1;
	CListBox m_List2;

protected:
	bool EnumSrcFiles(const CString& strFileDir, const CString strFilter);//返回false表示目录中无所需文件
	bool CheckOutputDirSilent(const CString& strOutputDir);
};
