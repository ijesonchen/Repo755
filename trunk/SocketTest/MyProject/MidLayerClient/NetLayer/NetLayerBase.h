#ifndef NetLayerBase_h__
#define NetLayerBase_h__

#include "NetLayer.h"

namespace NetLayer
{

/************************************************************************/
/*                       basic class for NetLayer                       */
/*          derive long / short connection base class from this         */
/************************************************************************/

class NetLayerBase
{
public:
	NetLayerBase(const wchar_t* pRemoteAddr , unsigned short uRemotePort, 
				const wchar_t* pSysid, void* pvThreadPool, bool bHeartBeat = false);
	~NetLayerBase(void);

	bool Init(void);

	bool QueueMsg(int type, long long seq, const google::protobuf::Message& msg);

	bool IsRunning(void);
	// notify thread to exit and return immediately. thread will exit when check flag.
	void NotifyThreadToExit();
	// wait until thread exit.
	void WaitThreadToExit(void);

protected:
	//////////////////////////////////////////////////////////////////////////
	// followed functions were overwritten by NetLayerLong / NetLayerShort
	// layer specialized init, e.g. alloc memory, start thread, etc.
	virtual bool InitLayer(void) = 0;

	//////////////////////////////////////////////////////////////////////////
	// NetLayerBase functions
protected:
	// thread pool to process tasks
	void* const m_pvThreadPool;
	// error msg
	NetLayer::MsgQueue m_msg;
	// msg for outcome
	NetLayer::BufQueue m_bufQueue;

private:
	// connection info
	std::wstring m_wstrRemoteAddr;	// addr in wstring
protected:
	std::string  m_astrRemoteAddr;	// addr in string
	unsigned short m_uRemotePort;
	std::wstring m_strSysid;

	// convert addr
	bool CheckParam(void);

	SOCKET Connect();

	// if send or check HeartBeat
	bool m_bHeartBeat;
	// if m_bHeartBeat && time-out, disconnect
	int m_nEchoTimeOutSecs;
	// time for last socket recv
	time_t m_tmLastRecv;

	int SocketRecvData(SOCKET& sd, char* pBuffer, const int nDataByte, volatile bool& bRunning)
	{
		return SocketProcData(sd, pBuffer, nDataByte, bRunning, false);
	}
	int SocketSendData(SOCKET& sd, const char* const pBuffer, const int nDataByte, volatile bool& bRunning)
	{
		return SocketProcData(sd, (char*)pBuffer, nDataByte, bRunning, true);
	}

	// send or recv fix data over socket
	int SocketProcData(SOCKET& sd, char* pBuffer, const int nDataByte, volatile bool& bRunning, bool bSend = true);

protected:
	// thread
	bool StartThread(LPTHREAD_START_ROUTINE pIn, LPTHREAD_START_ROUTINE pOut);
	void Pending(void);
	static void Doze(DWORD dwMillisec = 1);
private:
	// thread control flag
	volatile bool m_bPending;
protected:
	volatile bool m_bRunning;

	// in: recv in long conn & listen in short conn
	HANDLE m_hInThread;
	DWORD m_dwInThread;
	// out: send only in long conn & connect first in short conn
	HANDLE m_hOutThread;
	DWORD m_dwOutThread;

	// message proc
	bool ParseMsg(google::protobuf::Message& msg, const char* pBuffer, const int nByte);
};


} // namespace NetLayer

#endif // NetLayerBase_h__
