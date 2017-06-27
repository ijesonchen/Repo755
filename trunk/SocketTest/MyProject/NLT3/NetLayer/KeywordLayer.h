#ifndef KeywordLayer_h__
#define KeywordLayer_h__

#include "NetLayer.h"
#include "LayerBase.h"
#include "LinkMap.h"


//////////////////////////////////////////////////////////////////////////
// namespace KwdLayer
namespace KwdLayer
{

/**
 * server, long connection
 * 
 * isServer: true
 * listenPort: required
 * remoteAddr: ignored
 * remotePort: ignored
 *
 * recv: sd_accepted
 * send: sd_recv
 */

class KeywordLayer :
	public NetLayer::LayerBase
{
public:
	KeywordLayer(unsigned short ulPort, void* pvThreadPool);
	virtual ~KeywordLayer(void);

	bool QueueMsg(int type, long long seq, const google::protobuf::Message& msg);
protected:
	NetLayer::LinkMap m_linkMap;

	virtual bool InitLayer();
	virtual int OnConnected(SOCKET sd, NetLayer::LinkInfo& linkInfo);
	virtual void WaitLayerExit();
};



} // namespace KwdLayer


#endif // KeywordLayer_h__
