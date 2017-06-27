///////////////////////////////////////////////////////////
//  P2PLayer.h
//  Implementation of the Class P2PLayer
//  Created on:      29-¾ÅÔÂ-2015 16:01:24
//  Original author: user
///////////////////////////////////////////////////////////

#if !defined(EA_7029FE9F_C4EF_4bc1_88CD_A53E8BDD2EC4__INCLUDED_)
#define EA_7029FE9F_C4EF_4bc1_88CD_A53E8BDD2EC4__INCLUDED_

#include "LayerBase.h"


//////////////////////////////////////////////////////////////////////////
// namespace MidLayer
namespace MidLayer
{


/**
 * for short, p2p connection
 * use task-oriented model
 * 
 * isServer: always true
 * listenPort: required
 * remoteAddr: required
 * remotePort: required, same as listenPort
 * 
 * OnConnected: task_recv
 * SendThread: task_send
 * task_recv: sd_accepted
 * task_send: sd_connected
 * 
 * remoteAddr connect_mode task-oriented model  P2P: OnConnected: task_recv:
 * connect_mode
 * task-oriented model
 */
class P2PLayer : public NetLayer::LayerBase
{

public:
	P2PLayer(const wchar_t* pSysID, const wchar_t* prAddr, unsigned short uPort, 
			void* pvThreadPool, const wchar_t* pMsgPrefix = L"P2PLayerUnamed");
	virtual ~P2PLayer();



	virtual bool InitLayer();
	virtual int OnConnected(SOCKET sd, NetLayer::LinkInfo& linkInfo);
	virtual void WaitLayerExit();
	bool QueueMsg2AllLink(int type, long long seq, const google::protobuf::Message& msg);
private:
	bool OnShortMsgRecved(SOCKET sd, google::protobuf::int64 sn, const int nType, const char* pBuffer, const int nByte) ;
	static unsigned int SendThread(P2PLayer* pvNetP2P);
	void SendMsg(SOCKET sd);
	const std::wstring m_strSydID;
	HANDLE m_hSendThread;
	NetLayer::BufQueue m_bufQueue;
	char* m_pSendBufHdr;

	// for debug info
	virtual std::wstring GetMsgTypeString(int nType);
};


} // namespace MidLayer


#endif // !defined(EA_7029FE9F_C4EF_4bc1_88CD_A53E8BDD2EC4__INCLUDED_)
