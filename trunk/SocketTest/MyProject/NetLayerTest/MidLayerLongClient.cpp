#include <afx.h>
#include "MidLayerLongClient.h"
#include <sstream>
#include "protobuf/PBNetTest.pb.h"
#include "Public/PublicFunc.h"
#include "Public/ThreadPool.h"
#include "NetLayer/NetLayerRecvTask.h"
#include "Task/NetTaskCallProcessReq.h"

using namespace std;
using namespace PBNetTest;
using namespace PublicFunc;

MidLayerLongClient::MidLayerLongClient(const wchar_t* pAddr, const wchar_t* pSysId, void* pvThreadPool)
: NetLayerBase(pvThreadPool)
, m_addrUnicode(pAddr)
, m_strSysid(pSysId)
, m_uRemotePort(NETLAYER_PORT_MID_RECOG)
, m_socket(INVALID_SOCKET)
, m_tmLastRecv(0)
, m_nRecvTimeOutSecs(NETLAYER_ECHO_TIMEOUT_SEC)
, m_nEchoSendSecs(NETLAYER_ECHO_SEND_SEC)
{
	wstringstream ss;
	ss << L"MidLayerLongClient-" << pAddr;
	m_msg.SetPrefix(ss.str().c_str());
}

MidLayerLongClient::~MidLayerLongClient(void)
{
	NotifyThreadToExit();
	WaitThreadToExit();
	CleanSocket();
}

// send echo message to keep link alive
bool MidLayerLongClient::QueueEchoMsg(void)
{
	EchoReqRes echo;
	echo.set_timestamp((google::protobuf::uint32)time(NULL));
	return QueueMsg(MSG_ECHO_REQ, 0, echo);
}

// alloc send header mem
bool MidLayerLongClient::InitLayer(void)
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

	const int nHdr = NETLAYER_PB_HEADER_LENGTH;
	if (!m_cpSendHdr.size(nHdr))
	{
		return false;
	}
	return true;
}

SOCKET  MidLayerLongClient::TryConnect(void)
{
	if (m_socket != INVALID_SOCKET)
	{
		return INVALID_SOCKET;
	}

	// connect
	SOCKET sd = Connect(m_addrAnsi, m_uRemotePort);
	if (sd == INVALID_SOCKET)
	{
		wstringstream ss;
		ss << L"TryConnect: " << m_addrUnicode << ":" << m_uRemotePort << " failed.";
		m_msg = ss;
		return INVALID_SOCKET;
	}

	ConnectRequest msgConnect;
	string srid = EncodeUtf8(m_strSysid);
	msgConnect.set_spyid("");
	msgConnect.set_srid(srid);
	if (!QueueMsg(MSG_CONNECT_REQ, 0, msgConnect))
	{
		m_msg = L"TryConnect: QueueMsg connect request failed.";
		closesocket(sd);
		return INVALID_SOCKET;
	}

	m_socket = sd;
	m_tmLastRecv = time(NULL);
	return m_socket;
}

void MidLayerLongClient::CleanSocket(void)
{
	if (m_socket != INVALID_SOCKET)
	{
		shutdown(m_socket, SD_BOTH);
		closesocket(m_socket);
	}
	m_socket = INVALID_SOCKET;
}

void MidLayerLongClient::OnConnect(SOCKET connSock)
{
	NetLayerRecvTask* pTask = new NetLayerRecvTask(this, connSock, true);
	ThreadPoolPtr pool = (ThreadPoolPtr)m_pvThreadPool;
	pool->QueueRequest(pTask);
}

void MidLayerLongClient::OnNetFailure(void)
{
	CleanSocket();
}

bool MidLayerLongClient::FailWhenNoRecv(void)
{
	return time(NULL) - m_tmLastRecv >= m_nRecvTimeOutSecs;
}

bool MidLayerLongClient::OnRecvMsg(google::protobuf::int64 sn, const int nType, 
								   const char* pBuffer, const int nByte,
								   SOCKET sd /*= INVALID_SOCKET*/) 
{
	bool bRet = false;

	switch (nType)
	{
	case MSG_UNKNOWN             :
		m_msg = L"unknown msg type.";
		bRet = false;
		break;

	case MSG_CONNECT_REQ         :
		{
			// should not receive this message
			ConnectRequest msg;
			bRet = ParseMsg(msg, pBuffer, nByte);
		}
		break;

	case MSG_CONNECT_RESP        :
		{
			ConnectResponse msg;
			if (ParseMsg(msg, pBuffer, nByte))
			{
				OperationStatus sta = msg.status();
				if (sta.code() != 0)
				{
					// connect failed
					wstring wmsg = DecodeUtf8(sta.description());
					wstringstream ss;
					ss << L"ConnectResponse failed: " << sta.code() << L" " << wmsg;
					m_msg.info(ss);
				}
				else
				{
					bRet = true;
				}
			}
		}
		break;

	case MSG_CONNECT_RELEASE     :
		{
			// always return false;
			ConnectRelease msg;
			if (ParseMsg(msg, pBuffer, nByte))
			{
				OperationStatus sta = msg.status();
				if (sta.code())
				{
					wstring wmsg = DecodeUtf8(sta.description());
					wstringstream ss;
					ss << L"ConnectRelease received: " << sta.code() << L" " << wmsg;
					m_msg.info(ss);
				}
			}
			bRet = false;
		}
		break;

	case MSG_ECHO_REQ            : 
		// break; // fall-through to MSG_ECHO_RESP
	case MSG_ECHO_RESP           :
		{
			EchoReqRes msg;
			m_msg.info(L"MSG_ECHO_RESP recved.");
			bRet = ParseMsg(msg, pBuffer, nByte);
		}
		break;

	case MSG_CALL_PROCESS_REQ    :
		{
			CallProcessRequest msg;
			if (ParseMsg(msg, pBuffer, nByte))
			{
				ThreadPoolPtr pThreadPool = (ThreadPoolPtr) m_pvThreadPool;
				NetTaskCallProcessReq* pTask = new NetTaskCallProcessReq(sn, msg);
				pThreadPool->QueueRequest(pTask);
				return true;
			}
		}
		break;

	default:
		m_msg.info(L"bad msg type.");
		break;
	}

	return bRet;
}

void MidLayerLongClient::OnNoDataSend(void)
{
	const int nWaitEcho = NETLAYER_SLEEP_WAITECHO;
	const int nWaitNoData = NETLAYER_SLEEP_SEND_NO_DATA;
	if (((time(NULL) - m_tmLastRecv) >= m_nEchoSendSecs))
	{
		QueueEchoMsg();
		Doze(nWaitEcho);
	}
	else
	{
		Doze(nWaitNoData);
	}
}

// how to deal with message to send in bufQueue
// pMsgBuf should be delete after send.
void MidLayerLongClient::OnSend(SOCKET sd, MsgBuffer* const pMsgBuf )
{
	if (SendMsg(sd, pMsgBuf, m_cpSendHdr.ptr()))
	{
		m_bufQueue.ReUse(pMsgBuf);
	}
	else
	{
		delete pMsgBuf;
	}
}