#include <afx.h>

#include "NetBase.h"
#include <WinSock2.h>
#include <sstream>
#include <google/protobuf/message.h>
#include "..\protobuf\PBNetTest.pb.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// NetBase
//////////////////////////////////////////////////////////////////////////


NetBase::NetBase(bool bServer, void* pvThreadPool)
: m_bServer(bServer)
, m_pvThreadPool(pvThreadPool)
, m_uListenPort(0)
, m_uRemotePort(0)
, m_bPending(false)
, m_bRunning(true)
, m_hConnectThread(NULL)
{
}

NetBase::~NetBase(void)
{
	WaitThreadToExit();
}

bool NetBase::Init(void)
{
	if (!m_pvThreadPool)
	{
		m_msg = L"Check base parameter failed.";
		return false;
	}
	if (!InitLayer())
	{
		m_msg = L"InitLayer failed.";
		return false;
	}

	if (!StartThread())
	{
		m_msg = L"StartThread failed.";
		return false;
	}

	m_msg.info(L"Init OK");
	return true;
}

bool NetBase::QueueMsg(SOCKET sd, int type, long long seq, const google::protobuf::Message& msg)
{
	int n = msg.ByteSize();
	char* p = new char[n];
	std::auto_ptr<char> ap(p);
	bool berr = false;
	try
	{
		if (!msg.SerializeToArray(p, n))
		{
			throw 1;
		}
	}
	catch (google::protobuf::FatalException e)
	{
		berr = true;

		wstring errmsg = Ansi2Unicode(e.what());
		wstringstream ss;
		ss << L"QueueMsg: " << errmsg;
		m_msg = ss;
	}
	catch (...)
	{
		berr = true;
		m_msg = L"QueueMsg: unknown exception.";
	}

	if (berr)
	{
		return false;
	}

	// memory will managed by BufQueue
	m_bufQueue.Add(type, seq, p, n);
	ap.release();
	return true;
}

void NetBase::NotifyThreadToExit()
{
	m_msg = L"Notify thread to exit...";
	m_bRunning = false;
	m_bPending = false;
}

void NetBase::WaitThreadToExit(void)
{
	if (!m_hConnectThread)
	{
		m_msg = L"No thread running...";
		return;
	}

	if (m_bRunning || m_bPending)
	{
		NotifyThreadToExit();
	}

	WaitForSingleObject(m_hConnectThread, INFINITE);
	m_msg = L"All thread exited.";
}

SOCKET NetBase::TryConnect()
{
	if (m_bServer)
	{
		return TryListen(m_uListenPort);
	}
	else
	{
		return TryConnectServer(m_strRemoteAddr.c_str(), m_uRemotePort);
	}
	return INVALID_SOCKET;
}

SOCKET NetBase::TryListen(unsigned short uListenPort)
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
		addr.sin_port = htons(uListenPort);
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

SOCKET NetBase::TryConnectServer(const wchar_t* pRemotAddr, unsigned short uRemotePort)
{
	SOCKET sConn = INVALID_SOCKET;
	if (!pRemotAddr)
	{
		return sConn;
	}
	string addrAnsi = Unicode2Ansi(pRemotAddr);
	if (addrAnsi.empty())
	{
		return sConn;
	}

	sConn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sConn == INVALID_SOCKET) 
	{
		m_msg = L"Connect: socket function failed.";
		return INVALID_SOCKET;
	}

	struct sockaddr_in sockaddrServer; 
	sockaddrServer.sin_family = AF_INET;
	sockaddrServer.sin_addr.s_addr = inet_addr( addrAnsi.c_str() );
	sockaddrServer.sin_port = htons( uRemotePort );

	if (SOCKET_ERROR == 
		connect( sConn, (SOCKADDR*) &sockaddrServer, sizeof(sockaddrServer) )) 
	{
		m_msg = L"Connect: connect function failed.";
		closesocket(sConn);
		return INVALID_SOCKET;
	}

	return sConn;
}

int NetBase::SendMsg(SOCKET sd, const int type, long long seq, const google::protobuf::Message& msg)
{
	const int len = msg.ByteSize();
	char* const buf = new char[len];
	std::auto_ptr<char> apb(buf);
	try
	{
		if (!msg.SerializeToArray(buf, len))
		{
			// failed
			throw 1;
		}
	}
	catch (google::protobuf::FatalException e)
	{
		wstring errmsg = Ansi2Unicode(e.what());
		wstringstream ss;
		ss << L"QueueMsg: " << errmsg;
		m_msg = ss;
	}
	catch (...)
	{
		m_msg = L"QueueMsg: unknown exception.";
	}

	MsgBuffer* pMsgBuf = new MsgBuffer(type, seq, buf, len);
	apb.release();
	std::auto_ptr<MsgBuffer> apm(pMsgBuf);

	return SendMsg(sd, pMsgBuf);
}

int NetBase::SendMsg(SOCKET sd, MsgBuffer* const pMsgBuf, char* const pHdrBuf /*= NULL*/)
{
	const int nHeader = NETLAYER_PB_HEADER_LENGTH;

	char* pHeader = pHdrBuf;
	std::auto_ptr<char> aph;
	if (!pHdrBuf)
	{
		pHeader = new char[nHeader];
		aph.reset(pHeader);
	}


	PBNetTest::HeadMsg header;
	header.set_syn(NETLAYER_PB_HEADER_SYN);
	header.set_pro(NETLAYER_PB_HEADER_VER);
	header.set_type(pMsgBuf->type);
	header.set_seqno(pMsgBuf->seq);
	header.set_len(pMsgBuf->len);

	if (nHeader != header.ByteSize())
	{
		m_msg = L"SendMsg failed: header.size";
		return 2;
	}

	if (!header.SerializeToArray(pHeader, nHeader))
	{
		m_msg = L"SendMsg failed: header.Serialize";
		return 3;
	}

	if (SocketSendData(sd, pHeader, nHeader, m_bRunning) != nHeader)
	{
		m_msg = L"SendMsg failed: send header";
		return 4;
	}

	if (SocketSendData(sd, pMsgBuf->buf, pMsgBuf->len, m_bRunning) != pMsgBuf->len)
	{
		m_msg = L"SendMsg failed: send msg";
		return 5;
	}

	wstringstream ss;
	ss << L"***PB msg sent: t " << pMsgBuf->type << L", s " << pMsgBuf->seq << L", l" << pMsgBuf->len;
	m_msg.info(ss);

	return 0;
}

// send or recv fix data over socket
int NetBase::SocketProcData(SOCKET& sd, char* pBuffer, const int nDataByte, volatile bool& bRunning, bool bSend /*= true*/)
{
	typedef int (__stdcall*pfnSocket)(SOCKET ss, char* buf, int bytes, int flags);

	const bool bRecv = !bSend;

	pfnSocket fnSocket = 0;
	if (bSend)
	{
		fnSocket = (pfnSocket)send;
	}
	else
	{
		fnSocket = recv;
	}

	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = NETLAYER_SELECT_USEC;
	fd_set fdSet;
	int nTotal = 0;
	while (nDataByte > nTotal)
	{
		if (!bRunning)
		{
			break;
		}

		// select socket
		FD_ZERO(&fdSet);
		FD_SET(sd, &fdSet);
		int nRet = 0;
		if (bSend)
		{
			nRet = select(0, 0, &fdSet, 0, &tv);
		}
		else
		{
			nRet = select(0, &fdSet, 0, 0, &tv);
		}

		// check select state
		if (nRet == SOCKET_ERROR)
		{
			return SOCKET_ERROR;
		}
		else if (nRet == 0)
		{
			// recv: no data or send: busy
			if (bRecv && FailWhenNoRecv())
			{
				// recv time-out
				return SOCKET_ERROR;
			}
			Doze();
			continue;
		}

		// send or recv data
		int nRecv = fnSocket(sd, pBuffer + nTotal, nDataByte - nTotal, 0);
		if (nRecv <= 0)
		{
			return nRecv;
		}
		nTotal += nRecv;

		if (bRecv)
		{
			OnDataReceived();
		}
	}
	return nTotal;
}

bool NetBase::StartThread(void)
{
	WaitThreadToExit();
	m_bRunning = true;
	m_bPending = true;

	m_hConnectThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ConnectThread, this, 0, NULL);
	if (!m_hConnectThread)
	{
		m_msg = L"Create ConnectThread failed:";
		WaitThreadToExit();
		return false;
	}
	// run thread
	m_bPending = false;
	return true;
}

unsigned NetBase::ConnectThread(void* lpLayer)
{
	NetBase* pLayer = (NetBase*)lpLayer;
	pLayer->Pending();

	MsgQueue& msg = pLayer->m_msg;
	volatile bool& bRunning = pLayer->m_bRunning;

	while (bRunning)
	{
		SOCKET sd = pLayer->TryConnect();
		if (sd == INVALID_SOCKET)
		{
			pLayer->Doze(NETLAYER_SLEEP_TRYCONNECT);
			continue;
		}
		msg.info(L"ConnectThread: connected.");
		pLayer->OnConnect(sd);
	}

	CloseHandle(pLayer->m_hConnectThread);
	pLayer->m_hConnectThread = NULL;
	return 0;
}

void NetBase::Pending(void)
{
	while (m_bPending && m_bRunning)
	{
		Doze();
	}
}

void NetBase::Doze(unsigned dwMillisec /*= NETLAYER_DOZE_DEFAULT_MS*/)
{
	const unsigned nSlice = NETLAYER_DOZE_SLICE_MS;
	if (dwMillisec < nSlice)
	{
		Sleep(dwMillisec);
		return;
	}
	for (unsigned ii = 0, ni = dwMillisec / nSlice; 
		(ii < ni) && m_bRunning; ++ii)
	{
		Sleep(nSlice);
	}
}

bool NetBase::ParseMsg(google::protobuf::Message& msg, const char* pBuffer, const int nByte)
{
	if (!msg.ParseFromArray(pBuffer, nByte))
	{
		m_msg = L"ParseFromArray failed.";
		return false;
	}
	return true;
}
