///////////////////////////////////////////////////////////
//  RecogLayer.h
//  Implementation of the Class RecogLayer
//  Created on:      29-¾ÅÔÂ-2015 9:17:08
//  Original author: user
///////////////////////////////////////////////////////////

#if !defined(EA_BE96FE5F_A859_4844_83B6_8F885919299A__INCLUDED_)
#define EA_BE96FE5F_A859_4844_83B6_8F885919299A__INCLUDED_

#include "LayerBase.h"
#include "LinkMap.h"


//////////////////////////////////////////////////////////////////////////
// namespace MidLayer
namespace MidLayer
{


/**
 * client, long connection
 * 
 * isServer: false
 * listenPort: ignored
 * remoteAddr: required
 * remotePort: required
 *
 * recv: sd_connect_remote
 * send: sd_recv
 */
class RecogLayer : public NetLayer::LayerBase
{

public:
	RecogLayer(const wchar_t* pSysID, const wchar_t* prAddr, void* pvThreadPool);
	virtual ~RecogLayer();

	bool QueueMsg(int type, long long seq, const google::protobuf::Message& msg);
protected:
	virtual bool InitLayer();
	virtual int OnConnected(SOCKET sd, NetLayer::LinkInfo& linkInfo);
	virtual void RemoveLink(SOCKET sd);
	virtual void WaitLayerExit();

private:
	NetLayer::SingleLink m_link;
	const std::wstring m_strSydID;

};


} // namespace MidLayer


#endif // !defined(EA_BE96FE5F_A859_4844_83B6_8F885919299A__INCLUDED_)
