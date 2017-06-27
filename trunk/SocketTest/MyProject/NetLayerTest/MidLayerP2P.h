#pragma once
#include "NetLayer/NetLayerBase.h"

class MidLayerP2P :
	public NetLayerBase
{
public:
	MidLayerP2P(const wchar_t* pAddr, unsigned short uPort, const wchar_t* pSysId, void* const pvThreadPool);
	~MidLayerP2P(void);

private:
	SOCKET m_socket;
	std::wstring m_addrUnicode;
	std::string m_addrAnsi;
	std::wstring m_strSysid;
	unsigned short m_uListenPort;
	unsigned short m_uRemotePort;

	//////////////////////////////////////////////////////////////////////////
	// interface realization
	//////////////////////////////////////////////////////////////////////////
private:
	virtual bool InitLayer(void);

	virtual SOCKET TryConnect(void);
	virtual void   OnConnect(SOCKET connSock);

	virtual bool OnRecvMsg(google::protobuf::int64 sn, const int nType, 
						const char* pBuffer, const int nByte, 
						SOCKET sd = INVALID_SOCKET) ;

	virtual void   OnNoDataSend(void);
	virtual SOCKET GetSendSocket(void);
	// how to deal with message to send in bufQueue
	// pMsgBuf should be delete after send.
	virtual void OnSend(SOCKET sd, MsgBuffer* const pMsgBuf );
};
