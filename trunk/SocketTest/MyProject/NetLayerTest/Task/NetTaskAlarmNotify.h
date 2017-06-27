#pragma once
#include "../NetLayer/NetLayer.h"

class NetTaskAlarmNotify :
	public PbShortTask
{
public:
	NetTaskAlarmNotify(SOCKET s, MsgBuffer* p);

	~NetTaskAlarmNotify();

	virtual void DoTask(void *pvParam, OVERLAPPED *pOverlapped);
private:
	MsgBuffer* buf;
};
