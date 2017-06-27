#ifndef ThreadTask_h__
#define ThreadTask_h__

// task interface for ThreadPool
// all task should derived from this class
struct _OVERLAPPED;
typedef struct _OVERLAPPED OVERLAPPED;
class ThreadTask
{
public:
	virtual ~ThreadTask(){};
	virtual void DoTask(void *pvParam, OVERLAPPED *pOverlapped) = 0;
};


#endif // ThreadTask_h__
