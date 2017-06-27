#ifndef NetRecvTask_h__
#define NetRecvTask_h__

#include "../Public/ThreadTask.h"
#include "NetLayer.h"

class NetBase;

class NetLinkTask:
	public ThreadTask
{
public:
	// pv: pNetLayer
	// s: connected socket
	// b: persist connection
	NetLinkTask(NetBase* pBase, const LinkInfo link, SOCKET s);
	~NetLinkTask();

	bool Init(void);

	virtual void DoTask(void *pvParam, OVERLAPPED *pOverlapped);


	//////////////////////////////////////////////////////////////////////////
	bool QueueMsg(int type, long long seq, const google::protobuf::Message& msg){};
protected:
	friend class NetSendTask;

	virtual void RecvTask(void *pvParam, OVERLAPPED *pOverlapped);

	NetBase* pLayer;
	LinkInfo linkInfo;
	SOCKET m_sdRecv;

	bool bLongConn;

	// send thread
	HANDLE m_hSendThread;
	static unsigned SendThread(void* lpLinkTask);
};
#endif // NetRecvTask_h__
