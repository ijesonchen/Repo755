#include "KeywordLink.h"
#include "..\Public\ThreadPool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace NetLayer;

//////////////////////////////////////////////////////////////////////////
// namespace KwdLayer
namespace KwdLayer
{



KeywordLink::KeywordLink(SOCKET sd, const NetLayer::LinkInfo& info, NetLayer::LayerBase* pLayer)
: NetLink(sd, info, pLayer)
{
}

KeywordLink::~KeywordLink(void)
{
	ExitWait();
}

// how to deal with received message.
// sd: connection to recv or send message
bool KeywordLink::OnMsgRecved(google::protobuf::int64 sn, const int nType, const char* pBuffer, const int nByte) 
{
	ThreadPoolPtr pPool = (ThreadPoolPtr) m_pLayer->ThreadPool();

// 	switch ()
// 	{
// 	case :
// 		break;
// 	}

	return false;
}

// retrieve socket to send data
SOCKET KeywordLink::GetSendSocket()
{
	return m_sdRecv;
}



} // namespace KwdLayer

