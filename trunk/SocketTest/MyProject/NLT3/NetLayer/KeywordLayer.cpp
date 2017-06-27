#include "KeywordLayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace NetLayer;
//////////////////////////////////////////////////////////////////////////
// namespace KwdLayer
namespace KwdLayer
{


KeywordLayer::KeywordLayer(unsigned short ulPort, void* pvThreadPool)
: LayerBase(pvThreadPool)
{
	m_bServer = true;
	m_uListenPort = ulPort;
	m_msg.SetPrefix(L"KeywordLayer");
}

KeywordLayer::~KeywordLayer(void)
{
	ExitNotify();
}

bool KeywordLayer::QueueMsg(int type, long long seq, const google::protobuf::Message& msg)
{
	return m_linkMap.QueueMsg(type, seq, msg);
}

bool KeywordLayer::InitLayer()
{
	if (!m_bServer || !m_uListenPort)
	{
		return false;
	}
	return true;
}

int KeywordLayer::OnConnected(SOCKET sd, NetLayer::LinkInfo& linkInfo)
{
// 	KeywordLink* pLink = new KeywordLink(sd, linkInfo, this);
// 	std::auto_ptr<KeywordLink> ap(pLink);
// 	if (!pLink->Init())
// 	{
// 		wstringstream ss;
// 		ss << L"OnConnected: KeywordLink Init failed " << linkInfo.m_remoteAddr << L":" << linkInfo.m_remotePort
// 			<< L" in " << linkInfo.m_localAddr << L":" << linkInfo.m_localPort;
// 		m_msg = ss;
// 		return -1;
// 	}
// 
// 	m_linkMap.Capture(sd, pLink);
// 	ap.release();

	return 0;
}

void KeywordLayer::WaitLayerExit()
{
	m_linkMap.ClearLink();
}


} // namespace KwdLayer

