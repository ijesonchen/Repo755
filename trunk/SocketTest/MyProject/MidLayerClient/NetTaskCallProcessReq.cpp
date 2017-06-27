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
	std::wstring w_strGuid;	//Ψһ��ʶ

	long long	w_llSeqNo; // ��¼���кš��ӿڻػ�����ʱʹ�á�
	std::wstring w_strCallID;//������ʶ
	int w_nCombineFlag;//�����ʶ��0δ���壬1��·��2��·��
	int w_nTerminalNum;//ͨ��������1--3��
	int w_nChannalType;//�ź�����(ͨ������VOIP,���ߵ�)
	int w_nIsImport;//�Ƿ�Ϊ��������
	long long w_llStartTime;	  //��ʼʱ��
	long long w_llEndTime;	  //����ʱ��


	std::wstring w_strCaller;	  //���к���
	std::wstring w_strCallerCountryCode;//���й�����
	std::wstring w_strCallerProvinceCode;//����ʡ����
	std::wstring w_strCallerAreaCode;//���е�����

	std::wstring w_strCalled;	  //���к���
	std::wstring w_strCalledCountryCode;//���й�����
	std::wstring w_strCalledProvinceCode;//����ʡ����
	std::wstring w_strCalledAreaCode;//���е�����

	std::wstring w_strCallThird;	  //��������
	std::wstring w_strThirdCountryCode;//����������
	std::wstring w_strThirdProvinceCode;//����ʡ����
	std::wstring w_strThirdAreaCode;//����������


	std::wstring w_strSrcPath1;	  //�����ļ�·�� middle file w_strSrcSoundPath
	std::wstring w_strSrcPath2;
	std::wstring w_strSrcPath3;

	int		w_nProcessModeBits;//����λͼ
	int w_nPriority;//���ȼ���1--3��
	std::wstring w_strOperSysID;//ҵ��ϵͳ����
	int w_nAudioEncodingTag;//�����ʽ

	int		w_nSearchID;
	int     w_nState;		  //�Ƿ��ѷ���  ���ڸĳ�(�Ѵ����������̵�),�����Ѿ��ı�
	std::wstring w_strLang;        //����
	int     w_nGender;		  //�Ա�
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