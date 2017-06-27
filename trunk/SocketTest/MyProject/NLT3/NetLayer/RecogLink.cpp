#include "RecogLink.h"
#include <sstream>
#include "..\Public\ThreadPool.h"
#include "..\Public\PublicFunc.h"
#include "..\protobuf\PBNetTest.pb.h"
#include "..\Task\NetTaskCallProcessReq.h"

using namespace std;
using namespace PBNetTest;
using namespace PublicFunc;
using namespace NetLayer;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
// namespace MidLayer
namespace MidLayer
{



RecogLink::RecogLink(SOCKET sd, const NetLayer::LinkInfo& info, NetLayer::LayerBase* pLayer, const wchar_t* pSysID)
: NetLayer::NetLink(sd, info, pLayer)
, w_strSysID(pSysID)
{
}

RecogLink::~RecogLink(void)
{
	ExitWait();
}

bool RecogLink::RecvCheck(void)
{
	const time_t tmTimeOutSecs = NETLAYER_ECHO_TIMEOUT_SEC;
	if (time(NULL) - m_tmLastRecv <= tmTimeOutSecs)
	{
		return true;
	}
	return false;
}

bool RecogLink::OnMsgRecved(google::protobuf::int64 sn, const int nType, const char*  pBuffer, const int  nByte)
{
	ThreadPoolPtr pPool = (ThreadPoolPtr) m_pLayer->ThreadPool();

	switch (nType)
	{
	case MSG_CONNECT_RESP        :
		{
			ConnectResponse msg;
			if (m_pLayer->ParseMsg(msg, pBuffer, nByte))
			{
				return ProcReceivedMsg(msg);
			}
		}
		break;

	case MSG_CONNECT_RELEASE     :
		{
			// return false to reconnect
			ConnectRelease msg;
			if (m_pLayer->ParseMsg(msg, pBuffer, nByte))
			{
				return true;
			}
		}
		break;

	case MSG_CALL_PROCESS_REQ    :
		{
			CallProcessRequest msg;
			if (m_pLayer->ParseMsg(msg, pBuffer, nByte))
			{
				NetTaskCallProcessReq* pTask = new NetTaskCallProcessReq(sn, msg);
				pPool->QueueRequest(pTask);
				return true;
			}
		}
		break;

		//////////////////////////////////////////////////////////////////////////
		// no need to proc, fall-through to return true;
	case MSG_ECHO_REQ            : // fall-through
	case MSG_ECHO_RESP           : 
		return true;
		break;

		//////////////////////////////////////////////////////////////////////////
		// bad message type or should not receive, fall through to default
	default:
		{
			wstringstream ss;
			ss << "Unknown message or should not receive: " << nType;
			m_pLayer->Msg() = ss.str();
		}
		return false;
		break;
	}

	return false;
}

void RecogLink::OnNoDataSend(void)
{
	time_t tmSendEchoSec = NETLAYER_ECHO_SEND_SEC;
	const int nWaitEcho = NETLAYER_SLEEP_WAITECHO;
	const int nWaitNoData = NETLAYER_SLEEP_SEND_NO_DATA;
	if (((time(NULL) - m_tmLastSend) >= tmSendEchoSec))
	{
		QueueEcho();
		m_pLayer->Doze(nWaitEcho);
	}
	else
	{
		m_pLayer->Doze(nWaitNoData);
	}
}

SOCKET RecogLink::GetSendSocket()
{
	return m_sdRecv;
}

// for debug info
std::wstring RecogLink::GetMsgTypeString(int nType)
{
	return Ansi2Unicode(MsgType_Name((MsgType) nType));
}

bool RecogLink::QueueEcho(void)
{
	EchoReqRes echo;
	echo.set_timestamp((google::protobuf::uint32)time(NULL));
	return m_pLayer->Msg2Queue(m_bufQueue, MSG_ECHO_REQ, 0, echo);
}

bool RecogLink::QueueConnectReq(void)
{
	ConnectRequest msgConnect;
	string srid = EncodeUtf8(w_strSysID);
	msgConnect.set_spyid("");
	msgConnect.set_srid(srid);
	return m_pLayer->Msg2Queue(m_bufQueue, MSG_CONNECT_REQ, 0, msgConnect);
}


bool RecogLink::ProcReceivedMsg( PBNetTest::ConnectResponse &msg )
{
	OperationStatus sta = msg.status();
	if (sta.code() == 0)
	{
		return true;
	}
	else
	{
		// connect auth failed
		wstring wmsg = DecodeUtf8(sta.description());
		wstringstream ss;
		ss << L"ConnectResponse failed: " << sta.code() << L" " << wmsg;
		m_pLayer->Msg() = ss;
		return false;
	}
}


} // namespace MidLayer

