
// MpgWatcherDoc.h : CMpgWatcherDoc ��Ľӿ�
//


#pragma once
#include "param.h"


class CMpgWatcherDoc : public CDocument
{
protected: // �������л�����
	CMpgWatcherDoc();
	DECLARE_DYNCREATE(CMpgWatcherDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CMpgWatcherDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnWatcherStart();
protected:
	virtual BOOL SaveModified();
	static UINT WINAPI ConvertThread(LPVOID pContext);
	static UINT WatcherThread(LPVOID pContext);
public:
	afx_msg void OnWatcherStop();
protected:
	HANDLE m_hConvertThread;
	HANDLE m_hWatcherThread;

public:
	// Running flag for Watcher thread
	bool m_bRunning;
	CParam m_Param;
	
	// critical section for input file deque
	CRITICAL_SECTION m_csInputFile;
	deque<CString> m_dqInputFiles;
};


