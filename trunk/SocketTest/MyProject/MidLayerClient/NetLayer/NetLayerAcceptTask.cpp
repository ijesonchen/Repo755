#include "NetLayerAcceptTask.h"
#include <google/protobuf/message.h>
#include <string>
#include <sstream>

using namespace NetLayer;
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NetLayerAcceptTask::NetLayerAcceptTask(SOCKET s, void* pvThreadPool)
: PbShortTask(s)
, m_pvThreadPool(pvThreadPool)
{
	err.SetPrefix(L"AcceptTask");
}

void NetLayerAcceptTask::DoTask(void *, OVERLAPPED *)
{
	if (!m_pvThreadPool)
	{
		return;
	}
	unsigned short uPort;
	string strAddr = SocketGetName(sd, uPort);
	if (strAddr.empty() || !uPort)
	{
		err = L"getsockname error.";
		return;
	}
	CStringW wstrAddr = strAddr.c_str();


	MsgBuffer* pMsgBuffer = NULL;
	int nRet = RecvAndAllocPbBuf(sd, pMsgBuffer);
	if (nRet)
	{
		wstringstream ss;
		ss << L"RecvPbBuf failed: " <<  nRet;
		err = ss;
		return;
	}

	std::auto_ptr<MsgBuffer> ap(pMsgBuffer);

	if (!OnRecvMsg(pMsgBuffer->seq, pMsgBuffer->type, pMsgBuffer->buf, pMsgBuffer->len))
	{
		wstringstream ss;
		ss << L"proc msg failed: " <<  pMsgBuffer->type << L" " << pMsgBuffer->len << L" bytes.";
		err = ss;
		return;
	}
}

bool NetLayerAcceptTask::ParseMsg(google::protobuf::Message& msg, const char* pBuffer, const int nByte)
{
	if (!msg.ParseFromArray(pBuffer, nByte))
	{
		err = L"ParseFromArray failed.";
		return false;
	}
	return true;
}
