#ifndef MidLayerLongClient_h__
#define MidLayerLongClient_h__

#include "NetLayer/NetBase.h"

class NetLongClient :
	public NetBase
{
public:
	NetLongClient(const wchar_t* pAddr, const wchar_t* pSysId, void* const pvThreadPool);
	~NetLongClient(void);

private:
	SOCKET m_socket;
	std::wstring m_addrUnicode;
	std::string m_addrAnsi;
	std::wstring m_strSysid;
	unsigned short m_uRemotePort;

	CharBuffer m_cpSendHdr;
	char* m_pSendBufHdr;
	time_t m_tmLastRecv;

	// if time-out, fail recv
	int m_nRecvTimeOutSecs;
	// if time-out, queue echo msg
	int m_nEchoSendSecs;
	// queue echo message to keep link alive
	bool QueueEchoMsg(void);

	virtual void CleanSocket(void);
	//////////////////////////////////////////////////////////////////////////
	// Interface realization
protected:
	// alloc send header mem
	virtual bool InitLayer(void);

	virtual SOCKET TryConnect(void);
	virtual void OnConnect(SOCKET connSock);

	virtual void OnNetFailure(void);
	virtual bool FailWhenNoRecv(void);
	virtual void OnDataReceived(void)
		{ m_tmLastRecv = time(NULL); };
	virtual bool OnRecvMsg(google::protobuf::int64 sn, const int nType, 
						const char* pBuffer, const int nByte, 
						SOCKET sd = INVALID_SOCKET) ;

	virtual void OnNoDataSend(void);
	virtual SOCKET GetSendSocket(void)
		{ return m_socket; };
	// how to deal with message to send in bufQueue
	// pMsgBuf should be delete after send.
	virtual void OnSend(SOCKET sd, MsgBuffer* const pMsgBuf );
};


#endif // MidLayerLongClient_h__
