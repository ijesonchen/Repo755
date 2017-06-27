///////////////////////////////////////////////////////////
//  LayerBase.h
//  Implementation of the Class LayerBase
//  Created on:      28-¾ÅÔÂ-2015 9:20:34
//  Original author: user
///////////////////////////////////////////////////////////

#if !defined(EA_C4343879_16B3_4d4a_9C1E_CE5D24D5770C__INCLUDED_)
#define EA_C4343879_16B3_4d4a_9C1E_CE5D24D5770C__INCLUDED_

#include "NetLayer.h"

namespace NetLayer
{


/**
 * Base class for net service layer 
 * isServer
 * listenPort
 * remoteAddr
 * remotePort
 *
 * !! call ExitNotify(); before destructed in derived class;
 * 
 * for long connection:
 * derive LayerBase for net layer service instance and NetLink for connection
 * context.
 * 
 * isServer
 * listenPort
 * remoteAddr
 * remotePort
 *
 *  P2P: recv: sd_accept, send: sd_connect_remote. listen port and remote addr &
 * port should be set
 *  SRV: recv: sd_accept, send: sd_recv
 *  Client: recv: sd_connect_remote, send: sd_recv
 *
 * for short connection
 * derive LayerBase for net layer service instance, use task-oriented model
 * 
 * isServer: always true
 * listenPort: required.
 * remoteAddr
 * remotePort
 * 
 * P2P: OnConnected: task_recv: sd_accepted, send: task_send
 * Srv: OnConnected: task_recv: sd_accepted
 * 
 * for short connection client:  only send: task_send: sd_connected, NOconnect
 * thread needed. 
 *
 * Threads:
 * 
 * Layer thread: 
 * 	start when Init, exit when running flag is false
 * 	when delete
 * 		exit notify & wait thread exit (base class)
 * 		clear links for long connection
 * connect thread (base class)
 * send thread (short connection layer, user code)
 * 
 * Link thread:
 * 	start when Init
 * 	exit when connection is done / linking flag is false
 * 	recv thread will remove link from Layer when exit
 * 	when delete: exit notify & wait thread exit (base class)
 * recv & send (base class)
 */

class LayerBase
{
public:
	LayerBase(void* pvThreadPool);
	virtual ~LayerBase();

	bool Init();

	void ExitNotify();
	void ExitWait();

protected:
	//////////////////////////////////////////////////////////////////////////
	// interfaces
	virtual bool InitLayer() = 0;
	virtual int OnConnected(SOCKET sd, LinkInfo& linkInfo) = 0;
	virtual void WaitLayerExit() {};

public:	// for debug info
	virtual std::wstring GetMsgTypeString(int nType);

public:
	// for long connection only
	virtual void RemoveLink(SOCKET sd)
		{ };
protected:
	//////////////////////////////////////////////////////////////////////////
	// ConnectThread
	static unsigned int ConnectThread(void* pvBase);

	// is server mode
	bool m_bServer;
	// listen parameter
	unsigned short m_uListenPort;
	// connect parameter
	std::wstring m_strRemoteAddr;
	unsigned short m_uRemotePort;

	// if connected in client, long connection mode
	bool m_bClientConnected;
	SOCKET TryConnect();

public:
	static void ClearSocket(SOCKET& sd);
	static LinkInfo GetLinkInfo(SOCKET sd);

protected:
	SOCKET Listen(unsigned short uListenPort);
	SOCKET Connect(const std::wstring&  addr, const unsigned short uport);

public:
	int SocketRecvData(SOCKET& sd, char* pBuffer, const int nDataByte)
		{ return SocketProcData(sd, pBuffer, nDataByte, false); }
	int SocketSendData(SOCKET& sd, const char* const pBuffer, const int nDataByte)
		{ return SocketProcData(sd, (char*)pBuffer, nDataByte, true); }

	// send or recv fix data over socket
	int SocketProcData(SOCKET& sd, char* pBuffer, const int nDataByte, bool bSend = true);

	int SendBuf(SOCKET sd, BufQueue& bufQueue, char* const pbufHdr);

public:
	bool Msg2Queue(BufQueue& bufQueue, int type, long long seq, const google::protobuf::Message& msg);
	bool ParseMsg(google::protobuf::Message&  msg, const char* const pBuffer, const int  nByte);

public:
	void Doze(const unsigned int uMillisecs = NETLAYER_DOZE_DEFAULT_MS);
protected:
	// thread pool to process tasks
	void* const m_pvThreadPool;
	// error msg
	MsgQueue m_msg;
	// msg for send, short connection only
	BufQueue m_bufQueue;

	HANDLE m_hConnectThread;

	bool m_bRunning;
	bool m_bPending;


public:
	bool Running(void) const
		{ return m_bRunning; }
	void Pending();
	MsgQueue& Msg()
		{ return  m_msg; }
	void* ThreadPool(void)
		{ return m_pvThreadPool; }

};


} // namespace NetLayer


#endif // !defined(EA_C4343879_16B3_4d4a_9C1E_CE5D24D5770C__INCLUDED_)
