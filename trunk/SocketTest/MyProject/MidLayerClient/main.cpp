#include <afx.h>

#include "Public/ThreadPool.h"
#include "NetLayerMidLong.h"
#include "NetLayerMidShort.h"

#include "PBNetTest.pb.h"
#include <deque>
#include <string>
#include <sstream>
#include <iostream>
#include "Public/PublicFunc.h"

using namespace std;

using namespace PBNetTest;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace PublicFunc;


#pragma comment(lib, "ws2_32.lib")

#ifdef _DEBUG
#pragma comment(lib, "libprotobuf_Debug_Win32.lib")
#else
#pragma comment(lib, "libprotobuf_Release_Win32.lib")
#endif // _DEBUG

class CT
{
public:
	CT(int )
	{
		cout << "construct" << endl;
	}
	CT(const CT& )
	{
		cout << "copy" << endl;
	}

	CT& operator=(const CT& )
	{
		cout << "operator=" << endl;
	
		return *this;
	}
};

void Test(void)
{
	google::protobuf::ShutdownProtobufLibrary();

	deque<CT> dqct;
	dqct.push_back(CT(0));
	

	wstring ws1 = L"测试文字";
	string s1 = EncodeUtf8(ws1);
	string s2 = MSEncodeUtf8(ws1);

	wstring w2 = DecodeUtf8(s1);
	wstring w3 = MSDecodeUtf8(s2);

	exit(0);
}

using namespace NetLayer;
void TestThread(void)
{
	MsgQueue msgQueue;
	wstringstream ss;
	ss << L"test";

	msgQueue = ss;

	google::protobuf::ShutdownProtobufLibrary();
}

int main(void)
{	
	TestThread();
	return 0;
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	CString strRemoteIP = L"192.168.1.112";
	CString strLongIP	= L"192.168.1.112";

	int nRet = 0;

	ThreadPool<ThreadWorker> thrdPool;
	nRet = thrdPool.Initialize();
	if (FAILED(nRet))
	{
		printf("pool init failed.\n");
	}

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

	NetLayerMidLong* plLayer = new NetLayerMidLong(strLongIP, 7011, L"ulangsr", &thrdPool, true, strLongIP + L"-MidLong7011");
	if (!plLayer->Init())
	{
		return 2;
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

	NetLayerMidShort* psLayer7013 = new NetLayerMidShort(7013, strRemoteIP, 7013, L"ulangsr", &thrdPool, strRemoteIP + L"-Short7013");
	if (!psLayer7013->Init())
	{
		return 3;
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


	psLayer7013->QueueMsg(MSG_ALARM_NOTIFY, 999, alarmNotify);


	NetLayerMidShort* psLayer7014 = new NetLayerMidShort(7014, strRemoteIP, 7014, L"ulangsr", &thrdPool, strRemoteIP + L"-Short7013");
	if (!psLayer7014->Init())
	{
		return 4;
	}

	bool bLoop = true;
	while (bLoop)
	{
		Sleep(1);
	}

	delete plLayer;
	delete psLayer7013;
	delete psLayer7014;

	WSACleanup();
	thrdPool.Shutdown();
	google::protobuf::ShutdownProtobufLibrary();



	return 0;
}