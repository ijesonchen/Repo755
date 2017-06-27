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

typedef UINT_PTR SOCKET;

namespace NetLayer
{


// PB header length
const int NETLAYER_PB_HEADER_LENGTH = 29;
// PB header sync flag
const int NETLAYER_PB_HEADER_SYN = 0xFF;
// PB header ver flag
const int NETLAYER_PB_HEADER_VER = 100;
// PB message max length
const int NETLAYER_PB_MAX_MSG_LENGTH = 0x10000000;	// 256M
// echo message seconds
const int NETLAYER_ECHO_SEC = 10;
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
// milliseconds for connection to sleep when connect failed.
const unsigned NETLAYER_SLEEP_RECONNECT = 200;

class MsgQueue
{
public:
	// used to store internal error message
	MsgQueue();

	// NOT THREAD-SAFE
	void SetPrefix(const wchar_t* p);

	MsgQueue& operator=(const std::wstring & str);
	MsgQueue& operator=(const std::wstringstream & sstr);

private:
	std::deque<std::wstring> msgs;
	std::wstring prestr;
	volatile long idx;
};	

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
class PbLongTask:
	public ThreadTask
{
public:
	PbLongTask(const google::protobuf::int64 sn);

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



} // namespace NetLayer


#endif // NetLayer_h__