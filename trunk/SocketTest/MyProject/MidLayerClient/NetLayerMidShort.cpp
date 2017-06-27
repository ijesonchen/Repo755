#include "NetLayerMidShort.h"

#include <sstream>
#include "Public/ThreadPool.h"
#include "PBNetTest.pb.h"
#include "TaskNetLayerAcceptMid.h"
#include "NetTaskAlarmNotify.h"

using namespace std;
using namespace NetLayer;
using namespace PBNetTest;


namespace NetLayer
{


NetLayerMidShort::NetLayerMidShort(unsigned short uLocalPort, 
						const wchar_t* pRemoteAddr , unsigned short uRemotePort, 
						const wchar_t* pSysid, void* pvThreadPool,
						const wchar_t* pLogPrefix /*= L"MidShortUntitled"*/)
: NetLayerShort(uLocalPort, pRemoteAddr, uRemotePort, pSysid, pvThreadPool)
{
	m_msg.SetPrefix(pLogPrefix);
}

NetLayerMidShort::~NetLayerMidShort(void)
{
}

void NetLayerMidShort::OnConnection( SOCKET connSock )
{
	// task for process connection
	// pLayer->m_pvThreadPool is check when Init
	ThreadPoolPtr pThreadPool = (ThreadPoolPtr) m_pvThreadPool;
	pThreadPool->QueueRequest(new TaskNetLayerAcceptMid(connSock, m_pvThreadPool));
}


void NetLayerMidShort::OnSendMsg(SOCKET sd, MsgBuffer* const pMsgBuf )
{
	// pLayer->m_pvThreadPool is valided when Init
	ThreadPoolPtr pThreadPool = (ThreadPoolPtr) m_pvThreadPool;

	// CAUSTION: remember to delete pMsgBuf in somewhere
	switch (pMsgBuf->type)
	{
	case MSG_ALARM_NOTIFY:
		{
			AlarmNotify;
			pThreadPool->QueueRequest(new NetTaskAlarmNotify(sd, pMsgBuf));
		}
		break;

	default :
		delete pMsgBuf;
		break;
	}
}

} // namespace NetLayer