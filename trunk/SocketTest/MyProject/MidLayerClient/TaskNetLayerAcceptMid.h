#pragma once
#include "NetLayer\NetLayerAcceptTask.h"

class TaskNetLayerAcceptMid :
	public NetLayer::NetLayerAcceptTask
{
public:
	TaskNetLayerAcceptMid(SOCKET s, void* pvThreadPool);
	~TaskNetLayerAcceptMid(void);
private:
	virtual bool OnRecvMsg(google::protobuf::int64 sn, const int nType, const char* pBuffer, const int nByte);
};
