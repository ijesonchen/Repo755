///////////////////////////////////////////////////////////
//  NetLink.cpp
//  Implementation of the Class NetLink
//  Created on:      28-¾ÅÔÂ-2015 9:20:34
//  Original author: user
///////////////////////////////////////////////////////////

#include "NetLink.h"
#include <sstream>
#include <WinSock2.h>
#include "..\protobuf\PBNetTest.pb.h"

using namespace PBNetTest;
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace NetLayer
{


NetLink::NetLink(SOCKET sd, const LinkInfo& info, LayerBase* pLayer)
: m_linkInfo(info)
, m_pLayer(pLayer)
, m_bLinking(false)
, m_bRecving(true)
, m_sdRecv(sd)
, m_sdSend(INVALID_SOCKET)
, m_hRecvThread(NULL)
, m_hSendThread(NULL)
, m_tmLastRecv(0)
, m_tmLastSend(0)
{
}



NetLink::~NetLink(){

	if (m_hRecvThread)
	{
		ExitWait();
	}
}





bool NetLink::Init(){

	// check parameter
	if (m_sdRecv == INVALID_SOCKET || !m_linkInfo.IsValid() ||
		!m_pLayer )
	{
		return false;
	}

	if (m_hRecvThread || m_hSendThread)
	{
		ExitWait();
	}

	m_bLinking = true;

	wstringstream ss;
	ss << L"Create thread failed:";
	m_hRecvThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)RecvThread, this, 0, NULL);
	if (!m_hRecvThread)
	{
		ss << L" RecvThread";
	}
	m_hSendThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SendThread, this, 0, NULL);
	if (!m_hSendThread)
	{
		ss << L" SendThread";
	}
	ss << L" thread.";
	if (!m_hRecvThread || !m_hSendThread)
	{
		m_pLayer->Msg() = ss;
		ExitWait();
		return false;
	}

	return true;
}

bool NetLink::QueueMsg(int type, long long seq, const google::protobuf::Message& msg)
{
	return m_pLayer->Msg2Queue(m_bufQueue, type, seq, msg);
}

void NetLink::ExitNotify()
{
	m_bLinking = false;
}

void NetLink::ExitWait()
{
	if (NETLAYER_SHOW_EXIT_MSG)
	m_pLayer->Msg() = L"NetLink: Wait threads to exit...";
	ExitNotify();
	// recv thread is response for clean NetLink
	// should only wait recv thread is enough
	WaitForSingleObject(m_hRecvThread, INFINITE);
}

void NetLink::RecvPending(void)
{
	if (!m_pLayer || m_sdRecv == INVALID_SOCKET)
	{
		return;
	}

	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = NETLAYER_SELECT_USEC;
	fd_set fdSet;
	int nTotal = 0;

	while (m_pLayer->Running())
	{
		// select socket
		FD_ZERO(&fdSet);
		FD_SET(m_sdRecv, &fdSet);
		int nRet = 0;
		if (select(0, &fdSet, 0, 0, &tv) != 0)
		{
			// error or active
			return;
		}
	}
}

unsigned int NetLink::RecvThread(NetLink* pvLink){

	if (!pvLink)
	{
		return (unsigned)-1;
	}
	const int nHeader = NETLAYER_PB_HEADER_LENGTH;
	const int nMaxMsgLen = NETLAYER_PB_MAX_MSG_LENGTH;
	const int nSyn = NETLAYER_PB_HEADER_SYN;
	char* pHdr = new char[nHeader];
	std::auto_ptr<char> ap(pHdr);

	NetLink* pLink = pvLink;
	LayerBase* const pLayer = pLink->m_pLayer;
	SOCKET& sd = pLink->m_sdRecv;

	CharBuffer cpMsg;

	pLink->m_tmLastRecv = time(NULL);
	while (pLayer->Running() && pLink->m_bLinking)
	{
		if (!pLink->RecvCheck())
		{
			break;
		}
		pLink->RecvPending();
		if (pLayer->SocketRecvData(sd, pHdr, nHeader) != nHeader)
		{
			pLayer->Msg() = L"NetLink Recv HeadMsg failed.";
			break;
		}
		HeadMsg header;
		if (!header.ParseFromArray(pHdr, nHeader))
		{
			pLayer->Msg() = L"NetLink Parse HeadMsg failed.";
			break;
		}

		// check header
		if (header.syn() != nSyn)
		{
			wstringstream ss;
			ss << L"NetLink HeadMsg syn flag error: " << header.syn();
			pLayer->Msg() = ss;
			break;
		}
		const int nMsglen = header.len();
		if (nMsglen < 0 || nMsglen > nMaxMsgLen)
		{
			wstringstream ss;
			ss << L"NetLink Msg length error: " << nMsglen;
			pLayer->Msg() = ss;
			break;
		}

		// message body
		if (!cpMsg.size(nMsglen))
		{
			pLayer->Msg() = L"NetLink Alloc msg mem failed.";
			break;
		}
		memset(cpMsg.ptr(), 0, nMsglen);
		if (pLayer->SocketRecvData(sd, cpMsg.ptr(), nMsglen) != nMsglen)
		{
			wstringstream ss;
			ss << L"NetLink Recv msg failed: " << header.type() << L" " << nMsglen << L" bytes.";
			pLayer->Msg() = ss;
			break;
		}

		if (NETLAYER_SHOW_DETAIL_MSG)
		{
			wstringstream ss;
			ss << L"NetLink msg recved: " << pLink->GetMsgTypeString(header.type()) << L" " << nMsglen << L" bytes.";
			pLayer->Msg().info(ss);
		}

		if (!pLink->OnMsgRecved(header.seqno(), header.type(), cpMsg.ptr(), nMsglen))
		{
			wstringstream ss;
			ss << L"NetLink Proc msg failed: " << header.type() << L" " << nMsglen << L" bytes.";
			pLayer->Msg() = ss.str();
			break;
		}

		pLink->m_tmLastRecv = time(NULL);
	} 

	if (NETLAYER_SHOW_EXIT_MSG)
	pLayer->Msg().info(L"NetLink recv thread ready to exit. call ExitNotify");
	pLink->ExitNotify();

	if (NETLAYER_SHOW_EXIT_MSG)
	pLayer->Msg().info(L"NetLink recv thread exit. Wait send thread exit.");
	WaitForSingleObject(pLink->m_hSendThread, INFINITE);

	if (NETLAYER_SHOW_EXIT_MSG)
	pLayer->Msg().info(L"NetLink all thread exited. clear socket.");
	pLayer->ClearSocket(pLink->m_sdRecv);

	if (NETLAYER_SHOW_EXIT_MSG)
	pLayer->Msg().info(L"NetLink call Layer to RemoveLink");

	// close handle BEFORE remove link, or thread will lock.
	CloseHandle(pLink->m_hRecvThread);
	pLink->m_hRecvThread = NULL;

	pLink->m_bRecving = false;
	pLayer->RemoveLink(sd);

	return 0;
}

unsigned int NetLink::SendThread(NetLink* pvLink){

	const int nHdr = NETLAYER_PB_HEADER_LENGTH;
	const int nMsgNoData = NETLAYER_PROMPT_SEND_NO_DATA;

	NetLink* pLink = pvLink;
	LayerBase* const pLayer = pLink->m_pLayer;
	pLayer->Pending();
	char* pHdr = new char[nHdr];
	std::auto_ptr<char> ap(pHdr);
	time_t tmMsgNoData = time(NULL);
	pLink->m_tmLastSend = tmMsgNoData;
	while (pLayer->Running() && pLink->m_bLinking)
	{
		SOCKET sd = pLink->GetSendSocket();
		if (sd == INVALID_SOCKET)
		{
			pLayer->Doze(NETLAYER_SLEEP_TRYCONNECT);
			continue;
		}
		if (!pLink->m_bufQueue.HasData())
		{
			if (time(NULL) - tmMsgNoData > nMsgNoData)
			{
				tmMsgNoData = time(NULL);
				pLayer->Msg().info(L"SendThread: no data.");
			}
			pLink->OnNoDataSend();
			continue;
		}
		if (!pLayer->SendBuf(sd, pLink->m_bufQueue, pHdr))
		{
			pLink->m_tmLastSend = time(NULL);
		}
	}

	CloseHandle(pLink->m_hSendThread);
	pLink->m_hSendThread = NULL;

	pLayer->Msg().info(L"NetLink send thread exit.");
	return 0;
}

// for debug info
std::wstring NetLink::GetMsgTypeString(int nType)
{
	wstringstream ss;
	ss << nType << L"(over-write GetMsgTypeString to show type name)";
	return ss.str();
}


} // namespace NetLayer

