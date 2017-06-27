// ThreadTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ThreadTest.h"
#include "MyThread.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

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

	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: MFC 初始化失败\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 在此处为应用程序的行为编写代码。
	}

	//////////////////////////////////////////////////////////////////////////
	/*

	//////////////////////////////////////////////////////////////////////////
	1. 添加C++类而非MFC类，继承CWinThread。优点：直接delete，自动CloseHandle 缺点：调试无法看到线程函数名
		CWinThread::m_pThreadParams 是传递给线程函数ThreadFunc的参数。。
	2. 使用CreateThread。 优点：调试可以看到线程函数名，简单。 缺点：需要CloseHandle.
	3. CWinThread, AfxBeginThread, _beginthread都是对CreateThread的封装
	
	//////////////////////////////////////////////////////////////////////////
	CWinThread相关：

	调用者线程（一般是主线程）调用CWinThread::CreateThread创建新线程
		即CWinThread::CreateThread运行在调用者线程上
	_AFX_THREAD_STARTUP运行在新建线程

	结构：
	a. _AFX_THREAD_STARTUP
		pThread: CWinThread对象
		hEvent1: _AFX_THREAD_STARTUP通知CWinThread::CreateThread
		hEvent2: CWinThread::CreateThread通知_AFX_THREAD_STARTUP

	b. _ptiddata
		_initaddr: _AfxThreadEntry入口地址
		_initarg: _AFX_THREAD_STARTUP参数地址

	a. CWinThread线程函数保存在m_pfnThreadProc中
	b. CWinThread::CreateThread创建同步事件并调用_beginthread，
		函数入口_AfxThreadEntry，参数_AFX_THREAD_STARTUP包含CWinThread实例
	c. _beginthread调用CreateThread API
		线程入口_threadstartex，参数_ptiddata包含入口_AFX_THREAD_STARTUP
		因此，调试时线程名显示为_threadstartex
	d. _AfxThreadEntry带有参数_AFX_THREAD_STARTUP
		等到同步事件后，调用pThread->m_pfnThreadProc

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
