#include "..\NetLayer\NetLayer.h"
#include "NetTaskSearchAbortReq.h"
#include "..\protobuf\PBNetTest.pb.h"
#include <sstream>

#include <iostream>
using namespace std;
using namespace PBNetTest;
using namespace NetLayer;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NetTaskSearchAbortReq::NetTaskSearchAbortReq(SOCKET s, int t, const google::protobuf::int64 n, 
											 PBNetTest::SearchAbortRequest& m)
: NetSockTask(s)
, type(t)
, seq(n)
, msg(m)
{
	err.SetPrefix(L"NetTaskSearchAbortReq");
}

void NetTaskSearchAbortReq::DoTask(void *, OVERLAPPED *)
{
	unsigned short uPort = 0;
	string strAddr = SocketGetName(sd, uPort);
	if (strAddr.empty() || !uPort)
	{
		err = L"getsockname error.";
		return;
	}
	CStringW wstrAddr = strAddr.c_str();

	string taskid = msg.aborttaskid();
	// notify search to stop taskid
	std::cout << "task id " << taskid
		<< ": notify task " << msg.aborttaskid() << " to abort." << endl;

	SearchResponse resp;
	resp.set_taskid(taskid);

	resp.set_allocated_syscode(new SystemCode());
	resp.mutable_syscode()->set_srid("srid");
	resp.mutable_syscode()->set_spyid(msg.syscode().spyid());

	resp.set_allocated_status(new OperationStatus());
	resp.mutable_status()->set_code(0);
	
	if (SendPbMsg(sd, MSG_SEARCH_RESP, seq, resp))
	{
		wstringstream ss;
		ss << L"send message error, seq: " << seq 
			<< " " << wstrAddr.GetString() << ": " << uPort << endl;
		err = ss;
	}
}