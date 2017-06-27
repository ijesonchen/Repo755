#include "NetLayer\NetLayer.h"
#include "NetTaskCallProcessReq.h"
#include "PBNetTest.pb.h"
#include <sstream>

#include <iostream>
using namespace std;
using namespace NetLayer;
using namespace PBNetTest;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

struct SRecord
{
	std::wstring w_strGuid;	//唯一标识

	long long	w_llSeqNo; // 记录序列号。接口回话中临时使用。
	std::wstring w_strCallID;//话单标识
	int w_nCombineFlag;//分离标识（0未定义，1合路，2分路）
	int w_nTerminalNum;//通话个数（1--3）
	int w_nChannalType;//信号类型(通道类型VOIP,两线等)
	int w_nIsImport;//是否为导入语音
	long long w_llStartTime;	  //开始时间
	long long w_llEndTime;	  //结束时间


	std::wstring w_strCaller;	  //主叫号码
	std::wstring w_strCallerCountryCode;//主叫国家码
	std::wstring w_strCallerProvinceCode;//主叫省份码
	std::wstring w_strCallerAreaCode;//主叫地区码

	std::wstring w_strCalled;	  //被叫号码
	std::wstring w_strCalledCountryCode;//被叫国家码
	std::wstring w_strCalledProvinceCode;//被叫省份码
	std::wstring w_strCalledAreaCode;//被叫地区码

	std::wstring w_strCallThird;	  //三方号码
	std::wstring w_strThirdCountryCode;//三方国家码
	std::wstring w_strThirdProvinceCode;//三方省份码
	std::wstring w_strThirdAreaCode;//三方地区码


	std::wstring w_strSrcPath1;	  //语音文件路径 middle file w_strSrcSoundPath
	std::wstring w_strSrcPath2;
	std::wstring w_strSrcPath3;

	int		w_nProcessModeBits;//处理位图
	int w_nPriority;//优先级（1--3）
	std::wstring w_strOperSysID;//业务系统代号
	int w_nAudioEncodingTag;//编码格式

	int		w_nSearchID;
	int     w_nState;		  //是否已分配  现在改成(已处理，语音过短等),功能已经改变
	std::wstring w_strLang;        //语种
	int     w_nGender;		  //性别
};

NetTaskCallProcessReq::NetTaskCallProcessReq(const google::protobuf::int64 n, 
											 PBNetTest::CallProcessRequest& m)
: PbLongTask(n)
, msg(m)
{
	err.SetPrefix(L"NetTaskCallProcessReq");
}

void NetTaskCallProcessReq::DoTask(void *, OVERLAPPED *)
{
	// trans pb msg to db msg
	SRecord record;
	record.w_llSeqNo = seq;

	record.w_strCallID = U2W(msg.callid());
	record.w_nCombineFlag = msg.flag();
	record.w_nTerminalNum = msg.terminals();
	record.w_nChannalType = msg.channeltype();
	// msg transflag ignored
	record.w_strCaller = U2W(msg.caller());
	record.w_strCalled = U2W(msg.callee());
	record.w_strCallThird = U2W(msg.third());
	record.w_llStartTime = msg.starttime();
	record.w_llEndTime = msg.endtime();
	record.w_nProcessModeBits = msg.recogflag();
	// msg method ignored
	record.w_nPriority = msg.priority();

	PhoneNumber pn;
	pn = msg.callerappendix();
	record.w_strCallerCountryCode = U2W(pn.countrycode());
	record.w_strCallerProvinceCode = U2W(pn.provincecode());
	record.w_strCallerAreaCode = U2W(pn.areacode());

	pn = msg.calleeappendix();
	record.w_strCalledCountryCode = U2W(pn.countrycode());
	record.w_strCalledProvinceCode = U2W(pn.provincecode());
	record.w_strCalledAreaCode = U2W(pn.areacode());

	pn = msg.thirdappendix();
	record.w_strThirdProvinceCode = U2W(pn.countrycode());
	record.w_strThirdProvinceCode = U2W(pn.provincecode());
	record.w_strThirdAreaCode = U2W(pn.areacode());

	if (msg.voicefiles_size())
	{
		const FileLocationNotify& file = msg.voicefiles(0);
		record.w_nAudioEncodingTag = file.code();
	}

	for (size_t ii = 0, ni = msg.voicefiles_size(); ii < ni; ++ii)
	{
		const FileLocationNotify& fileLoc = msg.voicefiles(ii);
		if (fileLoc.callid() != msg.callid())
		{
			wcout << L"NetTaskCallProcessReq: not match: " <<	U2W(msg.callid()) << U2W(fileLoc.callid());
		}
		switch (fileLoc.filetag())
		{
		case AFT_Combination:
			record.w_strSrcPath1 = U2W(fileLoc.path());
			break;

		case AFT_Caller_ClusterA:
			record.w_strSrcPath1 = U2W(fileLoc.path());
			break;

		case AFT_Callee_ClusterB:
			record.w_strSrcPath2 = U2W(fileLoc.path());
			break;

		case AFT_Third_ClusterC:
			record.w_strSrcPath3 = U2W(fileLoc.path());
			break;
		}
	}


	SystemCode syscode = msg.syscode();
	record.w_strOperSysID = U2W(syscode.spyid());

	record.w_nIsImport = msg.isimport();

	record.w_strGuid = record.w_strOperSysID + record.w_strCallID;

	record.w_strGuid = record.w_strOperSysID + record.w_strCallID;

	cout << "CallProcessReq received: " << msg.DebugString() << endl;

}