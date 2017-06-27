#ifndef NetLayerLong_h__
#define NetLayerLong_h__

#include "NetLayerBase.h"

namespace NetLayer
{

/************************************************************************/
/*                 basic class for long connection                      */
/*        derive user customized long connection class from this        */
/************************************************************************/

class NetLayerLong :
	public NetLayerBase
{
public:
	NetLayerLong(const wchar_t* pRemoteAddr , unsigned short uRemotePort, 
				const wchar_t* pSysid, void* pvThreadPool, bool bHeartBeat);
	~NetLayerLong(void);

protected:	
	//////////////////////////////////////////////////////////////////////////
	// followed functions should be overwrite in derived class
	// user customized code after connected, e.g. post auth message.
	virtual bool PostConnectProc( SOCKET /*sd*/) 
		{return true;};
	// how to deal with received message.
	virtual bool OnRecvMsg(google::protobuf::int64 /*sn*/, const int /*nType*/, const char* /*pBuffer*/, const int /*nByte*/) 
		{return false;};

protected:
	//////////////////////////////////////////////////////////////////////////
	// NetLayerLong private functions
	// alloc header buffer & start thread
	bool InitLayer(void);

	// socket
	SOCKET m_socket;
	volatile bool m_bSocketReady;
	int Reconnect(void);

	void CleanSocket();

	// if time-out, queue echo msg
	int m_nEchoSendSecs;
	// queue echo message to keep link alive
	bool QueueEchoMsg(void);

	// PB header buffer, used only in relevant thread
	char* m_pBufferRecvHeader;
	char* m_pBufferSendHeader;
	void ClearBuffer(void);

	// thread
	static unsigned RecvThread(void* lpLayer);
	static unsigned SendThread(void* lpLayer);
};



} // namespace NetLayer

#endif // NetLayerLong_h__
