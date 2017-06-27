#ifndef NetLayerShort_h__
#define NetLayerShort_h__

#include "NetLayerBase.h"

namespace NetLayer
{

/************************************************************************/
/*                 basic class for short connection                     */
/*        derive user customized short connection class from this       */
/************************************************************************/

class NetLayerShort :
	public NetLayerBase
{
public:
	NetLayerShort(unsigned short uLocalPort, 
				const wchar_t* pRemoteAddr , unsigned short uRemotePort, 
				const wchar_t* pSysid, void* pvThreadPool);
	~NetLayerShort(void);

protected:
	//////////////////////////////////////////////////////////////////////////
	// followed functions should be overwrite in derived class 
	// how to deal with connection
	virtual void OnConnection(SOCKET /*connSock*/ ) 
		{};
	// how to deal with message to send in bufQueue
	virtual void OnSendMsg(SOCKET /*sd*/, MsgBuffer* const /*pMsgBuf*/ ) 
		{};

protected:
	//////////////////////////////////////////////////////////////////////////
	// NetLayerShort functions
	// start thread
	bool InitLayer(void);

	// local listen port
	unsigned short m_uLocalPort;

	// thread
	static unsigned ListenThread(void* lpLayer);


	static unsigned SendThread(void* lpLayer);


};


} // namespace NetLayer

#endif // NetLayerShort_h__
