#ifndef RecogLink_h__
#define RecogLink_h__


#include "NetLink.h"
namespace PBNetTest
{
	class ConnectResponse;
}

//////////////////////////////////////////////////////////////////////////
// namespace MidLayer
namespace MidLayer
{


class RecogLink :
	public NetLayer::NetLink
{
public:
	RecogLink(SOCKET sd, const NetLayer::LinkInfo& info, NetLayer::LayerBase* pLayer, const wchar_t* pSysID);
	virtual ~RecogLink(void);

protected:
	// if time-out, disconnect
	int m_nEchoTimeOutSecs;

protected:
	// recv procedure

	// action before recv loop, use this to check recv time-out.
	// return false will cause RecvThread to exit.
	virtual bool RecvCheck(void);
	// how to deal with received message.
	// sd: connection to recv or send message
	virtual bool OnMsgRecved(google::protobuf::int64 sn, const int nType, const char* pBuffer, const int nByte) ;

	// send procedure

	// action when no data to send. 
	// e.g. queue echo msg
	virtual void OnNoDataSend(void);
	// retrieve socket to send data
	virtual SOCKET GetSendSocket();

	// for debug info
	virtual std::wstring GetMsgTypeString(int nType);

private:
	const std::wstring w_strSysID;
	bool QueueEcho(void);
	bool ProcReceivedMsg( PBNetTest::ConnectResponse &msg );
public:
	bool QueueConnectReq(void);
};


} // namespace MidLayer


#endif // RecogLink_h__
