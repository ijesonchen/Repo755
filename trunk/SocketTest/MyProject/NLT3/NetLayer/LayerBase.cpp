///////////////////////////////////////////////////////////
//  LayerBase.cpp
//  Implementation of the Class LayerBase
//  Created on:      28-¾ÅÔÂ-2015 9:20:34
//  Original author: user
///////////////////////////////////////////////////////////

#include "LayerBase.h"
#include <string>
#include <sstream>
#include <WinSock2.h>
#include "..\Public\PublicFunc.h"
#include "..\protobuf\PBNetTest.pb.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace NetLayer
{


LayerBase::LayerBase(void* pvThreadPool)
: m_pvThreadPool(pvThreadPool)
, m_bRunning(false)
, m_bPending(false)
, m_hConnectThread(NULL)
, m_bServer(false)
, m_bClientConnected(false)
, m_uListenPort(0)
, m_uRemotePort(0)
{

}



LayerBase::~LayerBase(){
	ExitWait();
}

// for debug info
std::wstring LayerBase::GetMsgTypeString(int nType)
{
	wstringstream ss;
	ss << nType << L"(over-write for type name)";
	return ss.str();
}

unsigned int LayerBase::ConnectThread(void* pvBase){

	LayerBase* pLayer = (LayerBase*)pvBase;
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

		LinkInfo info = GetLinkInfo(sd);
		if (!info.IsValid())
		{
			msg = L"ConnectThread: GetLinkInfo failed.";
			ClearSocket(sd);
			continue;
		}

		wstringstream ss;
		ss << L"ConnectThread: connected " << info.m_remoteAddr << L":" << info.m_remotePort
			<< L" in " << info.m_localAddr << ":" << info.m_localPort;
		msg.info(ss);
		pLayer->OnConnected(sd, info);
	}

	CloseHandle(pLayer->m_hConnectThread);
	pLayer->m_hConnectThread = NULL;
	return 0;
}


SOCKET LayerBase::TryConnect()
{
	if (m_bServer)
	{
		return Listen(m_uListenPort);
	}
	else if (!m_bClientConnected)
	{
		return Connect(m_strRemoteAddr.c_str(), m_uRemotePort);
	}
	return INVALID_SOCKET;
}


bool LayerBase::Init(){
	if (!m_pvThreadPool)
	{
		m_msg = L"Check base parameter failed.";
		return false;
	}
	ExitWait();
	m_bRunning = true;
	m_bPending = true;

	m_hConnectThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ConnectThread, this, 0, NULL);
	if (!m_hConnectThread)
	{
		m_msg = L"Create connect thread failed";
		return false;
	}

	if (!InitLayer())
	{
		m_msg = L"InitLayer failed.";
		return false;
	}
	// run thread
	m_bPending = false;
	return true;
}

// send or recv fix data over socket
int LayerBase::SocketProcData(SOCKET& sd, char* pBuffer, const int nDataByte, bool bSend /*= true*/)
{
	typedef int (__stdcall*pfnSocket)(SOCKET ss, char* buf, int bytes, int flags);

	const time_t tmTimeOutSec = NETLAYER_SOCKET_TIMEOUT_SEC;

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

	time_t tmBegin = time(NULL);
	while (nDataByte > nTotal)
	{
		if (!m_bRunning)
		{
			break;
		}

		if (time(NULL) - tmBegin > tmTimeOutSec)
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
			if (NETLAYER_SHOW_DETAIL_MSG)
			{
				m_msg = L"SocketProcData select SOCKET_ERROR";
			}
			return SOCKET_ERROR;
		}
		else if (nRet == 0)
		{
			// recv: no data or send: busy
			Doze();
			continue;
		}

		// send or recv data
		int nRecv = fnSocket(sd, pBuffer + nTotal, nDataByte - nTotal, 0);
		if (nRecv <= 0)
		{
			if (NETLAYER_SHOW_DETAIL_MSG)
			{
				if (nRecv < 0)
				{
					m_msg = L"SocketProcData fnSocket error";
				}
				else
				{
					m_msg.info(L"SocketProcData socket closed.");
				}
			}
			return nTotal;
		}
		nTotal += nRecv;
	}
	if (NETLAYER_SHOW_DETAIL_MSG)
	{
		wstringstream ss;
		ss << L"SocketProcData proced " << nTotal << L" bytes.";
		m_msg.info(ss);
	}
	return nTotal;
}


int LayerBase::SendBuf(SOCKET sd, BufQueue& bufQueue, char* const pbufHdr){

	if (sd == INVALID_SOCKET || !pbufHdr)
	{
		m_msg = L"SendBuf check parameter failed.";
		return -1;
	}

	MsgBuffer* const pMsgBuf = bufQueue.ReleasePop();
	std::auto_ptr<MsgBuffer> apSend(pMsgBuf);
	if (!pMsgBuf)
	{
		m_msg.info(L"SendThread: no data.");
		return 1;
	}

	const int nHeader = NETLAYER_PB_HEADER_LENGTH;
	const int nSyn = NETLAYER_PB_HEADER_SYN;
	const int nVer = NETLAYER_PB_HEADER_VER;

	PBNetTest::HeadMsg header;
	header.set_syn(nSyn);
	header.set_pro(nVer);
	header.set_type(pMsgBuf->type);
	header.set_seqno(pMsgBuf->seq);
	header.set_len(pMsgBuf->len);

	int nRet = 0;
	try
	{
		if (nHeader != header.ByteSize())
		{
			m_msg = L"SendBuf failed: header.size";
			return -2;
		}

		if (NETLAYER_SHOW_DETAIL_MSG)
		{
			wstringstream ss;
			ss << L"HeadMsg is about to Serialize: " << PublicFunc::DecodeUtf8(header.Utf8DebugString());
			m_msg.info(ss);
		}

		if (!header.SerializeToArray(pbufHdr, nHeader))
		{
			m_msg = L"SendBuf failed: header.Serialize";
			return -3;
		}

		if (SocketSendData(sd, pbufHdr, nHeader) != nHeader)
		{
			m_msg = L"SendBuf failed: send header";
			return -4;
		}

		if (SocketSendData(sd, pMsgBuf->buf, pMsgBuf->len) != pMsgBuf->len)
		{
			m_msg = L"SendBuf failed: send msg";
			return -5;
		}

		nRet = 0;

		wstringstream ss;
		ss << L"***MsgBuffer sent: t " << pMsgBuf->type << L", s " << pMsgBuf->seq << L", l" << pMsgBuf->len;
		m_msg.info(ss);
	}
	catch (int code)
	{
		nRet = code;
	}
	catch (google::protobuf::FatalException e)
	{
		m_msg = Ansi2Unicode(e.what());
		nRet = -11;
	}
	catch (...)
	{
		m_msg = L"SendBuf: unknown exception";
		nRet = -11;
	}

	if (nRet)
	{
		apSend.release();
		bufQueue.Capture(pMsgBuf);
	}

	return nRet;
}


LinkInfo LayerBase::GetLinkInfo(SOCKET sd){

	LinkInfo info;
	sockaddr_in connAddr;
	int nAddr = sizeof(sockaddr_in);
	// local info
	if (SOCKET_ERROR == getsockname(sd, (sockaddr*)&connAddr, &nAddr))
	{
		return info;
	}
	string lAddr;
	unsigned short lPort = 0;

	lAddr = inet_ntoa(connAddr.sin_addr);
	lPort = ntohs(connAddr.sin_port);
	// remote info
	if (SOCKET_ERROR == getpeername(sd, (sockaddr*)&connAddr, &nAddr))
	{
		return info;
	}
	string rAddr;
	unsigned short rPort = 0;
	rAddr = inet_ntoa(connAddr.sin_addr);
	rPort = ntohs(connAddr.sin_port);

	info.m_localAddr = Ansi2Unicode(lAddr);
	info.m_localPort = lPort;
	info.m_remoteAddr = Ansi2Unicode(rAddr);
	info.m_remotePort = rPort;

	return  info;
}

bool LayerBase::Msg2Queue(BufQueue& bufQueue, int type, long long seq, const google::protobuf::Message& msg)
{
	int n = msg.ByteSize();
	char* p = new char[n];
	std::auto_ptr<char> ap(p);
	bool berr = false;
	try
	{
		if (NETLAYER_SHOW_DETAIL_MSG)
		{
			wstringstream ss;
			ss << L"Msg is about to Serialize: type " << GetMsgTypeString(type) 
				<< ", PB name " << PublicFunc::DecodeUtf8(msg.GetTypeName())  << endl
				<< PublicFunc::DecodeUtf8(msg.Utf8DebugString());
			m_msg.info(ss);
		}

		if (!msg.SerializeToArray(p, n))
		{
			throw 1;
		}
	}
	catch (google::protobuf::FatalException e)
	{
		berr = true;

		CString strw;
		strw = e.what();
		wstringstream ss;
		ss << L"QueueMsg: " << strw.GetString();
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
	bufQueue.Push(type, seq, p, n);
	ap.release();
	return true;
}

bool LayerBase::ParseMsg(google::protobuf::Message&  msg, const char* const pBuffer, const int  nByte){

	try
	{
		if (!msg.ParseFromArray(pBuffer, nByte))
		{
			m_msg = L"ParseFromArray failed.";
			return false;
		}

		if (NETLAYER_SHOW_DETAIL_MSG)
		{
			wstringstream ss;
			ss << "MsgParsed: " << PublicFunc::DecodeUtf8(msg.Utf8DebugString());
		}

		return true;
	}
	catch(google::protobuf::FatalException e)
	{
		wstringstream ss;
		ss << "ParseFromArray exception: " << Ansi2Unicode(e.what());
		m_msg = ss;
	}
	return false;
}


void LayerBase::ExitNotify(){
	if (NETLAYER_SHOW_EXIT_MSG)
	m_msg.info(L"Notify thread to exit...");
	m_bRunning = false;
	m_bPending = false;
}


void LayerBase::ExitWait(){
	ExitNotify();

	WaitForSingleObject(m_hConnectThread, INFINITE);
	WaitLayerExit();
	if (NETLAYER_SHOW_EXIT_MSG)
	m_msg.info(L"All threads exited.");
}

void LayerBase::Pending(){
	while (m_bPending && m_bRunning)
	{
		Doze();
	}
}


void LayerBase::Doze(const unsigned int uMillisecs /*= NETLAYER_DOZE_DEFAULT_MS*/){

	const unsigned nSlice = NETLAYER_DOZE_SLICE_MS;
	if (uMillisecs < nSlice)
	{
		Sleep(uMillisecs);
		return;
	}
	unsigned tickBeg = GetTickCount();
	while (m_bRunning && 
		(GetTickCount() - tickBeg < uMillisecs))
	{
		Sleep(nSlice);
	}
}

SOCKET LayerBase::Listen(unsigned short uListenPort)
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

			closesocket(sListen);
			return connSock;
		} // accept loop
		closesocket(sListen);
	} // listen loop

	return INVALID_SOCKET;
}

SOCKET LayerBase::Connect(const std::wstring&  addr, const unsigned short uport){

	SOCKET sConn = INVALID_SOCKET;

	sConn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sConn == INVALID_SOCKET) 
	{
		m_msg = L"Connect: socket function failed.";
		return INVALID_SOCKET;
	}

	const int CONNECT_TIME_OUT = -1000;

	try
	{
		int nRet = 0;

		// set socket to non-blocking mode
		unsigned long bNonBlocking = true;
		if (nRet = ioctlsocket(sConn, FIONBIO, &bNonBlocking))
		{
			throw nRet;
		}

		struct sockaddr_in sockaddrServer; 
		sockaddrServer.sin_family = AF_INET;
		sockaddrServer.sin_addr.s_addr = inet_addr( Unicode2Ansi(addr).c_str() );
		sockaddrServer.sin_port = htons( uport );

		if (SOCKET_ERROR != 
			connect( sConn, (SOCKADDR*) &sockaddrServer, sizeof(sockaddrServer) )) 
		{
			throw -1;
		}
		if (WSAEWOULDBLOCK != WSAGetLastError())
		{
			throw -2;
		}

		timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = NETLAYER_SELECT_USEC;
		fd_set fdSet;
		time_t tmMax = NETLAYER_SOCKET_TIMEOUT_SEC;
		time_t tmBeg = time(NULL);
		while (m_bRunning)
		{
			if (time(NULL) - tmBeg >= tmMax)
			{
				throw CONNECT_TIME_OUT;
			}

			// select socket
			FD_ZERO(&fdSet);
			FD_SET(sConn, &fdSet);
			int nRet = 0;
			nRet = select(0, 0, &fdSet, 0, &tv);

			// check select state
			if (nRet == SOCKET_ERROR)
			{
				throw -4;
			}
			else if (nRet == 0)
			{
				// connect: not ready
				Doze();
				continue;
			}

			bNonBlocking = false;
			if (nRet = ioctlsocket(sConn, FIONBIO, &bNonBlocking))
			{
				throw -5;
			}

			return sConn;
		}

		// not running
		return INVALID_SOCKET;
	}
	catch (const int code)
	{
		if (code == CONNECT_TIME_OUT)
		{
			m_msg.info(L"Connect timed-out.");
		}
		else
		{
			wstringstream ss;
			ss << L"Connect: failed with code " << code;
			m_msg = ss;
		}
	}

	closesocket(sConn);
	return INVALID_SOCKET;
}

void LayerBase::ClearSocket(SOCKET& sd)
{
	if (sd != INVALID_SOCKET)
	{
		shutdown(sd, SD_BOTH);
		closesocket(sd);
		sd = INVALID_SOCKET;
	}
}


} // namespace NetLayer