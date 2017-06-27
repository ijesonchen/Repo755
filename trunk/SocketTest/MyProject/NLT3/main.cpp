#include <afx.h>
#include "Public/ThreadPool.h"

#include "protobuf/PBNetTest.pb.h"
#include <deque>
#include <string>
#include <sstream>
#include <iostream>
#include "Public/PublicFunc.h"
#include "NetLayer/RecogLayer.h"
#include "NetLayer/P2PLayer.h"
#include "NetLayer/KeywordLayer.h"

using namespace std;
using namespace PBNetTest;
using namespace PublicFunc;
using namespace NetLayer;
using namespace MidLayer;
using namespace KwdLayer;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "ws2_32.lib")

#ifdef _DEBUG
#pragma comment(lib, "libprotobuf_Debug_Win32.lib")
#else
#pragma comment(lib, "libprotobuf_Release_Win32.lib")
#endif // _DEBUG


using namespace std;

void trap(void)
{
	cout << "trapped." << endl;
	exit(0);
}

void Test(void)
{
	char* pp = 0;
	std::auto_ptr<char> ap;
	pp = ap.get();

	
	char* p = new char[10];
	ap.release();
	pp = ap.get();
	ap.reset(p);
	pp = ap.get();



}

void main(void)
{
	cout << "enter main" << endl;

	int nRet = 0;
	string utf8;

	ThreadPool<ThreadWorker> thrdPool;
	nRet = thrdPool.Initialize();
	if (FAILED(nRet))
	{
		printf("pool init failed.\n");
		trap();
	}

	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2,2), &wsaData) != NO_ERROR)
	{
		trap();
	}

	CallProcessRequest req;
	char* pReq = new char[req.ByteSize()];
	if (!req.SerializeToArray(pReq, req.ByteSize()))
	{
		cout << "err" << endl;
	}


	const wchar_t* prAddr = L"192.168.1.107";
	wchar_t* pwid = L"srtest";

	cout << "RecogLayer" << endl;
	RecogLayer* pRecog = new RecogLayer(pwid, prAddr, &thrdPool);
	if (!pRecog->Init())
	{
		trap();
	}


	// echo
	EchoReqRes echo;
	echo.set_timestamp((unsigned)time(0));
	pRecog->QueueMsg(MSG_ECHO_REQ, 1, echo);
	echo.set_timestamp((unsigned)time(0));
	pRecog->QueueMsg(MSG_ECHO_RESP, 1, echo);

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
	pRecog->QueueMsg(MSG_CONNECT_RESP, 1, connresp);

	ps = new OperationStatus();
	ps->set_code(1);
	utf8 = EncodeUtf8(L"系统连接失败");
	ps->set_description(utf8);
	connresp.set_allocated_status(ps);
	pRecog->QueueMsg(MSG_CONNECT_RESP, 1, connresp);

	cout << "psLayer7013" << endl;
	P2PLayer* psLayer7013 = new P2PLayer(pwid, prAddr, 7013, &thrdPool, L"7013");
	if (!psLayer7013->Init())
	{
		trap();
	}


	AlarmNotify alarmNotify;
	alarmNotify.set_alarmtime("alarmtime");
	alarmNotify.set_level(AL_General);
	alarmNotify.set_content("content");
	alarmNotify.set_modulename("modulename");

	SystemCode* psystemCode = new SystemCode();
	psystemCode->set_spyid("spyid");
	psystemCode->set_srid("srid");
	alarmNotify.set_allocated_syscode(psystemCode);


	psLayer7013->QueueMsg2AllLink(MSG_ALARM_NOTIFY, 999, alarmNotify);


	cout << "psLayer7014" << endl;
	P2PLayer* psLayer7014 = new P2PLayer(pwid, prAddr, 7014, &thrdPool, L"7014");
	if (!psLayer7014->Init())
	{
		trap();
	}



	bool bRunning = true;
	while (getchar() == 'q' || getchar() == EOF)
	{
		break;
	}

	delete pRecog;
	delete psLayer7013;
	delete psLayer7014;

	cout << "end" << endl;

	WSACleanup();
	thrdPool.Shutdown();
	google::protobuf::ShutdownProtobufLibrary();
}