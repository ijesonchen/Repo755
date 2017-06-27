#ifndef NetLayerRecvTask_h__
#define NetLayerRecvTask_h__

#include "../Public/ThreadTask.h"
#include "NetLayer.h"

class NetLayerRecvTask:
	public ThreadTask
{
public:
	// pv: pNetLayer
	// s: connected socket
	// b: persist connection
	NetLayerRecvTask(void* pvNetLayer, SOCKET s, bool isLongConn);
	~NetLayerRecvTask();

	virtual void DoTask(void *pvParam, OVERLAPPED *pOverlapped);

protected:
	void* pvLayer;
	SOCKET sd;
	bool bLongConn;
};


#endif // NetLayerRecvTask_h__