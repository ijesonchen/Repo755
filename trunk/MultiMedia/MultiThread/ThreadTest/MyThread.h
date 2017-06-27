#pragma once
#include "afxwin.h"

class CMyThread :
	public CWinThread
{
public:
	CMyThread(AFX_THREADPROC pfnThreadProc, LPVOID pParam, int nID);
	~CMyThread(void);
	static unsigned int ThreadFunc(LPVOID pParam);
protected:
	int w_nID;
public:
	unsigned int ProcFunc(void);
};

//////////////////////////////////////////////////////////////////////////
/* 
	USAGE£º

	// create instance
	CMyThread* pThread = new CMyThread(CMyThread::ThreadFunc, NULL, 13);
	pThread->CreateThread(CREATE_SUSPENDED);
	
	// do sth. before thread run

	// run thread
	pThread->ResumeThread();

	// suggest exit notify
	pThread->SetExitFlag(true);

	// wait thread to exit
	WaitForSingleObject(pThread->m_hThread, INFINITE);

	// release resource
	delete pThread;

*/
//////////////////////////////////////////////////////////////////////////
