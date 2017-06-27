
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
	// �̹߳���
	std::deque<CSharePtr<CWorkerThread>> w_dqSpTrainThread;
	std::deque<CSharePtr<CWinThread>> w_dqSpWinThread;
public:
	// �߳����
	volatile LONG w_bProcessQuit;//�����˳���ǣ������ھ����Ƿ����LOG
	volatile LONG w_nRunningThread;//��������������̼߳���
	CCriticalSection w_csTasks;//ѵ���������ͬ��
	std::deque<CSharePtr<ITask>> w_dqSpTasks;

	afx_msg void OnBnClickedStartproc();

	void WriteMessage(CString strMsg);

protected:
	std::deque<CString> w_dqStrSrcFiles;

protected:
	bool m_bList1;	// ����ʹ��List1
	CListBox* m_pListA;
	CListBox* m_pListB;
	CListBox m_List1;
	CListBox m_List2;

protected:
	bool EnumSrcFiles(const CString& strFileDir, const CString strFilter);//����false��ʾĿ¼���������ļ�
	bool CheckOutputDirSilent(const CString& strOutputDir);
};
