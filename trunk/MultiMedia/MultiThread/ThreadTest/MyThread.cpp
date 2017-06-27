#include "StdAfx.h"
#include "MyThread.h"

CMyThread::CMyThread(AFX_THREADPROC pfnThreadProc, LPVOID pParam, int nID)
: CWinThread(pfnThreadProc, pParam)
, w_nID(nID)
{
	m_bAutoDelete = FALSE;
	m_pThreadParams = this;
}

CMyThread::~CMyThread(void)
{
}

unsigned int CMyThread::ThreadFunc(LPVOID pParam)
{
	CMyThread* pThread = (CMyThread*)pParam;
	return pThread->ProcFunc();
}

unsigned int CMyThread::ProcFunc(void)
{
	printf("Thread: ddddddddd %d\n", w_nID);
	Sleep(2000);
	printf("Thread: Exiting...\n");
	return 0;
}
