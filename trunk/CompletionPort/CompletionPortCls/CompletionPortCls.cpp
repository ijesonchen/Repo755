#include <Windows.h>
#include <atlutil.h>
#include <iostream>
using namespace std;

HANDLE m_hRequestQueue = 0;

unsigned Thread(void* p)
{
	DWORD dwBytesTransfered;
	ULONG_PTR dwCompletionKey;

	OVERLAPPED* pOverlapped;
	while (GetQueuedCompletionStatus(m_hRequestQueue, &dwBytesTransfered, &dwCompletionKey, &pOverlapped, INFINITE))
	{
		if (pOverlapped == ATLS_POOL_SHUTDOWN) // Shut down
		{
			cout << "thread ATLS_POOL_SHUTDOWN" << endl;
			break;
		}
		else
		{
			cout << "thread key " << dwCompletionKey << " overlap " << pOverlapped << endl;
		}
	}

	cout << "thread GetQueuedCompletionStatus failed" << endl;
	cout << "thread exit" << endl;
	

	return 0;
}

void main(void)
{

	// Create IO completion port to queue the requests
	HANDLE hCompletion = INVALID_HANDLE_VALUE;
	int nNumThreads = 1;
	m_hRequestQueue = CreateIoCompletionPort(hCompletion, NULL, 0, nNumThreads);

	HANDLE hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Thread, 0, 0, 0);

	if (!m_hRequestQueue)
	{
		return;
	}

	Sleep(10);

	// IO completion packet queued in FIFO.

	PostQueuedCompletionStatus(m_hRequestQueue, 0, 0, (LPOVERLAPPED)1);
	PostQueuedCompletionStatus(m_hRequestQueue, 0, (ULONG_PTR) 11, 0);
	PostQueuedCompletionStatus(m_hRequestQueue, 0, 0, (LPOVERLAPPED)2);
	PostQueuedCompletionStatus(m_hRequestQueue, 0, (ULONG_PTR) 12, 0);
	PostQueuedCompletionStatus(m_hRequestQueue, 0, 0, (LPOVERLAPPED)3);
	PostQueuedCompletionStatus(m_hRequestQueue, 0, (ULONG_PTR) 13, 0);

	Sleep(100);
	cout << "main post end." << endl;

	PostQueuedCompletionStatus(m_hRequestQueue, 0, 0, ATLS_POOL_SHUTDOWN);

	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);

	CloseHandle(m_hRequestQueue);
	m_hRequestQueue = NULL;

	cout << "main exit." << endl;
}