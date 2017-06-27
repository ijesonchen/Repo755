#include "TaskNetLayerAcceptMid.h"

#include "PBNetTest.pb.h"
#include "Public/PublicFunc.h"
#include "Public/ThreadPool.h"
#include "NetTaskSearchAbortReq.h"

using namespace NetLayer;
using namespace PBNetTest;
using namespace std;
using namespace PublicFunc;

#include "Public/ThreadPool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

TaskNetLayerAcceptMid::TaskNetLayerAcceptMid(SOCKET s, void* pvThreadPool)
: NetLayerAcceptTask(s, pvThreadPool)
{
}

TaskNetLayerAcceptMid::~TaskNetLayerAcceptMid(void)
{
}

bool TaskNetLayerAcceptMid::OnRecvMsg(google::protobuf::int64 sn, const int nType, const char* pBuffer, const int nByte)
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
				pool->QueueRequest(new NetTaskSearchAbortReq(sd, nType, sn, msg));
				sd = INVALID_SOCKET;
				return true;
			}
		}
		break;

	default:
		err = L"OnRecvMsg: bad msg type.";
		break;
	}

	return bRet;
}
