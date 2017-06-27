#ifndef NetLayerBase_h__
#define NetLayerBase_h__

#include "NetLayer.h"

// namespace NetLayer
// {

/************************************************************************/
/*                       basic class for NetLayer                       */
/*          derive long / short connection base class from this         */
/************************************************************************/

class NetLayerBase
{
public:
	NetLayerBase(void* pvThreadPool);
	~NetLayerBase(void);

	bool Init(void);

	bool QueueMsg(int type, long long seq, const google::protobuf::Message& msg);

	// notify thread to exit and return immediately. thread will exit as soon as possible.
	void NotifyThreadToExit();
	// wait until thread exit.
	void WaitThreadToExit(void);

	//////////////////////////////////////////////////////////////////////////
	// NetLayerBase functions
protected:
	// thread pool to process tasks
	void* const m_pvThreadPool;
	// error msg
	MsgQueue m_msg;
	// msg for send
	BufQueue m_bufQueue;
public:
	MsgQueue& GetMsgRef(void)
		{ return m_msg; };


	//////////////////////////////////////////////////////////////////////////
	// threads
	//////////////////////////////////////////////////////////////////////////
protected:
	// thread control function
	bool StartThread(void);
	void Pending(void);
	void Doze(unsigned dwMillisec = NETLAYER_DOZE_DEFAULT_MS);

	// thread control flag
	volatile bool m_bPending;
	volatile bool m_bRunning;
public:
	volatile bool& GetRunFlagRef(void)
	{ return m_bRunning; };


protected:
	// thread functions
	// connect or listen thread
	HANDLE m_hConnectThread;
	static unsigned ConnectThread(void* lpLayer);
	// send thread
	HANDLE m_hSendThread;
	static unsigned SendThread(void* lpLayer);

public:
	// message proc
	bool ParseMsg(google::protobuf::Message& msg, const char* pBuffer, const int nByte);


	//////////////////////////////////////////////////////////////////////////
	// support functions
	//////////////////////////////////////////////////////////////////////////
protected:
	SOCKET Connect(const std::string& addr, const unsigned short uport);

public:
	int SendMsg(SOCKET sd, MsgBuffer* const pMsgBuf, char* const pHdrBuf = NULL);

	int SendMsg(SOCKET sd, const int type, long long seq, const google::protobuf::Message& msg);

	int SocketRecvData(SOCKET& sd, char* pBuffer, const int nDataByte, volatile bool& bRunning)
	{
		return SocketProcData(sd, pBuffer, nDataByte, bRunning, false);
	}

	int SocketSendData(SOCKET& sd, const char* const pBuffer, const int nDataByte, volatile bool& bRunning)
	{
		return SocketProcData(sd, (char*)pBuffer, nDataByte, bRunning, true);
	}

private:
	// send or recv fix data over socket
	int SocketProcData(SOCKET& sd, char* pBuffer, const int nDataByte, volatile bool& bRunning, bool bSend = true);

protected:
	//////////////////////////////////////////////////////////////////////////
	// interface
	//////////////////////////////////////////////////////////////////////////

	// followed functions should be overwrite in derived class

	// layer init

	// layer specialized init, e.g. check param, alloc memory, etc.
	virtual bool InitLayer(void) = 0;

	// connect thread

	// if new connection, return sd
	// client: first connected, return sd, otherwise, INVALID_SOCKET
	// server: listen and accepted sd
	virtual SOCKET TryConnect()
		{ return INVALID_SOCKET; };
	// procedure when new connection established.
	virtual void OnConnect(SOCKET /*connSock*/ ) 
		{ };

public:
	// recv thread

	// estimation when no data recv  
	// if true, will cause SocketRecvData fail and return SOCKET_ERROR
	// *in short connection, this can be ignored.
	virtual bool FailWhenNoRecv(void)
		{ return false; };
	// procedure when data received in SocketRecvData
	// *in short connection, this can be ignored.
	virtual void OnDataReceived(void)
		{ };
	// procedure when thread / task detect a net failure.
	// e.g. when recv bad data, call this to close socket and cause reconnection
	// *in short connection, this can be ignored.
	virtual void OnNetFailure(void)
		{ };
	// how to deal with received message.
	// sd: connection to recv or send message
	virtual bool OnRecvMsg(google::protobuf::int64 /*sn*/, const int /*nType*/, const char* /*pBuffer*/, const int /*nByte*/, SOCKET sd = INVALID_SOCKET) 
		{ return false; };

protected:
	// send thread

	// action when no data to send. e.g. queue echo msg, check time-out to close socket etc.
	virtual void OnNoDataSend(void)
		{ Doze(); };
	// retrieve socket to send data
	virtual SOCKET GetSendSocket()
		{return INVALID_SOCKET;	};
	// how to deal with message to send in bufQueue
	// !!! remember to delete pMsgBuf somewhere.
	// e.g when buf sent, or task destructor when put buf in a task.
	virtual void OnSend(SOCKET /*sd*/, MsgBuffer* const pMsgBuf ) 
		{ delete pMsgBuf; };
};


//} // namespace NetLayer

#endif // NetLayerBase_h__
