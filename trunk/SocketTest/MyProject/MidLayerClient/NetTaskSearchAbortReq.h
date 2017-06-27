#pragma once
#include "NetLayer\NetLayer.h"
#include "PBNetTest.pb.h"

class NetTaskSearchAbortReq :
	public NetLayer::PbShortTask
{
public:
	NetTaskSearchAbortReq(SOCKET s, int t, const google::protobuf::int64 n,
							PBNetTest::SearchAbortRequest& m);

	virtual void DoTask(void *pvParam, OVERLAPPED *pOverlapped);
private:
	int type;
	google::protobuf::int64 seq;
	PBNetTest::SearchAbortRequest msg;
};
