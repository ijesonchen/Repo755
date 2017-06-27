#ifndef threadpool_h__
#define threadpool_h__

//////////////////////////////////////////////////////////////////////////
// src extract from MS VS2008 \VC\atlmfc\include\atlutil.h

#include <atlbase.h>
#include <atlcoll.h>

#ifndef ATLS_DEFAULT_THREADSPERPROC
#define ATLS_DEFAULT_THREADSPERPROC 2
#endif

#ifndef ATLS_DEFAULT_THREADPOOLSHUTDOWNTIMEOUT
#define ATLS_DEFAULT_THREADPOOLSHUTDOWNTIMEOUT 36000
#endif

//
// ThreadPool
// This class is a simple IO completion port based thread pool
//	Worker:
//		is a class that is responsible for handling requests
//		queued on the thread pool.
//		It must have a typedef for RequestType, where request type
//		is the datatype to be queued on the pool
//		RequestType must be castable to (DWORD)
//		The value -1 is reserved for shutdown
//		of the pool
//		Worker must also have a void Execute(RequestType request, void *pvParam, OVERLAPPED *pOverlapped) function
//	ThreadTraits:
//		is a class that implements a static CreateThread function
//		This allows for overriding how the threads are created
#define ATLS_POOL_SHUTDOWN ((OVERLAPPED*) ((__int64) -1))
template <class Worker, class ThreadTraits=DefaultThreadTraits, class WaitTraits=DefaultWaitTraits>
class ThreadPool
{
protected:

	CSimpleMap<DWORD, HANDLE> m_threadMap;

	CComCriticalSection m_critThreadId;
	CAtlArray<DWORD> m_aThreads;

	CComCriticalSection m_critSec;
	DWORD m_dwStackSize;
	DWORD m_dwMaxWait;

	void *m_pvWorkerParam;
	LONG m_bShutdown;

	HANDLE m_hThreadEvent;
	HANDLE m_hRequestQueue;

	bool m_bRunning;

public:

	ThreadPool() throw() :
	  m_hRequestQueue(NULL),
		  m_pvWorkerParam(NULL),
		  m_dwMaxWait(ATLS_DEFAULT_THREADPOOLSHUTDOWNTIMEOUT),
		  m_bShutdown(FALSE),
		  m_dwStackSize(0),
		  m_bRunning(false)
	  {
	  }

	  virtual ~ThreadPool() throw()
	  {
		  Shutdown();
	  }

	  // Initialize the thread pool
	  // if nNumThreads > 0, then it specifies the number of threads
	  // if nNumThreads < 0, then it specifies the number of threads per proc (-)
	  // if nNumThreads == 0, then it defaults to two threads per proc
	  // hCompletion is a handle of a file to associate with the completion port
	  // pvWorkerParam is a parameter that will be passed to Worker::Execute
	  //	dwStackSize:
	  //		The stack size to use when creating the threads
	  _Check_return_ HRESULT Initialize(_In_opt_ void *pvWorkerParam=NULL, _In_ int nNumThreads=0, _In_ DWORD dwStackSize=0, _In_ HANDLE hCompletion=INVALID_HANDLE_VALUE) throw()
	  {
		  ATLASSUME( m_hRequestQueue == NULL );

		  if (m_hRequestQueue)   // Already initialized
			  return AtlHresultFromWin32(ERROR_ALREADY_INITIALIZED);

		  if (S_OK != m_critSec.Init())
			  return E_FAIL;

		  if (S_OK != m_critThreadId.Init())
			  return E_FAIL;

		  m_hThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		  if (!m_hThreadEvent)
		  {
			  m_critSec.Term();
			  return AtlHresultFromLastError();
		  }

		  // Create IO completion port to queue the requests
		  m_hRequestQueue = CreateIoCompletionPort(hCompletion, NULL, 0, nNumThreads);
		  if (m_hRequestQueue == NULL)
		  {
			  // failed creating the Io completion port
			  m_critSec.Term();
			  CloseHandle(m_hThreadEvent);
			  return AtlHresultFromLastError();		
		  }
		  m_pvWorkerParam = pvWorkerParam;
		  m_dwStackSize = dwStackSize;

		  HRESULT hr = SetSize(nNumThreads);
		  if (hr != S_OK)
		  {
			  // Close the request queue handle
			  CloseHandle(m_hRequestQueue);

			  // Clear the queue handle
			  m_hRequestQueue = NULL;

			  // Uninitialize the critical sections
			  m_critSec.Term();
			  CloseHandle(m_hThreadEvent);


			  return hr;
		  }

		  m_bRunning = true;

		  return S_OK;
	  }

	  // Shutdown the thread pool
	  // This function posts the shutdown request to all the threads in the pool
	  // It will wait for the threads to shutdown a maximum of dwMaxWait MS.
	  // If the timeout expires it just returns without terminating the threads.
	  void Shutdown(_In_ DWORD dwMaxWait=0) throw()
	  {
		  if (!m_hRequestQueue)   // Not initialized
			  return;

		  CComCritSecLock<CComCriticalSection> lock(m_critSec, false);
		  if (FAILED(lock.Lock()))
		  {
			  // out of memory
			  ATLASSERT( FALSE );
			  return;
		  }


		  if (dwMaxWait == 0)
			  dwMaxWait = m_dwMaxWait;

		  m_bRunning = false;

		  HRESULT hr = InternalResizePool(0, dwMaxWait);

		  if (hr != S_OK)
			  ATLTRACE(atlTraceUtil, 0, _T("Thread pool not shutting down cleanly : %08x"), hr);
		  // If the threads have not returned, then something is wrong


		  CComCritSecLock<CComCriticalSection> lockt(m_critThreadId, true);
		  for (size_t ii = 0, ni = m_aThreads.GetCount();
			  ii < ni; ++ii)
		  {
			  DWORD dwThreadId = m_aThreads[ii];

			  int nIndex = m_threadMap.FindKey(dwThreadId);
			  if (nIndex != -1)
			  {
				  HANDLE hThread = m_threadMap.GetValueAt(nIndex);
				  // Wait for the thread to shutdown
				  if (WaitTraits::WaitForSingleObject(hThread, 60000) != WAIT_OBJECT_0) 
				  {
#pragma warning(push)
#pragma warning(disable: 6258)
					  /* deliberate design choice to use TerminateThread here in extremis */
					  ATLTRACE(atlTraceUtil, 0, _T("Terminating thread"));
					  TerminateThread(hThread, 0);
#pragma warning(pop)
				  }
				  CloseHandle(hThread);
				  m_threadMap.RemoveAt(nIndex);
			  }	
		  }
		  lockt.Unlock();

		  for (int i = m_threadMap.GetSize() - 1; i >= 0; i--)
		  {
			  HANDLE hThread = m_threadMap.GetValueAt(i);
			  DWORD dwExitCode;
			  GetExitCodeThread(hThread, &dwExitCode);
			  if (dwExitCode == STILL_ACTIVE)
			  {
#pragma warning(push)
#pragma warning(disable: 6258)
				  /* deliberate design choice to use TerminateThread here in extremis */
				  ATLTRACE(atlTraceUtil, 0, _T("Terminating thread"));
				  TerminateThread(hThread, 0);
#pragma warning(pop)
			  }
			  CloseHandle(hThread);
		  }

		  // Close the request queue handle
		  CloseHandle(m_hRequestQueue);

		  // Clear the queue handle
		  m_hRequestQueue = NULL;

		  ATLASSUME(m_threadMap.GetSize() == 0);

		  // Uninitialize the critical sections
		  lock.Unlock();
		  m_critSec.Term();
		  CloseHandle(m_hThreadEvent);

	  }

	  // IThreadPoolConfig methods
	  _Check_return_ HRESULT STDMETHODCALLTYPE SetSize(_In_ int nNumThreads) throw()
	  {
		  if (nNumThreads == 0)
			  nNumThreads = -ATLS_DEFAULT_THREADSPERPROC;

		  if (nNumThreads < 0)
		  {
			  SYSTEM_INFO si;
			  GetSystemInfo(&si);
			  nNumThreads = (int) (-nNumThreads) * si.dwNumberOfProcessors;
		  }

		  return InternalResizePool(nNumThreads, m_dwMaxWait);
	  }

	  _Check_return_ HRESULT STDMETHODCALLTYPE GetSize(_Out_ int *pnNumThreads) throw()
	  {
		  if (!pnNumThreads)
			  return E_POINTER;

		  *pnNumThreads = GetNumThreads();
		  return S_OK;
	  }

	  _Check_return_ HRESULT STDMETHODCALLTYPE SetTimeout(_In_ DWORD dwMaxWait) throw()
	  {
		  m_dwMaxWait = dwMaxWait;

		  return S_OK;
	  }

	  _Check_return_ HRESULT STDMETHODCALLTYPE GetTimeout(_Out_ DWORD *pdwMaxWait) throw()
	  {
		  if (!pdwMaxWait)
			  return E_POINTER;

		  *pdwMaxWait = m_dwMaxWait;

		  return S_OK;
	  }

	  // IUnknown methods
	  _Check_return_ HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ void **ppv) throw()
	  {
		  if (!ppv)
			  return E_POINTER;

		  *ppv = NULL;

		  if (InlineIsEqualGUID(riid, __uuidof(IUnknown)) ||
			  InlineIsEqualGUID(riid, __uuidof(IThreadPoolConfig)))
		  {
			  *ppv = static_cast<IThreadPoolConfig*>(this);
			  AddRef();
			  return S_OK;
		  }
		  return E_NOINTERFACE;
	  }

	  ULONG STDMETHODCALLTYPE AddRef() throw()
	  {
		  return 1;
	  }

	  ULONG STDMETHODCALLTYPE Release() throw()
	  {
		  return 1;
	  }


	  HANDLE GetQueueHandle() throw()
	  {
		  return m_hRequestQueue;
	  }

	  int GetNumThreads() throw()
	  {
		  return m_threadMap.GetSize();
	  }

	  // QueueRequest adds a request to the thread pool
	  // it will be picked up by one of the threads and dispatched to the worker
	  // in WorkerThreadProc
	  BOOL QueueRequest(_In_ typename Worker::RequestType request) throw()
	  {
		  ATLASSUME( m_hRequestQueue != NULL );

		  if (!PostQueuedCompletionStatus(m_hRequestQueue, 0, (ULONG_PTR) request, NULL))
			  return FALSE;
		  return TRUE;
	  }


protected:

	DWORD ThreadProc() throw()
	{
		DWORD dwBytesTransfered;
		ULONG_PTR dwCompletionKey;

		OVERLAPPED* pOverlapped;

		// this block is to ensure theWorker gets destructed before the 
		// thread handle is closed
		{
			// We instantiate an instance of the worker class on the stack
			// for the life time of the thread.
			Worker theWorker;

			if (theWorker.Initialize(m_pvWorkerParam) == FALSE)
			{
				return 1;
			}

			SetEvent(m_hThreadEvent);
			// Get the request from the IO completion port
			while (GetQueuedCompletionStatus(m_hRequestQueue, &dwBytesTransfered, &dwCompletionKey, &pOverlapped, INFINITE))
			{
				if (!m_bRunning)
				{
					break;
				}
				if (pOverlapped == ATLS_POOL_SHUTDOWN) // Shut down
				{
					LONG bResult = InterlockedExchange(&m_bShutdown, FALSE);
					if (bResult) // Shutdown has not been cancelled
						break;

					// else, shutdown has been cancelled -- continue as before
				}
				else										// Do work
				{
					Worker::RequestType request = (Worker::RequestType) dwCompletionKey;

					// Process the request.  Notice the following:
					// (1) It is the worker's responsibility to free any memory associated
					// with the request if the request is complete
					// (2) If the request still requires some more processing
					// the worker should queue the request again for dispatching
					theWorker.Execute(request, m_pvWorkerParam, pOverlapped);
				}
			}

			theWorker.Terminate(m_pvWorkerParam);
		}

		CComCritSecLock<CComCriticalSection> lockt(m_critThreadId, true);
		m_aThreads.Add(GetCurrentThreadId());
		lockt.Unlock();

		SetEvent(m_hThreadEvent);

		return 0; 
	}

	static DWORD WINAPI WorkerThreadProc(_In_ LPVOID pv) throw()
	{
		ThreadPool* pThis = 
			reinterpret_cast< ThreadPool* >(pv); 

		return pThis->ThreadProc();
	} 

	_Check_return_ HRESULT InternalResizePool(_In_ int nNumThreads, _In_ int dwMaxWait) throw()
	{
		if (!m_hRequestQueue)   // Not initialized
			return E_FAIL;

		CComCritSecLock<CComCriticalSection> lock(m_critSec, false);
		if (FAILED(lock.Lock()))
		{
			// out of memory
			ATLASSERT( FALSE );
			return E_FAIL;
		}

		int nCurThreads = m_threadMap.GetSize();
		if (nNumThreads == nCurThreads)
		{
			return S_OK;
		}
		else if (nNumThreads < nCurThreads)
		{
			int nNumShutdownThreads = nCurThreads - nNumThreads;
			for (int nThreadIndex = 0; nThreadIndex < nNumShutdownThreads; nThreadIndex++)
			{
				ResetEvent(m_hThreadEvent);

				InterlockedExchange(&m_bShutdown, TRUE);
				PostQueuedCompletionStatus(m_hRequestQueue, 0, 0, ATLS_POOL_SHUTDOWN);
				DWORD dwRet = WaitTraits::WaitForSingleObject(m_hThreadEvent, dwMaxWait);

				if (dwRet == WAIT_TIMEOUT)
				{
					LONG bResult = InterlockedExchange(&m_bShutdown, FALSE);
					if (bResult) // Nobody picked up the shutdown message
					{
						return AtlHresultFromWin32(WAIT_TIMEOUT);
					}
				}
				else if (dwRet != WAIT_OBJECT_0)
				{
					return AtlHresultFromLastError();
				}

				if (!m_aThreads.GetCount())
				{
					continue;
				}

				CComCritSecLock<CComCriticalSection> lockt(m_critThreadId, false);
				if (FAILED(lockt.Lock()))
				{
					// out of memory
					ATLASSERT( FALSE );
					return E_FAIL;
				}
				for (size_t ii = 0, ni = m_aThreads.GetCount();
					ii < ni; ++ii)
				{
					DWORD dwThreadId = m_aThreads[ii];

					int nIndex = m_threadMap.FindKey(dwThreadId);
					if (nIndex != -1)
					{
						HANDLE hThread = m_threadMap.GetValueAt(nIndex);
						// Wait for the thread to shutdown
						if (WaitTraits::WaitForSingleObject(hThread, 60000) == WAIT_OBJECT_0) 
						{
							CloseHandle(hThread);
							m_threadMap.RemoveAt(nIndex);
						}
						else
						{
							// Thread failed to terminate
							return E_FAIL;
						}
					}	
				}
			}
		}
		else
		{
			int nNumNewThreads = nNumThreads - nCurThreads;
			// Create and initialize worker threads

			for (int nThreadIndex = 0; nThreadIndex < nNumNewThreads; nThreadIndex++)
			{
				DWORD dwThreadID;
				ResetEvent(m_hThreadEvent);
				CHandle hdlThread( ThreadTraits::CreateThread(NULL, m_dwStackSize, WorkerThreadProc, (LPVOID)this, 0, &dwThreadID) );

				if (!hdlThread)
				{
					HRESULT hr = AtlHresultFromLastError();
					ATLASSERT(hr != S_OK);
					return hr;
				}

				DWORD dwRet = WaitTraits::WaitForSingleObject(m_hThreadEvent, dwMaxWait);
				if (dwRet != WAIT_OBJECT_0)
				{
					if (dwRet == WAIT_TIMEOUT)
					{
						return HRESULT_FROM_WIN32(WAIT_TIMEOUT);
					}
					else
					{
						return AtlHresultFromLastError();
					}
				}

				if (m_threadMap.Add(dwThreadID, hdlThread) != FALSE)
				{
					hdlThread.Detach();
				}
			}
		}
		return S_OK;
	}

}; // class ThreadPool

//////////////////////////////////////////////////////////////////////////
// ThreadTask
//////////////////////////////////////////////////////////////////////////

#include "ThreadTask.h"

//////////////////////////////////////////////////////////////////////////
// ThreadWorker
//////////////////////////////////////////////////////////////////////////

class ThreadWorker
{
public:
	// ThreadTask* (RequestType) pvTask
	typedef ThreadTask* RequestType;
	ThreadWorker(void);

	BOOL Initialize(void* pvWorkerParam);
	void Execute(RequestType pvTask, void* pvWorkerParam, OVERLAPPED* pOverlapped);
	void Terminate(void* pvWorkerParam);

private:
	long m_nWorkerIdx;
	int m_cntExec;
};

typedef ThreadPool<ThreadWorker>* ThreadPoolPtr;


#endif // threadpool_h__