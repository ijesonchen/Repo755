#include "MidLayerP2P.h"
#include <sstream>
#include "protobuf/PBNetTest.pb.h"
#include "Public/PublicFunc.h"
#include "Public/ThreadPool.h"
#include "NetLayer/NetLayerRecvTask.h"

using namespace std;
using namespace PBNetTest;
using namespace PublicFunc;
/*

SOCKET m_socket;
std::wstring m_addrUnicode;
std::string m_addrAnsi;
std::wstring m_strSysid;
unsigned short m_uLocalPort;
unsigned short m_uRemotePort;
*/
MidLayerP2P::MidLayerP2P(const wchar_t* pAddr, unsigned short uPort, const wchar_t* pSysId, void* const pvThreadPool)
: NetLayerBase(pvThreadPool)
, m_socket(INVALID_SOCKET)
, m_addrUnicode(pAddr)
, m_strSysid(pSysId)
, m_uListenPort(uPort)
, m_uRemotePort(uPort)
{
	wstringstream ss;
	ss << L"MidLayerP2P-" << pAddr << L":" << uPort;
	m_msg.SetPrefix(ss.str().c_str());
}

MidLayerP2P::~MidLayerP2P(void)
{
}

bool MidLayerP2P::InitLayer(void)
{
	if (m_addrUnicode.empty() || !m_uRemotePort)
	{
		return false;
	}
	m_addrAnsi = Unicode2Ansi(m_addrUnicode.c_str());

	if (m_addrAnsi.empty())
	{
		return false;
	}
	return true;
}

SOCKET MidLayerP2P::TryConnect(void)
{
	const int nSleepRetry = NETLAYER_SLEEP_TRYCONNECT;
	// listen loop
	while (m_bRunning)
	{
		// for retry, doze first...
		SOCKET sListen = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sListen == INVALID_SOCKET)
		{
			m_msg = L"Listen failed, retry...";
			Doze(nSleepRetry);
			continue;
		} 

		sockaddr_in addr;
		memset(&addr, 0, sizeof(sockaddr_in));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(m_uListenPort);
		addr.sin_addr.s_addr= htonl(INADDR_ANY);
		if (SOCKET_ERROR == bind(sListen, (const sockaddr*) & addr, sizeof(addr))) 
		{
			closesocket(sListen);
			continue;
		}
		//listen
		if (SOCKET_ERROR == listen(sListen, SOMAXCONN)) 
		{
			closesocket(sListen);
			continue;
		}

		// accept loop
		while (m_bRunning)
		{
			sockaddr_in connAddr;
			timeval tv;
			fd_set fdListen;

			int nClientAddrLen = sizeof(sockaddr_in);
			tv.tv_sec = 0;
			tv.tv_usec = NETLAYER_SELECT_USEC;
			FD_ZERO(&fdListen);
			FD_SET(sListen,&fdListen);

			int nRet = select(0,&fdListen,NULL,NULL,&tv);
			if (nRet == 0)
			{
				continue;
			}
			else if (nRet < 0)
			{
				// listen failed, re-listen
				break;
			}

			SOCKET connSock = accept(sListen, (sockaddr*)&connAddr, &nClientAddrLen);

			if (connSock == INVALID_SOCKET)
			{
				continue;
			}

			return connSock;
		} // accept loop
	} // listen loop

	return INVALID_SOCKET;
}

void MidLayerP2P::OnConnect(SOCKET connSock)
{
	NetLayerRecvTask* pTask = new NetLayerRecvTask(this, connSock, false);
	ThreadPoolPtr pool = (ThreadPoolPtr)m_pvThreadPool;
	pool->QueueRequest(pTask);
}

bool MidLayerP2P::OnRecvMsg(google::protobuf::int64 sn, const int nType, const char* pBuffer, const int nByte, SOCKET sd /*= INVALID_SOCKET*/) 
{
	bool bRet = false;

	// NOTE for return and sd switch-case :
	// if start a task with sd, do sd = INVALID_SOCKET and close sd in task, 

	switch (nType)
	{
	case MSG_SEARCH_ABORT_REQ    :
		{
			SearchAbortRequest msg;
			if (ParseMsg(msg, pBuffer, nByte))
			{
				ThreadPoolPtr pool = (ThreadPoolPtr) m_pvThreadPool;
				if (!pool)
				{
					return false;
				}
	//			pool->QueueRequest(new NetTaskSearchAbortReq(sd, nType, sn, msg));
				sd = INVALID_SOCKET;
				return true;
			}
		}
		break;

	default:
		m_msg = L"OnRecvMsg: bad msg type.";
		break;
	}

	return bRet;
}

void MidLayerP2P::OnNoDataSend(void)
{
}

SOCKET MidLayerP2P::GetSendSocket(void)
{
	return SOCKET();
}

void MidLayerP2P::OnSend(SOCKET sd, MsgBuffer* const pMsgBuf )
{
	delete pMsgBuf;
}