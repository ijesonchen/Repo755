// ThreadTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "ThreadTest.h"
#include "MyThread.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;

DWORD WINAPI ThreadFunc(LPVOID lParam)
{
	printf("ThreadFunc: start.\n");
	Sleep(2000);
	printf("ThreadFunc: exit\n");
	return 0;
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
	}

	//////////////////////////////////////////////////////////////////////////
	/*

	//////////////////////////////////////////////////////////////////////////
	1. ���C++�����MFC�࣬�̳�CWinThread���ŵ㣺ֱ��delete���Զ�CloseHandle ȱ�㣺�����޷������̺߳�����
		CWinThread::m_pThreadParams �Ǵ��ݸ��̺߳���ThreadFunc�Ĳ�������
	2. ʹ��CreateThread�� �ŵ㣺���Կ��Կ����̺߳��������򵥡� ȱ�㣺��ҪCloseHandle.
	3. CWinThread, AfxBeginThread, _beginthread���Ƕ�CreateThread�ķ�װ
	
	//////////////////////////////////////////////////////////////////////////
	CWinThread��أ�

	�������̣߳�һ�������̣߳�����CWinThread::CreateThread�������߳�
		��CWinThread::CreateThread�����ڵ������߳���
	_AFX_THREAD_STARTUP�������½��߳�

	�ṹ��
	a. _AFX_THREAD_STARTUP
		pThread: CWinThread����
		hEvent1: _AFX_THREAD_STARTUP֪ͨCWinThread::CreateThread
		hEvent2: CWinThread::CreateThread֪ͨ_AFX_THREAD_STARTUP

	b. _ptiddata
		_initaddr: _AfxThreadEntry��ڵ�ַ
		_initarg: _AFX_THREAD_STARTUP������ַ

	a. CWinThread�̺߳���������m_pfnThreadProc��
	b. CWinThread::CreateThread����ͬ���¼�������_beginthread��
		�������_AfxThreadEntry������_AFX_THREAD_STARTUP����CWinThreadʵ��
	c. _beginthread����CreateThread API
		�߳����_threadstartex������_ptiddata�������_AFX_THREAD_STARTUP
		��ˣ�����ʱ�߳�����ʾΪ_threadstartex
	d. _AfxThreadEntry���в���_AFX_THREAD_STARTUP
		�ȵ�ͬ���¼��󣬵���pThread->m_pfnThreadProc

	*/
	CMyThread* pThread = new CMyThread(CMyThread::ThreadFunc, (LPVOID)99, 13);
	pThread->CreateThread(CREATE_SUSPENDED);
	printf("Thread created.\n");

	Sleep(2000);

	printf("Thread resumed.\n");
	pThread->ResumeThread();


	printf("Waiting for exit.\n");
	WaitForSingleObject(pThread->m_hThread, INFINITE);
	printf("Thread exited\n");
	delete pThread;

	HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CMyThread::ThreadFunc, pThread, NULL, NULL);

	Sleep(500);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);

	CWinThread* pThread2 = AfxBeginThread((AFX_THREADPROC)ThreadFunc, NULL);
	pThread2->m_bAutoDelete = false;
	pThread2->ResumeThread();

	Sleep(500);
	WaitForSingleObject(pThread2->m_hThread, INFINITE);
	delete pThread2;

	printf("press any key to exit.\n");
	getchar();

	return nRetCode;
}
