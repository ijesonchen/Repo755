#pragma once
#include "..\Public\ThreadPool.h"
#include <string>

class CMsgCount
{
public:
	int idx;

	CMsgCount(int n): idx(n){};
};

class CTaskCount :
	public ThreadTask
{
public:
	CTaskCount(CMsgCount* p);
	virtual void DoTask(void* pvParam, OVERLAPPED* pOverlapped);
	
private:
	CMsgCount* m_pMsg;
};
