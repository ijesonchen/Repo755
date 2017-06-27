#include <afx.h>
#include "..\Public\ThreadPool.h"
#include <iostream>
#include <deque>

#include "TaskCount.h"

#define new DEBUG_NEW

using namespace std;

///////////////////////////////////////////////////////////////////////////////
int main(int /*argc*/, char* /*argv[]*/)
{
	const int THREAD_POOL_COUNT = 20;
	const int MSG_COUNT = 100;

	// create tasks
	deque<CMsgCount> dqMsg;
	for (int ii = 0; ii < MSG_COUNT; ++ii)
	{
		dqMsg.push_back(CMsgCount(ii));
	}
	
	ThreadPool<ThreadWorker> pool;
	pool.Initialize((void*)9999, THREAD_POOL_COUNT);

	// queue task
	for (int ii = 0; ii < MSG_COUNT; ++ii)
	{
		CTaskCount* pTask = new CTaskCount(&dqMsg[ii]);
		pool.QueueRequest(pTask);
	}

// 	while (true)
// 	{	// a trap
		Sleep(100);
//	}
// 



	printf("*********************************************************\n");
	Sleep(100);
 

	pool.Shutdown(100);
	printf("exit*********************************************************");

	return 0;
}