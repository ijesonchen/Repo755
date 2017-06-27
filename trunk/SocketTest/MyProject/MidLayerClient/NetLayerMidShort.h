#ifndef NetLayerMidShort_h__
#define NetLayerMidShort_h__

#include "NetLayer\NetLayerShort.h"

namespace NetLayer
{


class NetLayerMidShort :
	public NetLayerShort
{
public:
	NetLayerMidShort(unsigned short uLocalPort, 
					const wchar_t* pRemoteAddr , unsigned short uRemotePort, 
					const wchar_t* pSysid, void* pvThreadPool,
					const wchar_t* pLogPrefix = L"MidShortUntitled");
	~NetLayerMidShort(void);

private:
	void OnConnection(SOCKET connSock );
	void OnSendMsg(SOCKET sd, MsgBuffer* const pMsgBuf );
};


} // namespace NetLayer

#endif // NetLayerMidShort_h__
