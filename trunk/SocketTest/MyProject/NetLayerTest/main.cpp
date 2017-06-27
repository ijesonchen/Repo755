#include <afx.h>
#include "Public/ThreadPool.h"

#include "protobuf/PBNetTest.pb.h"
#include <deque>
#include <string>
#include <sstream>
#include <iostream>
#include "Public/PublicFunc.h"
#include "MidLayerLongClient.h"

using namespace std;
using namespace PBNetTest;
using namespace PublicFunc;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "ws2_32.lib")

#ifdef _DEBUG
#pragma comment(lib, "libprotobuf_Debug_Win32.lib")
#else
#pragma comment(lib, "libprotobuf_Release_Win32.lib")
#endif // _DEBUG

void Trap(void)
{
	cout << "trapped." << endl;
}

int main(void)
{	
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	int nRet =  0;

	ThreadPool<ThreadWorker> thrdPool;
	nRet = thrdPool.Initialize();
	if (FAILED(nRet))
	{
		printf("pool init failed.\n");
	}


	CString strRemoteIP = L"192.168.1.112";
	CString strLongIP	= L"192.168.1.112";
	wchar_t* pwid = L"ulangsr";


	char p[10] = {1, 0, 2, 0, 3, 0, 0};
	string strss;
	strss.assign(p, 5);
	int nnn = strss.length();
	nnn = strss.size();

	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2,2), &wsaData) != NO_ERROR)
	{
		return 1;
	}
	string utf8;

	MidLayerLongClient* plLayer = new MidLayerLongClient(strLongIP, pwid, &thrdPool);
	if (!plLayer->Init())
	{
		Trap();
	}








	// echo
	EchoReqRes echo;
	echo.set_timestamp((unsigned)time(0));
	plLayer->QueueMsg(MSG_ECHO_REQ, 1, echo);
	echo.set_timestamp((unsigned)time(0));
	plLayer->QueueMsg(MSG_ECHO_RESP, 1, echo);

	ConnectResponse connresp;

	OperationStatus* ps = new OperationStatus();
	ps->set_code(0);
	utf8 = EncodeUtf8(L"系统连接正常");
	ps->set_description(utf8);

	connresp.set_allocated_status(ps);
	utf8 = EncodeUtf8(L"业务系统ID");
	connresp.set_spyid(utf8);
	utf8 = EncodeUtf8(L"识别系统ID");
	connresp.set_srid(utf8);
	plLayer->QueueMsg(MSG_CONNECT_RESP, 1, connresp);

	ps = new OperationStatus();
	ps->set_code(1);
	utf8 = EncodeUtf8(L"系统连接失败");
	ps->set_description(utf8);
	connresp.set_allocated_status(ps);
	plLayer->QueueMsg(MSG_CONNECT_RESP, 1, connresp);
 
// 	NetLayerMidShort* psLayer7013 = new NetLayerMidShort(7013, strRemoteIP, 7013, L"ulangsr", &thrdPool, strRemoteIP + L"-Short7013");
// 	if (!psLayer7013->Init())
// 	{
// 		return 3;
// 	}
// 
// 
// 	AlarmNotify alarmNotify;
// 	alarmNotify.set_alarmtime("alarmtime");
// 	alarmNotify.set_level(AL_General);
// 	alarmNotify.set_content("content");
// 	alarmNotify.set_modulename("modulename");
// 
// 	SystemCode* psystemCode = new SystemCode();
// 	psystemCode->set_spyid("spyid");
// 	psystemCode->set_srid("srid");
// 	alarmNotify.set_allocated_syscode(psystemCode);
// 
// 
// 	psLayer7013->QueueMsg(MSG_ALARM_NOTIFY, 999, alarmNotify);
// 
// 
// 	NetLayerMidShort* psLayer7014 = new NetLayerMidShort(7014, strRemoteIP, 7014, L"ulangsr", &thrdPool, strRemoteIP + L"-Short7013");
// 	if (!psLayer7014->Init())
// 	{
// 		return 4;
// 	}
// 
	bool bLoop = true;
	while (bLoop)
	{
		Sleep(1);
	}

 	delete plLayer;
// 	delete psLayer7013;
// 	delete psLayer7014;


	WSACleanup();
	thrdPool.Shutdown();
	google::protobuf::ShutdownProtobufLibrary();



	return 0;
}