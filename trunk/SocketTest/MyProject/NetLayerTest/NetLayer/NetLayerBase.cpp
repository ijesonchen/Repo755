#include <afx.h>

#include "NetLayerBase.h"
#include <WinSock2.h>
#include <sstream>
#include <google/protobuf/message.h>
#include "..\protobuf\PBNetTest.pb.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// NetLayerBase
//////////////////////////////////////////////////////////////////////////

// namespace NetLayer
// {


NetLayerBase::NetLayerBase(void* pvThreadPool)
: m_pvThreadPool(pvThreadPool)
, m_bPending(false)
, m_bRunning(true)
, m_hConnectThread(NULL)
, m_hSendThread(NULL)
{
}

NetLayerBase::~NetLayerBase(void)
{
	WaitThreadToExit();
}

bool NetLayerBase::Init(void)
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

SOCKET NetLayerBase::Connect(const std::string& addr, const unsigned short uport)
{
	SOCKET sConn = INVALID_SOCKET;

	sConn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sConn == INVALID_SOCKET) 
	{
		m_msg = L"Connect: socket function failed.";
		return INVALID_SOCKET;
	}

	struct sockaddr_in sockaddrServer; 
	sockaddrServer.sin_family = AF_INET;
	sockaddrServer.sin_addr.s_addr = inet_addr( addr.c_str() );
	sockaddrServer.sin_port = htons( uport );

	if (SOCKET_ERROR == 
		connect( sConn, (SOCKADDR*) &sockaddrServer, sizeof(sockaddrServer) )) 
	{
		m_msg = L"Connect: connect function failed.";
		closesocket(sConn);
		return INVALID_SOCKET;
	}

	return sConn;
}

bool NetLayerBase::QueueMsg(int type, long long seq, const google::protobuf::Message& msg)
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

void NetLayerBase::NotifyThreadToExit()
{
	m_msg = L"Notify thread to exit...";
	m_bRunning = false;
	m_bPending = false;
}

void NetLayerBase::WaitThreadToExit(void)
{
	if (!m_hConnectThread && !m_hSendThread)
	{
		m_msg = L"No thread running...";
		return;
	}

	if (m_bRunning || m_bPending)
	{
		NotifyThreadToExit();
	}

	WaitForSingleObject(m_hConnectThread, INFINITE);
	WaitForSingleObject(m_hSendThread, INFINITE);
	m_msg = L"All thread exited.";
}


int NetLayerBase::SendMsg(SOCKET sd, const int type, long long seq, const google::protobuf::Message& msg)
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

int NetLayerBase::SendMsg(SOCKET sd, MsgBuffer* const pMsgBuf, char* const pHdrBuf /*= NULL*/)
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
int NetLayerBase::SocketProcData(SOCKET& sd, char* pBuffer, const int nDataByte, volatile bool& bRunning, bool bSend /*= true*/)
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

bool NetLayerBase::StartThread(void)
{
	WaitThreadToExit();
	m_bRunning = true;
	m_bPending = true;

	wstringstream ss;
	ss << L"Create thread failed:";
	m_hConnectThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ConnectThread, this, 0, NULL);
	if (!m_hConnectThread)
	{
		ss << L" in";
	}
	m_hSendThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SendThread, this, 0, NULL);
	if (!m_hSendThread)
	{
		ss << L" and out";
	}
	ss << L" thread.";
	if (!m_hConnectThread || !m_hSendThread)
	{
		m_msg = ss.str();
		WaitThreadToExit();
		return false;
	}
	// run thread
	m_bPending = false;
	return true;
}

unsigned NetLayerBase::ConnectThread(void* lpLayer)
{
	NetLayerBase* pLayer = (NetLayerBase*)lpLayer;
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
unsigned NetLayerBase::SendThread(void* lpLayer)
{
	NetLayerBase* pLayer = (NetLayerBase*)lpLayer;
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

	CloseHandle(pLayer->m_hSendThread);
	pLayer->m_hSendThread = NULL;
	return 0;
}

void NetLayerBase::Pending(void)
{
	while (m_bPending && m_bRunning)
	{
		Doze();
	}
}

void NetLayerBase::Doze(unsigned dwMillisec /*= NETLAYER_DOZE_DEFAULT_MS*/)
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

bool NetLayerBase::ParseMsg(google::protobuf::Message& msg, const char* pBuffer, const int nByte)
{
	if (!msg.ParseFromArray(pBuffer, nByte))
	{
		m_msg = L"ParseFromArray failed.";
		return false;
	}
	return true;
}


//} // namespace NetLayer