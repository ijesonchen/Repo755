#include <afx.h>
#include "NetLayerShort.h"
#include <sstream>
#include "../Public/ThreadPool.h"
#include "../PBNetTest.pb.h"

using namespace std;
using namespace NetLayer;
using namespace PBNetTest;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace NetLayer
{


NetLayerShort::NetLayerShort(unsigned short uLocalPort, 
							 const wchar_t* pRemoteAddr , unsigned short uRemotePort, 
							 const wchar_t* pSysid, void* pvThreadPool)
: NetLayerBase(pRemoteAddr, uRemotePort, pSysid, pvThreadPool)
, m_uLocalPort(uLocalPort)
{
}

NetLayerShort::~NetLayerShort(void)
{
}

// init & start thread
bool NetLayerShort::InitLayer(void)
{
	if (!StartThread((LPTHREAD_START_ROUTINE)ListenThread,
		(LPTHREAD_START_ROUTINE)SendThread))
	{
		return false;
	}

	return true;
}

// thread
unsigned NetLayerShort::ListenThread(void* lpLayer)
{
	NetLayerShort* pLayer = (NetLayerShort*)lpLayer;
	pLayer->Pending();

	int nRet = 0;

	MsgQueue& msg = pLayer->m_msg;
	volatile bool& bRunning = pLayer->m_bRunning;

	unsigned short& uPort = pLayer->m_uLocalPort;

	try
	{
		// listen loop
		while (bRunning)
		{
			// for retry, doze first...
			pLayer->Doze();
			SOCKET sListen = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (sListen == INVALID_SOCKET)
			{
				msg = L"Listen failed, retry...";
				continue;
			} 

			sockaddr_in addr;
			memset(&addr, 0, sizeof(sockaddr_in));
			addr.sin_family = AF_INET;
			addr.sin_port = htons(uPort);
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
			while (bRunning)
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

				pLayer->OnConnection(connSock);
			} // accept loop
		} // listen loop
	}
	catch (int nCode)
	{
		nRet = nCode;
	}
	catch (...)
	{
		// do nothing 
		nRet = -1;
	}

	CloseHandle(pLayer->m_hInThread);
	pLayer->m_hInThread = NULL;
	pLayer->m_dwInThread = 0;
	return nRet;
}

unsigned NetLayerShort::SendThread(void* lpLayer)
{
	NetLayerShort* pLayer = (NetLayerShort*)lpLayer;
	pLayer->Pending();

	volatile bool& bRunning = pLayer->m_bRunning;
	MsgQueue& msg = pLayer->m_msg;

	BufQueue& bufQueue = pLayer->m_bufQueue;

	while (bRunning)
	{
		// CAUTION: Get() will release management of pMsgBuf from bufQueue
		MsgBuffer* const pMsgBuf = bufQueue.GetAndRelease();
		if (!pMsgBuf)
		{
			pLayer->Doze();
			continue;
		}

		SOCKET sd = pLayer->Connect();
		if (sd == INVALID_SOCKET)
		{
			bufQueue.ReUse(pMsgBuf);
			msg = L"SendThread: connect failed.";
			pLayer->Doze(NETLAYER_SLEEP_RECONNECT);
			continue;
		}

		// task for process connection
		pLayer->OnSendMsg(sd, pMsgBuf);
	}

	CloseHandle(pLayer->m_hOutThread);
	pLayer->m_hOutThread = NULL;
	pLayer->m_dwOutThread = 0;
	return 0;
}



} // namespace NetLayer
