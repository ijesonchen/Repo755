#include <stdio.h>
#include <atlbase.h>
#include <atlutil.h>

#include "tasks.h"
#include "worker.h"


#define THREADPOOL_SIZE	5

///////////////////////////////////////////////////////////////////////////////
int main(int /*argc*/, char* /*argv[]*/)
{
	CThreadPool<CMyWorker> pool;
	CTaskArray	tasks;

	HRESULT hr = pool.Initialize((void*)321, THREADPOOL_SIZE);

	if( SUCCEEDED( hr ) ) {
		int i = -1;
		CTaskBase* pTask = NULL;
		if ( CreateTasks(tasks, 100) ) {
			for ( i = 0; i < tasks.GetSize(); i++ ) {
				pTask = tasks[ i ];
				ATLASSERT( NULL != pTask );
				pool.QueueRequest( (CMyWorker::RequestType) pTask );
			}
		}

		// Allow a little time for all the tasks to complete
		Sleep(1000);

		// Clean up the tasks and shutdown the thread pool
		for ( i = 0; i < tasks.GetSize(); i++ ) {
			pTask = tasks[ i ];
			ATLASSERT( NULL != pTask );
			delete pTask;
		}

		// Shutdown the thread pool
		pool.Shutdown();
	}
	else
		printf("Failed to init thread pool!");

	printf("\n");
	return 0;
}