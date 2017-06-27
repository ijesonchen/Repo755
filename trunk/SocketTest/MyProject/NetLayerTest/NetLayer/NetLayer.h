#ifndef NetLayer_h__
#define NetLayer_h__

#include <string>
#include <deque>
#include <afxmt.h>
#include "../Public/ThreadTask.h"
#include "../Public/PublicFunc.h"

// declarations
namespace google
{
	namespace protobuf
	{
		class Message;
		typedef __int64 int64;
	}
}

#ifndef SOCKET
typedef UINT_PTR SOCKET;
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET  (SOCKET)(~0)
#endif

// namespace NetLayer
// {


//////////////////////////////////////////////////////////////////////////
// constant
//////////////////////////////////////////////////////////////////////////
// PB net layer port
const unsigned short NETLAYER_PORT_MID_RECOG = 7011;
const unsigned short NETLAYER_PORT_MID_SET_LOCAL = 7012;
const unsigned short NETLAYER_PORT_MID_SET_REMOTE = 7012;
const unsigned short NETLAYER_PORT_MID_MAINTAIN_LOCAL = 7013;
const unsigned short NETLAYER_PORT_MID_MAINTAIN_REMOTE = 7013;
const unsigned short NETLAYER_PORT_MID_EXT_LOCAL = 7014;
const unsigned short NETLAYER_PORT_MID_EXT_REMOTE = 7014;
const unsigned short NETLAYER_PORT_INNER_PRE_KWD = 17000;


// char buffer max length
const int NETLAYER_CHARBUF_MAX_LENGTH = 0x10000000;	// 256M
// PB header length
const int NETLAYER_PB_HEADER_LENGTH = 29;
// PB header sync flag
const int NETLAYER_PB_HEADER_SYN = 0xFF;
// PB header ver flag
const int NETLAYER_PB_HEADER_VER = 100;
// PB message max length
const int NETLAYER_PB_MAX_MSG_LENGTH = NETLAYER_CHARBUF_MAX_LENGTH;	
// echo message seconds
const int NETLAYER_ECHO_SEND_SEC = 10;
// echo message seconds
const int NETLAYER_ECHO_TIMEOUT_SEC = 30;
// microseconds in select
const int NETLAYER_SELECT_USEC = 1000;
// MsgQueue msg length
const unsigned NETLAYER_MSG_QUEUE_LENGTH = 64;
// BufQueue max msgBuffer count
const unsigned NETLAYER_BUF_QUEUE_MAXSIZE = 10000;
// default time-out seconds for SocketProcData
const unsigned NETLAYER_SOCKET_TIMEOUT = 5;
// milliseconds for long connection to sleep after queue ECHO msg
const unsigned NETLAYER_SLEEP_WAITECHO = 100;
// milliseconds for connection to sleep when try connect failed.
const unsigned NETLAYER_SLEEP_TRYCONNECT = 200;
// milliseconds for connection to sleep when no data send
const unsigned NETLAYER_SLEEP_SEND_NO_DATA = 200;
// milliseconds for doze time slice
const unsigned NETLAYER_DOZE_SLICE_MS = 16;
const unsigned NETLAYER_DOZE_DEFAULT_MS = 1;


//////////////////////////////////////////////////////////////////////////
// MsgQueue
//////////////////////////////////////////////////////////////////////////

class MsgQueue
{
public:
	// used to store internal error message
	MsgQueue();

	// NOT THREAD-SAFE
	void SetPrefix(const wchar_t* p);

	MsgQueue& operator=(const std::wstring & str);
	MsgQueue& operator=(const std::wstringstream & sstr);

	void info(const std::wstring & str);
	void info(const std::wstringstream & sstr);

private:
	long SetMsg( const std::wstring & str );
	std::deque<std::wstring> msgs;
	std::wstring prestr;
	volatile long idx;
};	

//////////////////////////////////////////////////////////////////////////
// CharBuffer
//////////////////////////////////////////////////////////////////////////

// not thread safe!
// size to alloc mem, ptr to get mem point
class CharBuffer
{
public:
	CharBuffer();
	~CharBuffer();

	bool size(int n);

	char* ptr(void)
	{ return p;	};
private:
	char* p;
	int len;
};

//////////////////////////////////////////////////////////////////////////
// MsgBuffer
//////////////////////////////////////////////////////////////////////////

// struct for PB header info and serialized PB msg
class MsgBuffer
{
public:
	MsgBuffer(int t, long long l, char* p, int n);
	~MsgBuffer();

	const int type;
	const long long seq;
	const char* const buf;
	const int len;
};

//////////////////////////////////////////////////////////////////////////
// BufQueue
//////////////////////////////////////////////////////////////////////////

class BufQueue
{
public:
	// used to store serialized PB msg for socket send, FIFO
	// usage: Add, Get, proc , delete
	// if proc failed and need re-proc later, use ReUse
	// if too many bufs, oldest bufs will remove & delete when Add.
	// all mem of added bufs will freed in destructor.

	// WARNING: BufQueue do not manage memory of MsgBuffer* in Get

	~BufQueue();

	// !!! do not free p after Push, p will freed when delete MsgBuffer*
	// if too many bufs, oldest bufs will removed when Add.
	void Add(int t, long long l, char* p, int n);
	// get first buf and remove from dq
	// !!!BufQueue will release management of pMsgBuffer
	MsgBuffer* GetAndRelease(void);
	// put a (previous retrieved) buf to dq
	void ReUse(MsgBuffer* const p);

private:
	std::deque<MsgBuffer*> dqBufs;
	CCriticalSection csBuf;
};

//////////////////////////////////////////////////////////////////////////
// PbTask
//////////////////////////////////////////////////////////////////////////




class NetLayerTask:
	public ThreadTask
{
public:
	NetLayerTask(const google::protobuf::int64 sn);

protected:
	google::protobuf::int64 seq;
	MsgQueue err;
	// trans utf-8 to unicode, string -> CStringW, NOT thread-safe
	std::wstring w_wstrTrans;
	const wchar_t* U2W(const std::string& strUtf8)
	{
		w_wstrTrans = PublicFunc::DecodeUtf8(strUtf8);
		return w_wstrTrans.c_str();
	}
};

class PbShortTask:
	public ThreadTask
{
public:
	PbShortTask(SOCKET s);
	~PbShortTask();

protected:
	SOCKET sd;
	MsgQueue err;
};

//////////////////////////////////////////////////////////////////////////
// socket functions
//////////////////////////////////////////////////////////////////////////
int SocketSendData(SOCKET& sd, const char* const pBuffer, const int nDataByte, 
				   time_t tmMax = NETLAYER_SOCKET_TIMEOUT);
int SocketRecvData(SOCKET& sd, char* pBuffer, const int nDataByte, 
				   time_t tmMax = NETLAYER_SOCKET_TIMEOUT);

std::string SocketGetName(SOCKET sd, unsigned short& uPort);

int SendPbMsg(SOCKET sd, const int type, long long seq, const google::protobuf::Message& msg);
int SendPbMsg(SOCKET sd, MsgBuffer* const pMsgBuf);

// pMsgBuffer should released outside
// RecvPbBuf will use pHdr to recv serialized HeadMsg, or alloc mem internally and free it when finished.
int RecvAndAllocPbBuf(SOCKET sd, MsgBuffer*& pMsgBuffer, char* pHdr = NULL);

std::string Unicode2Ansi(const wchar_t* pUnicode);
std::wstring Ansi2Unicode(const char* pAnsi);


//} // namespace NetLayer


#endif // NetLayer_h__