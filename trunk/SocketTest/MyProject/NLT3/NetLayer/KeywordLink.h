#ifndef KeywordLink_h__
#define KeywordLink_h__



#include "NetLink.h"

//////////////////////////////////////////////////////////////////////////
// namespace KwdLayer
namespace KwdLayer
{

class KeywordLink :
	public NetLayer::NetLink
{
public:
	KeywordLink(SOCKET sd, const NetLayer::LinkInfo& info, NetLayer::LayerBase* pLayer);
	~KeywordLink(void);	
	
	//////////////////////////////////////////////////////////////////////////
	// interfaces
protected:
	// recv procedure

	// how to deal with received message.
	// sd: connection to recv or send message
	virtual bool OnMsgRecved(google::protobuf::int64 sn, const int nType, const char* pBuffer, const int nByte) ;

	// send procedure

	// retrieve socket to send data
	virtual SOCKET GetSendSocket();
};


} // namespace KwdLayer


#endif // KeywordLink_h__