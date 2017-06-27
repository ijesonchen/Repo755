#ifndef NetLayerMidLong_h__
#define NetLayerMidLong_h__

#include "NetLayer\NetLayerLong.h"

namespace NetLayer
{


class NetLayerMidLong :
	public NetLayerLong
{
public:
	NetLayerMidLong(const wchar_t* pRemoteAddr , unsigned short uRemotePort, 
					const wchar_t* pSysid, void* pvThreadPool, 
					bool bHeartBeat = true, const wchar_t* pLogPrefix = L"MidLongUntitled");
	~NetLayerMidLong(void);

private:
	virtual bool PostConnectProc( SOCKET sd );
	virtual bool OnRecvMsg(google::protobuf::int64 sn, const int nType, const char* pBuffer, const int nByte);
};


} // NetLayer

#endif // NetLayerMidLong_h__
