///////////////////////////////////////////////////////////
//  RecogLayer.cpp
//  Implementation of the Class RecogLayer
//  Created on:      29-¾ÅÔÂ-2015 9:17:09
//  Original author: user
///////////////////////////////////////////////////////////

#include "RecogLayer.h"
#include <string>
#include <sstream>
#include "RecogLink.h"

using namespace std;
using namespace NetLayer;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
// namespace MidLayer
namespace MidLayer
{



RecogLayer::RecogLayer(const wchar_t* pSysID, const wchar_t* prAddr, void* pvThreadPool)
: LayerBase(pvThreadPool)
, m_strSydID(pSysID)
{
	m_bServer = false;
	if (prAddr)
	{
		m_strRemoteAddr = prAddr;
	}
	m_uRemotePort = NETLAYER_PORT_MID_RECOG;
	m_msg.SetPrefix(L"RecogLayer");
}


RecogLayer::~RecogLayer(){

	ExitNotify();
	m_link.ClearLink();
}


bool RecogLayer::QueueMsg(int type, long long seq, const google::protobuf::Message& msg)
{
	return m_link.QueueMsg(type, seq, msg);
}

/**
 * new NetLink, add to map
 */
int RecogLayer::OnConnected(SOCKET sd, NetLayer::LinkInfo& linkInfo){

	RecogLink* pLink = new RecogLink(sd, linkInfo, this, m_strSydID.c_str());
	std::auto_ptr<RecogLink> ap(pLink);

	if (!pLink->Init())
	{
		wstringstream ss;
		ss << L"OnConnected: RecogLink Init failed " << linkInfo.m_remoteAddr << L":" << linkInfo.m_remotePort
			<< L" in " << linkInfo.m_localAddr << L":" << linkInfo.m_localPort;
		m_msg = ss;
		return -1;
	}

	if (!pLink->QueueConnectReq())
	{
		m_msg = L"OnConnected: QueueConnectReq connect request failed.";
		ClearSocket(sd);
		return -2;
	}

	m_link.Reset(pLink);
	ap.release();

	m_bClientConnected = true;

	return 0;
}


bool RecogLayer::InitLayer(){

	// check parameter
	if (m_bServer || m_strRemoteAddr.empty() || !m_uRemotePort ||
		m_strSydID.empty())
	{
		m_msg = L"InitLayer: check parameter failed.";
		return false;
	}
	return true;
}

void RecogLayer::RemoveLink(SOCKET sd)
{
	m_link.ClearLink();
	if (m_bRunning)
	{
		m_bClientConnected = false;
	}
}


/**
 * P2P/SRV: listen & accept Client: connect
 */
void RecogLayer::WaitLayerExit(){

	m_link.ClearLink();
}


} // namespace MidLayer

