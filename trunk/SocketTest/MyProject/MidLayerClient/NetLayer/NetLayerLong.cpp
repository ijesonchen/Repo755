#include <afx.h>
#include "NetLayerLong.h"
#include <WinSock2.h>
#include <sstream>
#include "../PBNetTest.pb.h"
#include "../Public/PublicFunc.h"

using namespace std;
using namespace PublicFunc;
using namespace PBNetTest;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace NetLayer
{


NetLayerLong::NetLayerLong(const wchar_t* pRemoteAddr , unsigned short uRemotePort, 
					const wchar_t* pSysid, void* pvThreadPool, bool bHeartBeat)
: NetLayerBase(pRemoteAddr, uRemotePort, pSysid, pvThreadPool, bHeartBeat)
, m_socket(INVALID_SOCKET)
, m_bSocketReady(false)
, m_nEchoSendSecs(NETLAYER_ECHO_SEC)
, m_pBufferRecvHeader(NULL)
, m_pBufferSendHeader(NULL)
{
}

NetLayerLong::~NetLayerLong(void)
{
	WaitThreadToExit();
	CleanSocket();
	ClearBuffer();
}

// init, alloc mem & start thread
bool NetLayerLong::InitLayer(void)
{
	try
	{
		if (!m_pBufferRecvHeader)
		{
			m_pBufferRecvHeader = new char[NETLAYER_PB_HEADER_LENGTH];
		}
		if (!m_pBufferSendHeader)
		{
			m_pBufferSendHeader = new char[NETLAYER_PB_HEADER_LENGTH];
		}
	}
	catch (CException* e)
	{
		wstringstream ss;
		const int nMsgLen = 2048;
		wchar_t pzMsg[nMsgLen] = {0};
		if (e->GetErrorMessage(pzMsg, nMsgLen))
		{
			ss << "Memory alloc error: " << pzMsg << endl;
			m_msg = ss.str();
		}
		else
		{
			m_msg = L"Memory alloc error.";
		}
		return false;
	}

	if (!StartThread((LPTHREAD_START_ROUTINE)RecvThread,
					(LPTHREAD_START_ROUTINE)SendThread))
	{
		return false;
	}

	return true;
}

void NetLayerLong::ClearBuffer(void)
{
	if (m_pBufferRecvHeader)
	{
		delete m_pBufferRecvHeader;
		m_pBufferRecvHeader = NULL;
	}
	if (m_pBufferSendHeader)
	{
		delete m_pBufferSendHeader;
		m_pBufferSendHeader = NULL;
	}
}

int NetLayerLong::Reconnect(void)
{
	m_bSocketReady = false;
	// check input
	if (!m_astrRemoteAddr.length() || !m_uRemotePort)
	{
		m_msg = L"Invalid parameter or parameter not set.";
		return 1;
	}

	if (m_socket != INVALID_SOCKET)
	{
		CleanSocket();
	}

	// connect
	SOCKET sd = Connect();
	if (sd == INVALID_SOCKET)
	{
		return 2;
	}
	if (!PostConnectProc(sd))
	{
		return 3;
	}

	m_socket = sd;
	m_tmLastRecv = time(NULL);
	m_bSocketReady = true;

	return 0;
}


void NetLayerLong::CleanSocket()
{
	m_bSocketReady = false;
	if (m_socket != INVALID_SOCKET)
	{
		shutdown(m_socket, SD_BOTH);
		closesocket(m_socket);
	}
	m_socket = INVALID_SOCKET;
}

// send echo message to keep link alive
bool NetLayerLong::QueueEchoMsg(void)
{
	EchoReqRes echo;
	echo.set_timestamp((google::protobuf::uint32)time(NULL));
	return QueueMsg(MSG_ECHO_REQ, 0, echo);
}

unsigned NetLayerLong::RecvThread(void* lpLayer)
{
	NetLayerLong* pLayer = (NetLayerLong*)lpLayer;
	pLayer->Pending();
	volatile bool& bRunning = pLayer->m_bRunning;
	char*& pHeader = pLayer->m_pBufferRecvHeader;
	SOCKET& sd = pLayer->m_socket;
	MsgQueue& msg = pLayer->m_msg;
	bool bReconnect = false;

	while (bRunning)
	{
		if (bReconnect || sd == INVALID_SOCKET)
		{
			if (pLayer->Reconnect())
			{
				msg = L"RecvThread: connect failed.";
				pLayer->Doze(NETLAYER_SLEEP_RECONNECT);
				continue;
			}
		}
		bReconnect = true;
		if (pLayer->SocketRecvData(sd, pHeader, NETLAYER_PB_HEADER_LENGTH, bRunning) != NETLAYER_PB_HEADER_LENGTH)
		{
			msg = L"RecvThread: recv header failed.";
			continue;
		}
		HeadMsg header;
		if (!header.ParseFromArray(pHeader, NETLAYER_PB_HEADER_LENGTH))
		{
			msg = L"RecvThread: parse header failed.";
			continue;
		}

		// check header
		if (header.syn() != NETLAYER_PB_HEADER_SYN)
		{
			wstringstream ss;
			ss << L"RecvThread: header syn flag error: " << header.syn();
			msg = ss.str();
			continue;
		}
		const int nMsglen = header.len();
		if (nMsglen < 0 || nMsglen > NETLAYER_PB_MAX_MSG_LENGTH)
		{
			wstringstream ss;
			ss << L"RecvThread: msg length error: " << nMsglen;
			msg = ss.str();
			continue;
		}

		// message body
		char* pMsg = new char[nMsglen];
		auto_ptr<char> apMsg(pMsg);
		memset(pMsg, 0, nMsglen);
		if (pLayer->SocketRecvData(sd, pMsg, nMsglen, bRunning) != nMsglen)
		{
			wstringstream ss;
			ss << L"RecvThread recv msg failed: " << header.type() << L" " << nMsglen << L" bytes.";
			continue;
		}
		if (!pLayer->OnRecvMsg(header.seqno(), header.type(), pMsg, nMsglen))
		{
			wstringstream ss;
			ss << L"RecvThread proc msg failed: " << header.type() << L" " << nMsglen << L" bytes.";
			msg = ss.str();
			continue;
		}

		bReconnect = false;
	}

	if (sd != INVALID_SOCKET)
	{
		shutdown(sd, SD_RECEIVE);
	}

	CloseHandle(pLayer->m_hInThread);
	pLayer->m_hInThread = NULL;
	pLayer->m_dwInThread = 0;
	return 0;
}

unsigned NetLayerLong::SendThread(void* lpLayer)
{
	NetLayerLong* pLayer = (NetLayerLong*)lpLayer;
	pLayer->Pending();

	volatile bool& bRunning = pLayer->m_bRunning;
	volatile bool& bReady = pLayer->m_bSocketReady;
	SOCKET& sd = pLayer->m_socket;
	char* const & pHeader = pLayer->m_pBufferSendHeader;
	const bool& bHeartBeat = pLayer->m_bHeartBeat;
	const int nHeader = NETLAYER_PB_HEADER_LENGTH;
	MsgQueue& msg = pLayer->m_msg;

	BufQueue& bufQueue = pLayer->m_bufQueue;

	while (bRunning)
	{
		if (!bReady)
		{
			pLayer->Doze();
			continue;
		}

		MsgBuffer* const pMsgBuf = bufQueue.GetAndRelease();
		if (!pMsgBuf)
		{
			if (bHeartBeat && 
				((time(NULL) - pLayer->m_tmLastRecv) >= pLayer->m_nEchoSendSecs))
			{
				pLayer->QueueEchoMsg();
				pLayer->Doze(NETLAYER_SLEEP_WAITECHO);
			}
			else
			{
				pLayer->Doze();
			}
			continue;
		}
		std::auto_ptr<MsgBuffer> ap(pMsgBuf);

		HeadMsg header;
		header.set_syn(NETLAYER_PB_HEADER_SYN);
		header.set_pro(NETLAYER_PB_HEADER_VER);
		header.set_type(pMsgBuf->type);
		header.set_seqno(pMsgBuf->seq);
		header.set_len(pMsgBuf->len);

		if (nHeader != header.ByteSize())
		{
			msg = L"SendThread: header size error.";
			continue;
		}

		if (!header.SerializeToArray(pHeader, nHeader))
		{
			msg = L"SendThread: serialize header error.";
			continue;
		}

		if (pLayer->SocketSendData(sd, pHeader, nHeader, bRunning) != nHeader)
		{
			ap.release();
			bufQueue.ReUse(pMsgBuf);
			msg = L"SendThread: send header error.";
			continue;
		}

		if (pLayer->SocketSendData(sd, pMsgBuf->buf, pMsgBuf->len, bRunning) != pMsgBuf->len)
		{
			ap.release();
			bufQueue.ReUse(pMsgBuf);
			msg = L"SendThread: send msg error.";
			continue;
		}
	}

	if (sd != INVALID_SOCKET)
	{
		shutdown(sd, SD_SEND);
	}

	CloseHandle(pLayer->m_hOutThread);
	pLayer->m_hOutThread = NULL;
	pLayer->m_dwOutThread = 0;
	return 0;
}


} // namespace NetLayer
