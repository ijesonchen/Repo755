#include "stdafx.h"
#include <deque>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <memory>

#include "PBMidLayer.pb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace PBMidLayer;


//////////////////////////////////////////////////////////////////////////
// socktest
const int NETLAYER_SOCKET_TIMEOUT_SEC = 100;

int SocketSendData(SOCKET& sd, const char* const pBuffer, const int nDataByte, 
				   time_t tmMax = NETLAYER_SOCKET_TIMEOUT_SEC);
int SocketRecvData(SOCKET& sd, char* pBuffer, const int nDataByte, 
				   time_t tmMax = NETLAYER_SOCKET_TIMEOUT_SEC);



//////////////////////////////////////////////////////////////////////////
// pb test

void trap(void)
{
	cout << "trapped." << endl;
	google::protobuf::ShutdownProtobufLibrary();
	exit(0);
}


void TestSerialize(void)
{
	CallProcessRequest req;
	req.set_callee("aaa");

	int n = req.ByteSize();
	tr1::shared_ptr<char> sp(new char[n]);

	string str = req.DebugString();

	try
	{
		if (!req.SerializeToArray(sp.get(), n))
		{
			string str =  req.InitializationErrorString();
			cout << "serialize failed." << endl;
		}
	}
	catch (std::exception& e)
	{
		string str = e.what();
		cout << str << endl;
		str = req.InitializationErrorString();
	}

}

void TestParse(void)
{
	bool b = false;
//	char* szName = "C:\\SVN\\DrVoiceBs\\Log\\PBMidLayer.SetRuleRequest-57027273-960274";
	char* szName = "Z:\\cap\\1";
	fstream fs;
	fs.open(szName, ios::in | ios::binary | ios::out);
	if (!fs)
	{
		trap();
	}
	fs.seekg(0, ios::end);
	streampos n = fs.tellg();
	unsigned k = n;


	cout << "len " << k << endl;
	fs.seekg(0, ios::beg);

	char* p = new char[n];
	auto_ptr<char> ap(p);
	memset(p, 0, n);
	
	fs.read(p, n);
	if (!fs)
	{
		trap();
	}
	fs.close();

	string strData(p, k);
	if (memcmp(p, strData.c_str(), strData.length()))
	{
		trap();
	}



	SetRuleRequest msg0;
	if (!msg0.ParseFromArray(p, n))
	{
		bool bInit = msg0.IsInitialized();
		string initErr = msg0.InitializationErrorString();
		stringstream wss;
		wss << "ParseFromArray failed, Init " << bInit << ". " << endl;
		if (initErr.length())
		{
			wss << (msg0.GetTypeName()) << " missing required fields: " << (initErr) << ". " << endl; 
		}
		if (msg0.ParsePartialFromArray(p, n))
		{
			wss << "ParsePartialFromArray OK: " << endl << (msg0.Utf8DebugString());
		}

		string s = wss.str();
		trap();
	}


	SetRuleRequest msg1;
	msg1.set_opertype(OT_Create);
	msg1.set_operation(RT_SpeechSegment);
	SpeechSegRule* rule = msg1.mutable_speechseg();
	rule->set_languagename("test_langname");
	rule->set_speechsegid("test_speechsegid");
	rule->set_data(strData);

	string t1data = rule->data();
	t1data = "aaa";
	t1data = rule->data();


	int n2 = msg1.ByteSize();
	char* p2 = new char[n2];
	auto_ptr<char> ap2(p2);
	memset(p2, 0, n2);
	b = msg1.SerializeToArray(p2, n2);
	if (!b)
	{
		trap();
	}

	SetRuleRequest msg2;

	if (!msg2.ParseFromArray(p2, n2))
	{
		bool bInit = msg2.IsInitialized();
		string initErr = msg2.InitializationErrorString();
		stringstream wss;
		wss << "ParseFromArray failed, Init " << bInit << ". " << endl;
		if (initErr.length())
		{
			wss << (msg2.GetTypeName()) << " missing required fields: " << (initErr) << ". " << endl; 
		}
		if (msg2.ParsePartialFromArray(p2, n2))
		{
			wss << "ParsePartialFromArray OK: " << endl << (msg2.Utf8DebugString());
		}

		string s = wss.str();
	}
	b = msg2.ParseFromArray(p2, n2);
	if (!b)
	{
		trap();
	}
	const SpeechSegRule& rule2 = msg2.speechseg();
	string data = rule2.data();
	if (k != data.length())
	{
		trap();
	}

	if (memcmp(p, data.c_str(), data.length()))
	{
		trap();
	}


	int n3 = 1000;
	char* p3 = new char[n3];
	auto_ptr<char> ap3(p3);
	for (int i = 0; i < n3; ++i)
	{
		p3[i] = i;
	}

	int size = n2;

	SetRuleRequest sr;	
	if(!sr.ParseFromArray(p2,size))
	{
		trap();
	}
	if(!sr.has_speechseg())	trap();

	SpeechSegRule speechseg = sr.speechseg();

	if(!speechseg.has_data()) 	trap();

	string raw_data = speechseg.data();
	string ret;
	ret.assign(p3, n3);

	SpeechSegRule* new_speech_seg = new SpeechSegRule();
	if(!new_speech_seg) trap();

	speechseg.clear_data();
	speechseg.set_data(ret);
	*new_speech_seg = speechseg;

	sr.clear_speechseg();
	sr.set_allocated_speechseg(new_speech_seg);

	int n4 = sr.ByteSize();
	char* p4 = new char[n4];
	auto_ptr<char> ap4(p4);


	if(!sr.SerializeToArray(p4,n4))
	{
		trap();
	}

	SetRuleRequest msg4;
	b = msg4.ParseFromArray(p4, n4);
	if (!b)
	{
		trap();
	}

	b = msg4.has_operation();
	if (!b)
	{
		trap();
	}


	;
	google::protobuf::ShutdownProtobufLibrary();
}

//////////////////////////////////////////////////////////////////////////
//
void TestRepeat(void)
{	
	SpeechSegRule srule;
	srule.set_languagename("lang");
	srule.set_speechsegid("sid");
// 	PhoneNumber* p = srule.add_telrule();
// 	p->set_telnumber("tel");
// 	p = srule.add_telrule();
// 	p->set_telnumber("tel");
// 	p = srule.add_telrule();
// 	p->set_telnumber("tel");

	cout << srule.DebugString() << endl;

	SpeechSegRule r2;
	r2.set_languagename("lang");
	r2.set_speechsegid("sid");
	cout << r2.DebugString() << endl;
	::google::protobuf::RepeatedPtrField< ::PBMidLayer::PhoneNumber >* p2 = r2.mutable_telrule();
	*p2 = srule.telrule();
	cout << r2.DebugString() << endl;

	cout << endl;
	google::protobuf::ShutdownProtobufLibrary();
}

//////////////////////////////////////////////////////////////////////////
// test perf of memory operation for proto & user class
/*
结论：建议不考虑二者性能差异，但是在开发环境中运行时，性能差2个量级以上

直接运行时（非IDE调试模式下），
release版性能差别不大3倍
debug版性能相差5倍左右
但是二者都非常快，所以非量级性能差异不明显。

dev debug PerfTestCount = 10000
proto alloc 6397
proto perf 8440
class alloc 47
class perf 47

dev release PerfTestCount = 10000
proto alloc 5912
proto perf 7784
class alloc 16
class perf 16

debug PerfTestCount = 10000
proto alloc 203
proto perf 234
class alloc 47
class perf 47

release PerfTestCount = 100000
proto alloc 359
proto perf 390
class alloc 125
class perf 125
*/

const int PerfTestCount = 100000;

void TestProtoPerf(void)
{
	DWORD dw = GetTickCount();
	CallProcessRequest req;
	req.set_callid("callidtest");
	req.set_flag(ARM_Combination);
	req.set_terminals(2);
	req.set_channeltype(ACT_Pcm);
	req.set_transflag(ATF_Trans);
	req.set_caller("callertest");
	req.set_callee("calleetest");
	req.set_third("thirdtest");
	req.set_starttime(1234);
	req.set_endtime(5678);
	req.set_recogflag(127);
	req.set_method(FAM_URL);
	PhoneNumber* p = new PhoneNumber();
	p->set_telnumber("telnumber");
	p->set_areacode("area");
	p->set_countrycode("country");
	p->set_provincecode("prov");
	req.set_allocated_callerappendix(p);
	vector<CallProcessRequest> v;
	for (int ii = 0; ii < PerfTestCount; ++ii)
	{
		v.push_back(req);
	}
	DWORD dw1 = GetTickCount() - dw;
	cout << "proto alloc " << dw1 << endl;
}

class Tel
{
public:
	string tel;
	string area;
	string prov;
	string coun;
};

class CallReq
{
public:
	string callid;
	int flag;
	int term;
	int channel;
	int trans;
	string caller;
	string called;
	string third;
	long long start;
	long long end;
	int recog;
	int method;
	Tel tel;

	CallReq() 
		: flag(0)
		, term(0)
		, channel(0)
		, trans(0)
		, start(0)
		, end(0)
		, recog(0)
		, method(0)
	{
	}
protected:
private:
};


void TestUserPerf(void)
{
	DWORD dw = GetTickCount();
	CallReq req;
	req.callid = "callidtest";
	req.flag = 1;
	req.term = 2;;
	req.channel = 3;
	req.trans = 4;
	req.caller = "callertest";
	req.called = "calleetest";
	req.third = "thirdtest";	
	req.start = 5;
	req.end = 6;
	req.recog = 7;
	req.method = 8;
	req.tel.tel = "telnumber";
	req.tel.area = "area";
	req.tel.coun = "country";
	req.tel.prov = "prov";
	vector<CallReq> v;
	for (int ii = 0; ii < PerfTestCount; ++ii)
	{
		v.push_back(req);
	}
	DWORD dw1 = GetTickCount() - dw;
	cout << "class alloc " << dw1 << endl;
}

void TestPerf(void)
{
	//
	DWORD dw = GetTickCount();
	TestProtoPerf();
	dw = GetTickCount() - dw;
	cout << "proto perf " << dw << endl;

	dw = GetTickCount();
	TestUserPerf();
	dw = GetTickCount() - dw;
	cout << "class perf " << dw << endl;


	;
	google::protobuf::ShutdownProtobufLibrary();
}