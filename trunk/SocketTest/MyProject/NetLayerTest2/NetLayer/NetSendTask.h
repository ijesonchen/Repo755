#ifndef NetSendTask_h__
#define NetSendTask_h__

#include "../Public/ThreadTask.h"
#include "NetLayer.h"

class NetLinkTask;

class NetSendTask :
	public ThreadTask
{
public:
	NetSendTask(NetLinkTask* pvLink, SOCKET s);
	~NetSendTask(void);

	virtual void DoTask(void *pvParam, OVERLAPPED *pOverlapped);
protected:
	NetLinkTask* pLink;
	SOCKET sd;
};
#endif // NetSendTask_h__
