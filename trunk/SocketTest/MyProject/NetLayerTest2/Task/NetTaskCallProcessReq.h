#pragma once
#include "../NetLayer/NetLayer.h"
#include "../protobuf/PBNetTest.pb.h"

class NetTaskCallProcessReq :
	public NetLayerTask
{
public:
	NetTaskCallProcessReq(const google::protobuf::int64 n,
							PBNetTest::CallProcessRequest& m);

	virtual void DoTask(void *pvParam, OVERLAPPED *pOverlapped);
private:
	PBNetTest::CallProcessRequest msg;
};
