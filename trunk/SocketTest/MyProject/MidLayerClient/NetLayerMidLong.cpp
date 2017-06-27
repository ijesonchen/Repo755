#include "NetLayerMidLong.h"

#include <WinSock2.h>
#include <sstream>
#include "PBNetTest.pb.h"
#include "Public/PublicFunc.h"
#include "Public/ThreadPool.h"
#include "NetTaskCallProcessReq.h"

using namespace std;
using namespace PublicFunc;
using namespace PBNetTest;

namespace NetLayer
{


NetLayerMidLong::NetLayerMidLong(const wchar_t* pRemoteAddr, unsigned short uRemotePort, 
								const wchar_t* pSysid, void* pvThreadPool, 
								bool bHeartBeat /*= true*/, 
								const wchar_t* pLogPrefix /*= L"MidLongUntitled"*/)
: NetLayerLong(pRemoteAddr, uRemotePort, pSysid, pvThreadPool, bHeartBeat)
{
	m_msg.SetPrefix(pLogPrefix);
}

NetLayerMidLong::~NetLayerMidLong(void)
{
}

bool NetLayerMidLong::PostConnectProc( SOCKET sd )
{
	// send auth msg
	ConnectRequest msgConnect;
	string srid = EncodeUtf8(m_strSysid);
	msgConnect.set_spyid("");
	msgConnect.set_srid(srid);
	if (!QueueMsg(MSG_CONNECT_REQ, 0, msgConnect))
	{
		m_msg = L"PostConnectProc: QueueMsg MSG_CONNECT_REQ failed.";
		closesocket(sd);
		return false;
	}
	return true;
}

bool NetLayerMidLong::OnRecvMsg(google::protobuf::int64 sn, const int nType, const char* pBuffer, const int nByte)
{
	bool bRet = false;

	switch (nType)
	{
	case MSG_UNKNOWN             :
		printf("unknown msg type.\n");
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
					m_msg = ss.str();
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
					m_msg = ss.str();
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
			m_msg = L"MSG_ECHO_RESP recved.";
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
		printf("bad msg type.\n");
		break;
	}

	return bRet;
}


} // NetLayer