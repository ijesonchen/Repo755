///////////////////////////////////////////////////////////
//  NetLink.h
//  Implementation of the Class NetLink
//  Created on:      28-¾ÅÔÂ-2015 9:20:34
//  Original author: user
///////////////////////////////////////////////////////////

#if !defined(EA_EACD42FA_47B7_4a45_A9A8_9155EDCF4E5A__INCLUDED_)
#define EA_EACD42FA_47B7_4a45_A9A8_9155EDCF4E5A__INCLUDED_

#include "NetLayer.h"
#include "LayerBase.h"

namespace NetLayer
{


/**
 * connection context for
 * long , single / multi connection.
 * 
 * !! call ExitNotify(); before destructed in derived class;
 * 
 * derive NetLong for net layer service instance and NetLink for connection
 * context.
 */


class LinkMap;
class NetLink
{
	friend LinkMap;
public:
	NetLink(SOCKET sd, const LinkInfo& info, LayerBase* pLayer);
	virtual ~NetLink();

	bool Init();
	bool QueueMsg(int type, long long seq, const google::protobuf::Message& msg);
	void ExitNotify();
	void ExitWait();

private:
	void RecvPending(void);

protected:
	SOCKET m_sdRecv;
	SOCKET m_sdSend;

	HANDLE m_hRecvThread;
	static unsigned int RecvThread(NetLink* pvLink);
	HANDLE m_hSendThread;
	static unsigned int SendThread(NetLink* pvLink);


protected:
	LinkInfo m_linkInfo;
	LayerBase* const m_pLayer;
	volatile bool m_bLinking;
	volatile bool m_bRecving;
	BufQueue m_bufQueue;

	// time for last socket recv
	time_t m_tmLastRecv;
	time_t m_tmLastSend;

	//////////////////////////////////////////////////////////////////////////
	// interfaces
protected:
	// for recv procedure

	// action before recv loop, use this to check recv time-out.
	// return false will cause RecvThread to exit.
	virtual bool RecvCheck(void)
	{ return true; };
	// how to deal with received message.
	// sd: connection to recv or send message
	virtual bool OnMsgRecved(google::protobuf::int64 /*sn*/, const int /*nType*/, const char* /*pBuffer*/, const int /*nByte*/) 
	{ return false; };

protected:
	// send procedure

	// action when no data to send. 
	// e.g. queue echo msg
	virtual void OnNoDataSend(void)
	{ m_pLayer->Doze(NETLAYER_SLEEP_SEND_NO_DATA); };
	// retrieve socket to send data
	virtual SOCKET GetSendSocket()
	{ return INVALID_SOCKET;	};

	// for debug info
	virtual std::wstring GetMsgTypeString(int nType);
};


} // namespace NetLayer


#endif // !defined(EA_EACD42FA_47B7_4a45_A9A8_9155EDCF4E5A__INCLUDED_)
