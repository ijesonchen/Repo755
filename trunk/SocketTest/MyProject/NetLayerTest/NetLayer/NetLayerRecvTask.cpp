#include "NetLayerRecvTask.h"
#include <WinSock2.h>
#include <sstream>
#include <iostream>
#include "../protobuf/PBNetTest.pb.h"
#include "NetLayerBase.h"
using namespace std;
using namespace PBNetTest;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


NetLayerRecvTask::NetLayerRecvTask(void* pvNetLayer, SOCKET s, bool isLongConn)
: pvLayer(pvNetLayer)
, sd(s)
, bLongConn(isLongConn)
{
}

NetLayerRecvTask::~NetLayerRecvTask()
{
	if (sd != INVALID_SOCKET)
	{
		shutdown(sd, SD_BOTH);
		closesocket(sd);
	}
}

void NetLayerRecvTask::DoTask(void* , OVERLAPPED* )
{
	if (!pvLayer)
	{
		return;
	}
	const int nHeader = NETLAYER_PB_HEADER_LENGTH;
	const int nMaxMsgLen = NETLAYER_PB_MAX_MSG_LENGTH;
	const int nSyn = NETLAYER_PB_HEADER_SYN;
	NetLayerBase* pLayer = (NetLayerBase*) pvLayer;
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
		if (pLayer->SocketRecvData(sd, cpHeader.ptr(), nHeader, bRunning) != nHeader)
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
		if (pLayer->SocketRecvData(sd, cpMsg.ptr(), nMsglen, bRunning) != nMsglen)
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
