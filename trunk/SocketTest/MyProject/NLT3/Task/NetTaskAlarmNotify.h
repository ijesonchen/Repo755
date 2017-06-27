#pragma once
#include "../NetLayer/NetLayer.h"

class NetTaskAlarmNotify :
	public NetLayer::NetSockTask
{
public:
	NetTaskAlarmNotify(SOCKET s, NetLayer::MsgBuffer* p);

	~NetTaskAlarmNotify();

	virtual void DoTask(void *pvParam, OVERLAPPED *pOverlapped);
private:
	NetLayer::MsgBuffer* buf;
};
