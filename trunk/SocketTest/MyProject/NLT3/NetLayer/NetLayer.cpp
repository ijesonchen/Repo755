#include "NetLayer.h"
#include <WinSock2.h>
#include <memory>
#include <sstream>
#include <iostream>

#include "../protobuf/PBNetTest.pb.h"
using namespace std;
using namespace PBNetTest;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace NetLayer
{

//////////////////////////////////////////////////////////////////////////
// global variables
//////////////////////////////////////////////////////////////////////////
// global msgQueue, used to collect unchecked msg
MsgQueue g_msgQueue;

//////////////////////////////////////////////////////////////////////////
// NetLayerBase::MsgQueue
//////////////////////////////////////////////////////////////////////////

MsgQueue::MsgQueue()
: idx(-1)
, ts(NETLAYER_MSGQUEUE_TIMESTAMP)
{
	msgs.resize(NETLAYER_MSG_QUEUE_LENGTH, L"");
}

void MsgQueue::SetPrefix(const wchar_t* p)
{
	if (p)
		{ prestr = p; }
}

MsgQueue &MsgQueue::operator =(const std::wstring & str)
{
	std::wstring strmsg = SetMsg(str);

	wcout << L"  **E: " << strmsg << endl;
	return *this;
}

MsgQueue &MsgQueue::operator =(const std::wstringstream & sstr)
{
	*this = sstr.str();
	return *this;
}

void MsgQueue::info(const std::wstring & str)
{
	std::wstring strmsg = SetMsg(str);
	wcout << "INFO: " << strmsg << endl;
}

void MsgQueue::info(const std::wstringstream & sstr)
{
	info(sstr.str());
}

std::wstring MsgQueue::SetMsg( const std::wstring & str )
{
	long n = InterlockedIncrement(&idx);
	if (n >= NETLAYER_MSG_QUEUE_LENGTH || n < 0)
	{
		InterlockedExchange(&idx, 0);
	}
	std::wstring strmsg;
	if (!prestr.empty())
	{
		strmsg = prestr + L": " + str;
	}
	else
	{
		strmsg = str;
	}
	if (ts)
	{
		std::wstring strtime = CTime::GetCurrentTime().Format(_T("%Y%m%d %H:%M:%S ")).GetString();
		strmsg = strtime + strmsg;
	}
	msgs[idx] = strmsg;
	return strmsg;
}


//////////////////////////////////////////////////////////////////////////
// CharBuffer
//////////////////////////////////////////////////////////////////////////
CharBuffer::CharBuffer()
: p(NULL)
, len(0)
{
}

CharBuffer::~CharBuffer()
{
	delete p;
}

bool CharBuffer::size(int n)
{
	// threshold for double size memory
	const int nTh = NETLAYER_PB_MAX_MSG_LENGTH;
	if (n <= 0)
	{
		return false;
	}
	if (n > len)
	{
		delete p;
		// double length
		int nd = len * 2;
		// new length
		int nn = n;
		if ((n < nTh) && (nd < nTh))
		{
			nn = n > nd ? n : nd;
		}
		else
		{
			nn = n;
		}

		p = new char[nn];
		len = nn;
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////
// NetLayerBase::MsgBuffer
//////////////////////////////////////////////////////////////////////////

MsgBuffer::MsgBuffer(int t, long long l, char* p, int n)
: type(t)
, seq(l)
, buf(p)
, len(n)
{
}

MsgBuffer::~MsgBuffer()
{
	delete []buf;
}

//////////////////////////////////////////////////////////////////////////
// NetLayerBase::BufQueue
//////////////////////////////////////////////////////////////////////////

BufQueue::~BufQueue()
{
	CSingleLock lock(&csBuf, true);
	for (size_t ii = 0, ni = dqBufs.size();
		ii < ni; ++ii)
	{
		delete dqBufs[ii];
	}
	dqBufs.clear();
}

// !!! do not free p after Push, p will freed when delete MsgBuffer*
// if too many bufs, oldest bufs will removed when Add.
void BufQueue::Push(int type, long long seq, char* p, int n)
{
	CSingleLock lock(&csBuf, true);
	// push & ReUse, so sz should minus 2
	int sz = NETLAYER_BUF_QUEUE_MAXSIZE - 2;
	if (sz < 0)
	{
		sz = 0;
	}
	if (dqBufs.size() > (unsigned)sz)
	{
		// too many bufs, remove oldest
		std::deque<MsgBuffer*>::iterator const fst = dqBufs.begin();
		std::deque<MsgBuffer*>::iterator const lst = fst + (dqBufs.size() - sz);
		for (std::deque<MsgBuffer*>::iterator it = fst; it != lst; ++it)
		{
			delete *it;
		}
		dqBufs.erase(fst, lst);
	}
	dqBufs.push_back(new MsgBuffer(type, seq, p, n));
}

// get first buf and remove from dq
MsgBuffer* BufQueue::ReleasePop(void)
{
	CSingleLock lock(&csBuf, true);
	if (dqBufs.size())
	{
		MsgBuffer* p = dqBufs.front();
		dqBufs.pop_front();
		return p;
	}
	return NULL;
}

// put (previous got) a buf to dq
void BufQueue::Capture(MsgBuffer* const p)
{
	CSingleLock lock(&csBuf, true);
	dqBufs.push_front(p);
}

bool BufQueue::HasData(void)
{
	CSingleLock lock(&csBuf, true);
	return dqBufs.size() ? true : false;
}

//////////////////////////////////////////////////////////////////////////
// LinkInfo
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// NetTask
//////////////////////////////////////////////////////////////////////////

NetLinkTask::NetLinkTask(const google::protobuf::int64 seqno)
: seq(seqno)
{
}


NetSockTask::NetSockTask(SOCKET s)
: sd(s) 
{
}

NetSockTask::~NetSockTask()
{
	if (sd != INVALID_SOCKET)
	{
		shutdown(sd, SD_BOTH);
		closesocket(sd);
	}
}


//////////////////////////////////////////////////////////////////////////
// socket functions
//////////////////////////////////////////////////////////////////////////

int SocketProcData(SOCKET& sd, char* pBuffer, const int nDataByte, bool bSend /*= true*/, time_t tmMax /*= NETLAYER_SOCKET_TIMEOUT*/)
{
	typedef int (__stdcall*pfnSocket)(SOCKET ss, char* buf, int bytes, int flags);

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
	time_t tmBeg = time(NULL);
	while (nDataByte > nTotal)
	{
		if (time(NULL) - tmBeg >= tmMax)
		{
			if (NETLAYER_SHOW_DETAIL_MSG)
			{
				g_msgQueue = L"SocketProcData timed-out";
			}
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
			if (NETLAYER_SHOW_DETAIL_MSG)
			{
				g_msgQueue = L"SocketProcData select SOCKET_ERROR";
			}
			return SOCKET_ERROR;
		}
		else if (nRet == 0)
		{
			// recv: no data or send: busy
			Sleep(1);
			continue;
		}

		// send or recv data
		int nRecv = fnSocket(sd, pBuffer + nTotal, nDataByte - nTotal, 0);
		if (nRecv <= 0)
		{
			if (NETLAYER_SHOW_DETAIL_MSG)
			{
				if (nRecv < 0)
				{
					g_msgQueue = L"SocketProcData fnSocket error";
				}
				else
				{
					g_msgQueue = L"SocketProcData socket closed.";
				}
			}
			return nRecv;
		}
		nTotal += nRecv;
	}
	if (NETLAYER_SHOW_DETAIL_MSG)
	{
		wstringstream ss;
		ss << L"SocketProcData proced " << nTotal << L" bytes.";
		g_msgQueue = ss;
	}
	return nTotal;
}

int SocketSendData(SOCKET& sd, const char* const pBuffer, const int nDataByte, 
				   time_t tmMax /*= NETLAYER_SOCKET_TIMEOUT*/)
{
	return SocketProcData(sd, (char*)pBuffer, nDataByte, true, tmMax);
}

int SocketRecvData(SOCKET& sd, char* pBuffer, const int nDataByte, 
				   time_t tmMax /*= NETLAYER_SOCKET_TIMEOUT*/)
{
	return SocketProcData(sd, pBuffer, nDataByte, false, tmMax);
}

std::string SocketGetName(SOCKET sd, unsigned short& uPort)
{
	sockaddr_in connAddr;
	int nAddr = sizeof(sockaddr_in);
	std::string strAddr;
	if (SOCKET_ERROR == getsockname(sd, (sockaddr*)&connAddr, &nAddr))
	{
		return strAddr;
	}
	uPort = ntohs(connAddr.sin_port);
	strAddr = inet_ntoa(connAddr.sin_addr);
	return strAddr;
}
  
int SendPbMsg(SOCKET sd, const int type, long long seq, const google::protobuf::Message& msg)
{
	const int len = msg.ByteSize();
	char* const buf = new char[len];
	std::auto_ptr<char> apb(buf);
	try
	{
		if (NETLAYER_SHOW_DETAIL_MSG)
		{
			wstringstream ss;
			ss << L"Msg is about to Serialize: " << Ansi2Unicode(msg.GetTypeName()) << L" " << endl
				<< PublicFunc::DecodeUtf8(msg.Utf8DebugString());
			g_msgQueue = ss;
		}

		if (!msg.SerializeToArray(buf, len))
		{
			// failed
			throw 1;
		}
	}
	catch (google::protobuf::exception e)
	{
		e.what();
	}
	MsgBuffer* pMsgBuf = new MsgBuffer(type, seq, buf, len);
	apb.release();
	std::auto_ptr<MsgBuffer> apm(pMsgBuf);

	return SendPbMsg(sd, pMsgBuf);
}


int SendPbMsg(SOCKET sd, MsgBuffer* const pMsgBuf)
{
	const int nHeader = NETLAYER_PB_HEADER_LENGTH;
	char* const pHeader = new char[nHeader];
	std::auto_ptr<char> aph(pHeader);

	HeadMsg header;
	header.set_syn(NETLAYER_PB_HEADER_SYN);
	header.set_pro(NETLAYER_PB_HEADER_VER);
	header.set_type(pMsgBuf->type);
	header.set_seqno(pMsgBuf->seq);
	header.set_len(pMsgBuf->len);

	if (nHeader != header.ByteSize())
	{
		return 2;
	}

	if (NETLAYER_SHOW_DETAIL_MSG)
	{
		wstringstream ss;
		ss << L"HeadMsg is about to Serialize: " << PublicFunc::DecodeUtf8(header.Utf8DebugString());
		g_msgQueue = ss;
	}

	if (!header.SerializeToArray(pHeader, nHeader))
	{
		return 3;
	}

	if (SocketSendData(sd, pHeader, nHeader) != nHeader)
	{
		return 4;
	}

	if (SocketSendData(sd, pMsgBuf->buf, pMsgBuf->len) != pMsgBuf->len)
	{
		return 5;
	}

	stringstream ss;
	ss << "    ***pb msg sent t " << pMsgBuf->type << ", s " << pMsgBuf->seq << ", l" << pMsgBuf->len << endl;
	cout << ss.str();

	return 0;
}


int RecvAndAllocPbBuf(SOCKET sd, MsgBuffer*& pMsgBuffer)
{
	if (pMsgBuffer)
	{
		return 1;
	}

	const int nHdr = NETLAYER_PB_HEADER_LENGTH;
	char* pHdr = new char[nHdr];
	std::auto_ptr<char> aph(pHdr);

	if (nHdr != SocketRecvData(sd, pHdr, nHdr))
	{
		return 2;
	}

	HeadMsg header;
	if (!header.ParseFromArray(pHdr, nHdr))
	{
		return 3;
	}

	// check header
	if (header.syn() != NETLAYER_PB_HEADER_SYN)
	{
		return 4;
	}

	const int nMsglen = header.len();
	if (nMsglen < 0 || nMsglen > NETLAYER_PB_MAX_MSG_LENGTH)
	{
		return 5;
	}

	// message body
	char* pMsg = new char[nMsglen];
	std::auto_ptr<char> apMsg(pMsg);
	memset(pMsg, 0, nMsglen);
	if (SocketRecvData(sd, pMsg, nMsglen) != nMsglen)
	{
		return 6;
	}
	apMsg.release();
	pMsgBuffer = new MsgBuffer(header.type(), header.seqno(), pMsg, nMsglen);
	return 0;
}


std::string Unicode2Ansi(const wchar_t* pUnicode)
{
	CStringA str;
	str = pUnicode;
	return str.GetString();
}

std::string Unicode2Ansi(std::wstring strUnicode)
{
	return Unicode2Ansi(strUnicode.c_str());
}

std::wstring Ansi2Unicode(const char* pAnsi)
{
	CString str;
	str = pAnsi;
	return str.GetString();
}

std::wstring Ansi2Unicode(std::string strAnsi)
{
	return Ansi2Unicode(strAnsi.c_str());
}

} // namespace NetLayer