#ifndef NetLayerAcceptTask_h__
#define NetLayerAcceptTask_h__

#include "NetLayer.h"

namespace NetLayer
{


class NetLayerAcceptTask :
	public NetLayer::PbShortTask
{
public:
	NetLayerAcceptTask(SOCKET s, void* pvThreadPool);

	virtual void DoTask(void *pvParam, OVERLAPPED *pOverlapped);

protected:
	//////////////////////////////////////////////////////////////////////////
	// followed functions should be overwrite in derived class 
	// how to deal with connection
	virtual bool OnRecvMsg(google::protobuf::int64 /*sn*/, const int /*nType*/, 
							const char* /*pBuffer*/, const int /*nByte*/)
		{return false;};


	//////////////////////////////////////////////////////////////////////////
	// NetLayerAcceptTask functions
	// thread pool to process tasks
	void* const m_pvThreadPool;
	// message proc
	bool ParseMsg(google::protobuf::Message& msg, const char* pBuffer, const int nByte);
};


} // namespace NetLayer

#endif // NetLayerAcceptTask_h__