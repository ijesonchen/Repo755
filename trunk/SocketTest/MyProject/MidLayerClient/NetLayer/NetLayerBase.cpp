#include <afx.h>

#include "NetLayerBase.h"
#include <WinSock2.h>
#include <sstream>
#include <google/protobuf/message.h>

using namespace NetLayer;
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// NetLayerBase
//////////////////////////////////////////////////////////////////////////

namespace NetLayer
{


NetLayerBase::NetLayerBase(const wchar_t* pRemoteAddr , unsigned short uRemotePort, 
						   const wchar_t* pSysid, void* pvThreadPool, bool bHeartBeat /*= false*/)
: m_wstrRemoteAddr(pRemoteAddr)
, m_uRemotePort(uRemotePort)
, m_strSysid(pSysid)
, m_pvThreadPool(pvThreadPool)
, m_bHeartBeat(bHeartBeat)
, m_nEchoTimeOutSecs(NETLAYER_ECHO_TIMEOUT_SEC)
, m_tmLastRecv(0)
, m_bPending(false)
, m_bRunning(true)
, m_hInThread(NULL)
, m_dwInThread(0)
, m_hOutThread(NULL)
, m_dwOutThread(0)
{
}

NetLayerBase::~NetLayerBase(void)
{
	WaitThreadToExit();
}

bool NetLayerBase::Init(void)
{
	if (!CheckParam())
	{
		return false;
	}
	if (!InitLayer())
	{
		return false;
	}

	return true;
}

// convert addr
bool NetLayerBase::CheckParam(void)
{
	if (m_wstrRemoteAddr.empty() || !m_uRemotePort || m_strSysid.empty() || !m_pvThreadPool)
	{
		m_msg = L"Invalid parameter";
		return false;
	}

	CStringA str;
	str = m_wstrRemoteAddr.c_str();
	m_astrRemoteAddr = str.GetString();

	return true;
}

SOCKET NetLayerBase::Connect()
{
	SOCKET sConn = INVALID_SOCKET;

	sConn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sConn == INVALID_SOCKET) 
	{
		m_msg = L"Connect: socket function failed.";
		return INVALID_SOCKET;
	}

	struct sockaddr_in sockaddrServer; 
	sockaddrServer.sin_family = AF_INET;
	sockaddrServer.sin_addr.s_addr = inet_addr( m_astrRemoteAddr.c_str() );
	sockaddrServer.sin_port = htons( m_uRemotePort );

	if (SOCKET_ERROR == 
		connect( sConn, (SOCKADDR*) &sockaddrServer, sizeof(sockaddrServer) )) 
	{
		m_msg = L"Connect: connect function failed.";
		closesocket(sConn);
		return INVALID_SOCKET;
	}

	return sConn;
}

bool NetLayerBase::QueueMsg(int type, long long seq, const google::protobuf::Message& msg)
{
	int n = msg.ByteSize();
	char* p = new char[n];
	std::auto_ptr<char> ap(p);
	bool berr = false;
	try
	{
		if (!msg.SerializeToArray(p, n))
		{
			throw 1;
		}
	}
	catch (google::protobuf::FatalException e)
	{
		berr = true;

		CString strw;
		strw = e.what();
		wstringstream ss;
		ss << L"QueueMsg: " << strw.GetString();
		m_msg = ss;
	}
	catch (...)
	{
		berr = true;
		m_msg = L"QueueMsg: unknown exception.";
	}

	if (berr)
	{
		return false;
	}

	// memory will managed by BufQueue
	m_bufQueue.Add(type, seq, p, n);
	ap.release();
	return true;
}

void NetLayerBase::NotifyThreadToExit()
{
	m_msg = L"Notify thread to exit...";
	m_bRunning = false;
	m_bPending = false;
}

void NetLayerBase::WaitThreadToExit(void)
{
	if (!m_hInThread && !m_hOutThread)
	{
		m_msg = L"No thread running...";
		return;
	}

	if (m_bRunning || m_bPending)
	{
		NotifyThreadToExit();
	}

	WaitForSingleObject(m_hInThread, INFINITE);
	WaitForSingleObject(m_hOutThread, INFINITE);
	m_msg = L"All thread exited.";
}

bool NetLayerBase::IsRunning(void)
{
	return m_hInThread || m_hOutThread;
}

// send or recv fix data over socket
int NetLayerBase::SocketProcData(SOCKET& sd, char* pBuffer, const int nDataByte, volatile bool& bRunning, bool bSend /*= true*/)
{
	typedef int (__stdcall*pfnSocket)(SOCKET ss, char* buf, int bytes, int flags);

	const bool bRecv = !bSend;

	pfnSocket fnSocket = 0;
	if (bSend)
	{
		fnSocket = (pfnSocket)send;
	}
	else
	{
		fnSocket = recv;
	}

	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = NETLAYER_SELECT_USEC;
	fd_set fdSet;
	int nTotal = 0;
	while (nDataByte > nTotal)
	{
		if (!bRunning)
		{
			break;
		}

		// select socket
		FD_ZERO(&fdSet);
		FD_SET(sd, &fdSet);
		int nRet = 0;
		if (bSend)
		{
			nRet = select(0, 0, &fdSet, 0, &tv);
		}
		else
		{
			nRet = select(0, &fdSet, 0, 0, &tv);
		}

		// check select state
		if (nRet == SOCKET_ERROR)
		{
			return SOCKET_ERROR;
		}
		else if (nRet == 0)
		{
			// recv: no data or send: busy
			if (bRecv && m_bHeartBeat && 
				(time(NULL) - m_tmLastRecv >= m_nEchoTimeOutSecs))
			{
				// recv time-out
				return SOCKET_ERROR;
			}
			Doze();
			continue;
		}

		// send or recv data
		int nRecv = fnSocket(sd, pBuffer + nTotal, nDataByte - nTotal, 0);
		if (nRecv <= 0)
		{
			return nRecv;
		}
		nTotal += nRecv;

		// record socket last action time
		if (!bSend)
		{
			m_tmLastRecv = time(NULL);
		}
	}
	return nTotal;
}

bool NetLayerBase::StartThread(LPTHREAD_START_ROUTINE pIn, LPTHREAD_START_ROUTINE pOut)
{
	WaitThreadToExit();
	m_bRunning = true;
	m_bPending = true;

	wstringstream ss;
	ss << L"Create thread failed:";
	m_hInThread = CreateThread(0, 0, pIn, this, 0, &m_dwInThread);
	if (!m_hInThread)
	{
		ss << L" in";
	}
	m_hOutThread = CreateThread(0, 0, pOut, this, 0, &m_dwOutThread);
	if (!m_hOutThread)
	{
		ss << L" and out";
	}
	ss << L" thread.";
	if (!m_hInThread || !m_hOutThread)
	{
		m_msg = ss.str();
		WaitThreadToExit();
		return false;
	}
	// run thread
	m_bPending = false;
	return true;
}


void NetLayerBase::Pending(void)
{
	while (m_bPending && m_bRunning)
	{
		Doze();
	}
}

void NetLayerBase::Doze(DWORD dwMillisec /*= 1*/)
{
	Sleep(dwMillisec);
}

bool NetLayerBase::ParseMsg(google::protobuf::Message& msg, const char* pBuffer, const int nByte)
{
	if (!msg.ParseFromArray(pBuffer, nByte))
	{
		m_msg = L"ParseFromArray failed.";
		return false;
	}
	return true;
}


} // namespace NetLayer