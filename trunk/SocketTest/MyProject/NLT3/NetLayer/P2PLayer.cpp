///////////////////////////////////////////////////////////
//  P2PLayer.cpp
//  Implementation of the Class P2PLayer
//  Created on:      29-¾ÅÔÂ-2015 16:01:24
//  Original author: user
///////////////////////////////////////////////////////////

#include "P2PLayer.h"
#include <sstream>
#include "..\Public\ThreadPool.h"
#include "..\protobuf\PBNetTest.pb.h"
#include "..\Task\NetTaskSearchAbortReq.h"
#include "..\Task\NetTaskAlarmNotify.h"

using namespace std;
using namespace PBNetTest;
using namespace NetLayer;



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
// namespace MidLayer
namespace MidLayer
{


P2PLayer::P2PLayer(const wchar_t* pSysID, const wchar_t* prAddr, unsigned short uPort, 
				   void* pvThreadPool, const wchar_t* pMsgPrefix /*= L"P2PLayerUnamed"*/)
: LayerBase(pvThreadPool)
, m_strSydID(pSysID)
{
	m_bServer = true;
	m_uListenPort = uPort;
	if (prAddr)
	{
		m_strRemoteAddr = prAddr;
	}
	m_uRemotePort = uPort;
	m_msg.SetPrefix(pMsgPrefix);
}


P2PLayer::~P2PLayer(){
	ExitWait();

	if (m_pSendBufHdr)
	{
		delete m_pSendBufHdr;
		m_pSendBufHdr = NULL;
	}
}


int P2PLayer::OnConnected(SOCKET sd, NetLayer::LinkInfo& linkInfo){

	MsgBuffer* pMsgBuffer = NULL;
	int nRet = RecvAndAllocPbBuf(sd, pMsgBuffer);
	if (nRet)
	{
		wstringstream ss;
		ss << L"RecvAndAllocPbBuf failed: " <<  nRet << L" from " << linkInfo.m_remoteAddr << L":" << linkInfo.m_remotePort
			<< L" in port " << linkInfo.m_localPort;
		m_msg = ss;
		return 1;
	}

	std::auto_ptr<MsgBuffer> ap(pMsgBuffer);

	if (!OnShortMsgRecved(sd, pMsgBuffer->seq, pMsgBuffer->type, pMsgBuffer->buf, pMsgBuffer->len))
	{
		wstringstream ss;
		ss << L"proc msg failed: t:" <<  pMsgBuffer->type << L" len:" << pMsgBuffer->len << L" from " << linkInfo.m_remoteAddr << ":" << linkInfo.m_remotePort
			<< L" in port " << linkInfo.m_localPort;;
		m_msg = ss;
		return 2;
	}

	// start a task
	return 0;
}

bool P2PLayer::OnShortMsgRecved(SOCKET sd, google::protobuf::int64 sn, const int nType, const char* pBuffer, const int nByte) 
{
	if (NETLAYER_SHOW_DETAIL_MSG)
	{
		wstringstream ss;
		ss << L"Msg recved: " << GetMsgTypeString(nType) << L" " << nByte << L" bytes.";
		m_msg.info(ss);
	}

	ThreadPoolPtr pool = (ThreadPoolPtr) m_pvThreadPool;
	switch (nType)
	{
	case MSG_SEARCH_ABORT_REQ    :
		{
			SearchAbortRequest msg;
			if (ParseMsg(msg, pBuffer, nByte))
			{
				pool->QueueRequest(new NetTaskSearchAbortReq(sd, nType, sn, msg));
				sd = INVALID_SOCKET;
				return true;
			}
		}
		break;

	default:
		m_msg = L"OnRecvMsg: bad msg type.";
		return false;
		break;
	}

	return false;
}
unsigned int P2PLayer::SendThread(P2PLayer* pvNetP2P){

	P2PLayer* pLayer = pvNetP2P;
	pLayer->Pending();

	const int nHdr = NETLAYER_PB_HEADER_LENGTH;
	BufQueue& bufQueue = pLayer->m_bufQueue;
	while (pLayer->Running())
	{
		// check has data first, then connect, send
		if (!bufQueue.HasData())
		{
			pLayer->Msg().info(L"SendThread: no data.");
			pLayer->Doze(NETLAYER_SLEEP_SEND_NO_DATA);
			continue;
		}
		SOCKET sd = pLayer->Connect(pLayer->m_strRemoteAddr, pLayer->m_uRemotePort);
		if (sd == INVALID_SOCKET)
		{
			pLayer->Doze(NETLAYER_SLEEP_TRYCONNECT);
			continue;
		}
		pLayer->SendMsg(sd);
	}

	pLayer->Msg().info(L"Send thread exit.");

	CloseHandle(pLayer->m_hSendThread);
	pLayer->m_hSendThread = NULL;

	return 0;
}


bool P2PLayer::InitLayer(){
	
	// check parameter
	if (!m_uListenPort || m_strRemoteAddr.empty() || !m_uRemotePort ||
		m_strSydID.empty())
	{
		return false;
	}

	m_pSendBufHdr = new char[NETLAYER_PB_HEADER_LENGTH];

	m_hSendThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SendThread, this, 0, NULL);
	if (!m_hSendThread)
	{
		m_msg = L"InitLayer create SendThread failed.";
		return false;
	}

	return true;
}

void P2PLayer::WaitLayerExit()
{
	WaitForSingleObject(m_hSendThread, INFINITE);
}


void P2PLayer::SendMsg(SOCKET sd) 
{
	MsgBuffer* const pMsgBuf = m_bufQueue.ReleasePop();
	if (sd == INVALID_SOCKET || !pMsgBuf)
	{
		m_msg = L"SendBuf check parameter failed.";
		return;
	}
	ThreadPoolPtr pPool = (ThreadPoolPtr) m_pvThreadPool;

	// CAUSTION: remember to delete pMsgBuf in somewhere
	switch (pMsgBuf->type)
	{
	case MSG_ALARM_NOTIFY:
		{
			pPool->QueueRequest(new NetTaskAlarmNotify(sd, pMsgBuf));
		}
		break;

	default :
		delete pMsgBuf;
		break;
	}

}

bool P2PLayer::QueueMsg2AllLink(int type, long long seq, const google::protobuf::Message& msg)
{
	return Msg2Queue(m_bufQueue, type, seq, msg);
}

// for debug info
std::wstring P2PLayer::GetMsgTypeString(int nType)
{
	return Ansi2Unicode(MsgType_Name((MsgType) nType));
}


} // namespace MidLayer

