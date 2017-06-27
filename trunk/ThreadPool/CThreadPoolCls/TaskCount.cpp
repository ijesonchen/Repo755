#include "TaskCount.h"
#include <sstream>
#include <iostream>
using namespace std;

CTaskCount::CTaskCount(CMsgCount* p)
: m_pMsg(p)
{
}

void CTaskCount::DoTask(void* pvParam, OVERLAPPED* pOverlapped)
{
	wstringstream ss;
	ss << "task msg: idx " << m_pMsg->idx;
	wcout << ss.str() << endl;
}
