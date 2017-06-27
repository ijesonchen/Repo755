#include "NetLinkTask.h"
#include <WinSock2.h>
#include <sstream>
#include <iostream>
#include "../protobuf/PBNetTest.pb.h"
#include "NetBase.h"
#include "NetLink.h"
using namespace std;
using namespace PBNetTest;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


NetLinkTask::NetLinkTask(NetBase* pBase, const LinkInfo link, SOCKET s)
: pLayer(pBase)
, linkInfo(link)
, m_sdRecv(s)
, bLongConn(false)
, m_hSendThread(NULL)
{
}

NetLinkTask::~NetLinkTask()
{
	if (m_sdRecv != INVALID_SOCKET)
	{
		shutdown(m_sdRecv, SD_BOTH);
		closesocket(m_sdRecv);
	}
}

bool NetLinkTask::Init(void)
{
	return false;
}

void NetLinkTask::DoTask(void *pvParam, OVERLAPPED *pOverlapped)
{
	RecvTask(pvParam, pOverlapped);
}

void NetLinkTask::RecvTask(void* , OVERLAPPED* )
{
	if (!pLayer)
	{
		return;
	}
	const int nHeader = NETLAYER_PB_HEADER_LENGTH;
	const int nMaxMsgLen = NETLAYER_PB_MAX_MSG_LENGTH;
	const int nSyn = NETLAYER_PB_HEADER_SYN;
	MsgQueue& msg = pLayer->GetMsgRef();
	volatile bool& bRunning = pLayer->GetRunFlagRef();

	CharBuffer cpHeader;
	if (!cpHeader.size(nHeader))
	{
		msg.info(L"task failed: no enough mem.");
		return;
	}
	
	CharBuffer cpMsg;

	bool bFailed = false;

	do
	{
		if (bFailed)
		{
			pLayer->OnNetFailure();
			break;
		}
		bFailed = true;
		if (pLayer->SocketRecvData(m_sdRecv, cpHeader.ptr(), nHeader, bRunning) != nHeader)
		{
			msg = L"Recv HeadMsg failed.";
			continue;
		}
		HeadMsg header;
		if (!header.ParseFromArray(cpHeader.ptr(), nHeader))
		{
			msg = L"Parse HeadMsg failed.";
			continue;
		}

		// check header
		if (header.syn() != nSyn)
		{
			wstringstream ss;
			ss << L"HeadMsg syn flag error: " << header.syn();
			msg = ss.str();
			continue;
		}
		const int nMsglen = header.len();
		if (nMsglen < 0 || nMsglen > nMaxMsgLen)
		{
			wstringstream ss;
			ss << L"Msg length error: " << nMsglen;
			msg = ss.str();
			continue;
		}

		// message body
		if (!cpMsg.size(nMsglen))
		{
			msg = L"Alloc msg mem failed.";
			continue;
		}
		memset(cpMsg.ptr(), 0, nMsglen);
		if (pLayer->SocketRecvData(m_sdRecv, cpMsg.ptr(), nMsglen, bRunning) != nMsglen)
		{
			wstringstream ss;
			ss << L"Recv msg failed: " << header.type() << L" " << nMsglen << L" bytes.";
			msg = ss;
			continue;
		}
		if (!pLayer->OnRecvMsg(header.seqno(), header.type(), cpMsg.ptr(), nMsglen))
		{
			wstringstream ss;
			ss << L"Proc msg failed: " << header.type() << L" " << nMsglen << L" bytes.";
			msg = ss.str();
			continue;
		}

		bFailed = false;
	} while (bLongConn && bRunning);

	msg.info(L"task exit.");
}

unsigned NetLinkTask::SendThread(void* lpLinkTask)
{
	NetLinkTask* pLink = (NetLinkTask*)lpLinkTask;
	NetBase* pLayer = pLink->pLayer;
	pLayer->Pending();

	volatile bool& bRunning = pLayer->m_bRunning;
	MsgQueue& msg = pLayer->m_msg;

	BufQueue& bufQueue = pLayer->m_bufQueue;

	while (bRunning)
	{
		MsgBuffer* const pMsgBuf = bufQueue.GetAndRelease();
		if (!pMsgBuf)
		{
			msg.info(L"SendThread: no data.");
			pLayer->OnNoDataSend();
			continue;
		}
		SOCKET sd = pLayer->GetSendSocket();
		if (sd == INVALID_SOCKET)
		{
			msg.info(L"SendThread: GetSendSocket not connected.");
			pLayer->Doze(NETLAYER_SLEEP_TRYCONNECT);
			continue;
		}

		pLayer->OnSend(sd, pMsgBuf);
	}

	CloseHandle(pLink->m_hSendThread);
	pLink->m_hSendThread = NULL;
	return 0;
}
