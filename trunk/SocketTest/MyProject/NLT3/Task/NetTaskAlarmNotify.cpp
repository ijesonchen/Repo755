#include "..\NetLayer\NetLayer.h"
#include "NetTaskAlarmNotify.h"
#include <sstream>
#include "..\protobuf\PBNetTest.pb.h"

using namespace std;
using namespace PBNetTest;
using namespace NetLayer;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NetTaskAlarmNotify::NetTaskAlarmNotify(SOCKET s, NetLayer::MsgBuffer* p)
: NetSockTask(s)
, buf(p)
{
	err.SetPrefix(L"NetTaskAlarmNotify");
}

NetTaskAlarmNotify::~NetTaskAlarmNotify()
{
	delete buf;
}

void NetTaskAlarmNotify::DoTask(void *, OVERLAPPED *)
{
	if (!buf)
	{
		return;
	}
	unsigned short uPort = 0;
	string strAddr = SocketGetName(sd, uPort);
	if (strAddr.empty() || !uPort)
	{
		err = L"getsockname error.";
		return;
	}
	CStringW wstrAddr = strAddr.c_str();

	int nRet = 0;

	nRet = SendPbMsg(sd, buf);
	if (nRet)
	{
		wstringstream ss;
		ss << L"send message error, seq " << buf->seq 
			<< " addr: " << wstrAddr.GetString() << ": " << uPort 
			<< " code: " << nRet;
		err = ss;
		return;
	}
	// if reply, recv, parse and proc
// 	MsgBuffer* pBuf = NULL;
// 	nRet = RecvPbBuf(sd, pBuf);
// 	if (nRet)
// 	{
// 		wstringstream ss;
// 		ss << L"recv message error, seq " << buf->seq 
// 			<< " " << wstrAddr.GetString() << ":" << uPort  
// 			<< ", code " << nRet;
// 		err = ss;
// 		return;
// 	}
// 
// 	if (pBuf->type != MSG_RESP)
// 	{
// 		wstringstream ss;
// 		ss << L"recv bad resp msg, type " << pBuf->type
// 			<< ", seq " << pBuf->seq;
// 		err = ss;
// 		return;
// 	}
// 	Response resp;
// 	if (!resp.ParseFromArray(pBuf->buf, pBuf->len))
// 	{
// 		wstringstream ss;
// 		ss << L"failed to parse resp msg, seq " << pBuf->seq;
// 		err = ss;
// 		return;
// 	}
}