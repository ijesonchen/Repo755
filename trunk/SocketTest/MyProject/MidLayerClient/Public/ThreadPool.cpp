#pragma once
#include "ThreadPool.h"
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// ThreadPool
//////////////////////////////////////////////////////////////////////////

// template class

//////////////////////////////////////////////////////////////////////////
// ThreadTask
//////////////////////////////////////////////////////////////////////////

// interface class

//////////////////////////////////////////////////////////////////////////
// ThreadWorker
//////////////////////////////////////////////////////////////////////////

static volatile long g_nWorkerIdx = -1;

ThreadWorker::ThreadWorker(void)
: m_cntExec(0)
{
	m_nWorkerIdx = InterlockedIncrement(&g_nWorkerIdx);
}


BOOL ThreadWorker::Initialize(void* pvWorkerParam)
{
	std::cout << "CWorker: " << m_nWorkerIdx << " Initialize, param: " << pvWorkerParam << std::endl;
	return TRUE;
}

void ThreadWorker::Execute(RequestType pvTask, void* pvWorkerParam, OVERLAPPED* pOverlapped)
{
	std::cout << "CWorker: " << m_nWorkerIdx << " Execute " << m_cntExec 
		<< ", param: " << pvTask << " " << pvWorkerParam << " " << pOverlapped << std::endl; 

	ThreadTask* pTask = pvTask;
	if (pTask)
	{
		pTask->DoTask(pvWorkerParam, pOverlapped);
		delete pTask;
		Sleep(10);
	}
	++m_cntExec;
}

void ThreadWorker::Terminate(void* )
{
	std::cout << "CWorker: " << m_nWorkerIdx << " Terminate, exec " 
		<< m_cntExec << " times"<< std::endl;
}