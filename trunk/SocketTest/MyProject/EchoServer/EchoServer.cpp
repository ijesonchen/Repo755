// EchoServer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <time.h>

#pragma warning (disable: 4996)
#pragma comment(lib, "Ws2_32.lib")

// type
typedef unsigned short u_16;
typedef unsigned int u_32;

// const
// 6011 for send and 6012 for recv
#define SEND_PORT 6011
#define RECV_PORT 6012
#define TEL_LENGTH		256
#define VOICEID_SIZE	37
#define FILENAME_SIZE	256
#define FILEPATH_SIZE	260

const int BUFFER_LENGTH = 1024*1024;
static int g_nMsgCounter = 0;

// message type
#define MSG_HEART_REQUEST        0x0011     // ��������
#define MSG_HEART_RESPONSE       0x0012     // ������Ӧ
#define MSG_VOICE_INFO	0x0013 // ������Ϣ����֪ͨV2,V1

#define MSG_MAGIC	(0xFFFF)
#define MSGID_SIZE       32


#define HTONL(x) (x = ntohl(x))
#define HTONS(x) (x = htons(x))
#define NTOHL(x) (x = ntohl(x))
#define NTOHS(x) (x = ntohs(x))

//////////////////////////////////////////////////////////////////////////
// message structure
typedef struct _PackHead
{
	u_16 w_nFlag;   // ͬ����־
	u_16 w_nVer;	// Э��汾
	u_32 w_nMsgType; // ��Ϣ����
	char w_szMsgID[MSGID_SIZE] ;   // ��Ϣ���к�
	u_32 w_nMsgLen; // ��Ϣ��ĳ���
	void hton(void)
	{
		HTONS(w_nFlag);   
		HTONS(w_nVer);	
		HTONL(w_nMsgType); 
		HTONL(w_nMsgLen); 
	}
	void ntoh(void)
	{
		NTOHS(w_nFlag);   
		NTOHS(w_nVer);	
		NTOHL(w_nMsgType); 
		NTOHL(w_nMsgLen); 
	}
}PackHead;

typedef struct _HeartBeat
{
	PackHead	w_Head;
	u_32		w_nDirect;   //�������	Int(32)	M	��ʶ:����Ϊ0����ӦΪ1
	u_32		w_nTimeStamp;//ʱ���	Int(32)	M	1970�굽��ǰʱ�������
	void hton(void)
	{
		HTONL(w_nDirect);
		HTONL(w_nTimeStamp);
	}
	void ntoh(void)
	{
		NTOHL(w_nDirect);
		NTOHL(w_nTimeStamp);
	}
}HeartBeat;

// ��Ϣ��
// ��������֪ͨ
typedef struct _VoiceInfo
{
	PackHead w_Head;
	u_32	w_nCallerType;					// Ŀ������(��Ӫ������)	Int(32)	M	
	char	w_szCallerTelNumber[TEL_LENGTH];	// ���к��� Char(32) M
	char	w_szCalledTelNumber[TEL_LENGTH];	// ���к���	Char(32)	C
	char    w_szFileName[FILENAME_SIZE];	// �����ļ���	Char(128)	C
	char    w_szFilePathA[FILEPATH_SIZE];	// A·�����ߺ�¼�������ļ�ȫ·��
	char    w_szFilePathB[FILEPATH_SIZE];	// B·�����ļ�ȫ·��
	u_32    w_nStartTime;					// ͨ����ʼʱ��	Int(32)	C	
	u_32    w_nEndTime;						// ͨ������ʱ��	Int(32)	C	
	u_32	w_nComposeFlag;					// �ϳɱ�ʶ Int(32) C 0��ʾ�ϳɣ�1����ֻ��A·��2����ֻ��B½��3����A,B����
	char    w_szVoiceID[VOICEID_SIZE];		// ������ʾ	Char(37)	M
	u_32	w_nProcessModeBits;				// ��λʵ��Ϊ���������Ϣ���͹���
	u_32	w_nCodeType;					// ������������
	u_32	w_nXmlLength;					// xml���ȣ���0ʱ��XML��Ϣ

	void hton(void)
	{
		HTONL(w_nCallerType);				   
		HTONL(w_nStartTime);				   
		HTONL(w_nEndTime);					   
		HTONL(w_nComposeFlag);				 
		HTONL(w_nProcessModeBits);		 
		HTONL(w_nCodeType);					   
		HTONL(w_nXmlLength);					 
	}
	void ntoh(void)
	{
		NTOHL(w_nCallerType);				
		NTOHL(w_nStartTime);				
		NTOHL(w_nEndTime);					
		NTOHL(w_nComposeFlag);				
		NTOHL(w_nProcessModeBits);		
		NTOHL(w_nCodeType);					
		NTOHL(w_nXmlLength);			
	}
}VoiceInfo;

typedef struct _VoiceInfResult 
{
	PackHead	w_Head;
	u_32	w_nCallerType;					//Ŀ������(��������)	Int(32)	M	
	char	w_szCallerTelNumber[TEL_LENGTH];	//�����ʶ(�������) Char(32) M
	char	w_szCalledTelNumber[TEL_LENGTH];	//�Զ˺���	Char(32)	C
	char    w_szFileName[FILENAME_SIZE];	//�����ļ���	Char(128)	C
	u_32	w_nStartTime;                //ͨ����ʼʱ��	Int(32)	C	
	u_32	w_nEndTime;                  //ͨ������ʱ��	Int(32)	C	
	char	w_szRecogResult[32];				//ʶ������	Char(32)	M	��������ʶ����Ľ�������ʶ��ɹ���ʶ��ʧ��
	char	w_szVoiceID[VOICEID_SIZE];			//������ʾ	Char(128)	M
	u_32	w_nComposeType;					//�ϳɱ�ʶ	Int(32)	M	����·ȡ0������ȡ1��2��
	u_32	w_nXmlLength;						// XMLʶ��������
	void hton(void)
	{
		HTONL(w_nCallerType);
		HTONL(w_nStartTime);
		HTONL(w_nEndTime);
		HTONL(w_nComposeType);
		HTONL(w_nXmlLength);
	}
	void ntoh(void)
	{
		NTOHL(w_nCallerType);
		NTOHL(w_nStartTime);
		NTOHL(w_nEndTime);
		NTOHL(w_nComposeType);
		NTOHL(w_nXmlLength);
	}
} VoiceInfResult;
//////////////////////////////////////////////////////////////////////////

// functions
int netSetMsgVoiceInfo( char * pBufSend, const int nBufLen = BUFFER_LENGTH) 
{
	memset(pBufSend, 0, nBufLen);
	if (sizeof(VoiceInfo) >= nBufLen)
	{
		return 0;
	}
	g_nMsgCounter++;

	VoiceInfo* pInfo = (VoiceInfo*)pBufSend;

	PackHead* pHeader = &pInfo->w_Head;
	pHeader->w_nVer = 2;
	pHeader->w_nFlag = MSG_MAGIC;
	pHeader->w_nMsgType = (MSG_VOICE_INFO);
	pHeader->w_nMsgLen = (sizeof(VoiceInfo) - sizeof(PackHead));
	pHeader->hton();

	pInfo->w_nCallerType = (g_nMsgCounter);
	_snprintf(pInfo->w_szCallerTelNumber, TEL_LENGTH, "Caller:%08d", g_nMsgCounter);
	_snprintf(pInfo->w_szCalledTelNumber, TEL_LENGTH, "Called:%08d", g_nMsgCounter);
	_snprintf(pInfo->w_szFileName, FILENAME_SIZE, "w_szFileName:%08d", g_nMsgCounter);
	_snprintf(pInfo->w_szFilePathA, FILEPATH_SIZE, "w_szFilePathA:%08d", g_nMsgCounter);
	_snprintf(pInfo->w_szFilePathB, FILEPATH_SIZE, "w_szFilePathB:%08d", g_nMsgCounter);
	pInfo->w_nStartTime = ((u_long)time(NULL));
	pInfo->w_nEndTime = ((u_long)time(NULL) + 1);
	pInfo->w_nComposeFlag = 0x03;	// ����1�ͷ���2
	_snprintf(pInfo->w_szVoiceID, VOICEID_SIZE, "w_szVoiceID:%08d", g_nMsgCounter);
	pInfo->w_nProcessModeBits = 0x05;	// ����ʶ�� + ����ʶ��
	pInfo->w_nCodeType = 0x34;	// PCM  64Kbps
	pInfo->w_nXmlLength = 0;

	pInfo->hton();
	
	return sizeof(VoiceInfo);
}

int SendPackage(
		 __in  SOCKET s,
		 __in  const char* buf,
		 __in  int len,
		 __in  int flags = NULL
		 )
{
	int nSend = 0;
	int nLeft = len;
	while (nLeft)
	{
		nSend = send(s, buf + len - nLeft, nLeft, flags);
		if (SOCKET_ERROR == nSend)
		{
			return SOCKET_ERROR;
		}
		nLeft -= nSend;
	}
	return len;
}

int RecvPackage(
		 __in  SOCKET s,
		 __in  char* buf,
		 __in  int len,
		 __in  int flags = NULL
				)
{
	int nRecv = 0;
	int nTotal = 0;
	while (nTotal < len)
	{
		nRecv = recv(s, buf + nTotal, len - nTotal, flags);
		if (SOCKET_ERROR == nRecv)
		{
			return SOCKET_ERROR;
		}
		nTotal += nRecv;
	}
	return nTotal;
}

DWORD WINAPI SendThread(LPVOID nPort) 
{
	int nRet = 0;
	SOCKET sListen = INVALID_SOCKET;
	SOCKET sClient = INVALID_SOCKET;
	int nSendPort = (int)nPort;

	char* pBufSend = new char[BUFFER_LENGTH];
	try
	{
		// init
		WSADATA wsa;

		if (WSAStartup(MAKEWORD(2, 2), &wsa))
		{
			throw 1;
		}

		addrinfo aiHints;
		ZeroMemory(&aiHints, sizeof(aiHints));
		aiHints.ai_family = AF_INET;
		aiHints.ai_socktype = SOCK_STREAM;
		aiHints.ai_protocol = IPPROTO_TCP;
		aiHints.ai_flags = AI_PASSIVE;

		// create
		sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

		if (INVALID_SOCKET == sListen)
		{
			throw 2;
		}

		// bind
		sockaddr_in sAddrBind;
		sAddrBind.sin_family = AF_INET;
		sAddrBind.sin_port = htons(nSendPort);
		sAddrBind.sin_addr.S_un.S_addr = ADDR_ANY;
		memset(&sAddrBind.sin_zero, 0, sizeof(sAddrBind.sin_zero));
		if (SOCKET_ERROR == bind(sListen, (sockaddr*)&sAddrBind, sizeof(sAddrBind)))
		{
			throw 3;
		}

		// listen
		if (SOCKET_ERROR == listen(sListen, SOMAXCONN))
		{
			throw 4;
		}

		printf("SENF waiting for connection...\n");
		// accept
		sockaddr sAddrClient;
		int nAddrClientLen = sizeof(sAddrClient);
		sClient = accept(sListen, &sAddrClient, &nAddrClientLen);
		if (INVALID_SOCKET == sClient)
		{
			throw 5;
		}
		if (sAddrClient.sa_family == AF_INET)
		{
			// print client address info
			printf("SEND client %u.%u.%u.%u connected on %u\n",
				(unsigned char)sAddrClient.sa_data[2],
				(unsigned char)sAddrClient.sa_data[3],
				(unsigned char)sAddrClient.sa_data[4],
				(unsigned char)sAddrClient.sa_data[5], 
				ntohs(*(unsigned short*)sAddrClient.sa_data));
		}
		// recv or send

		// fill header
		int nSleep = 180;
		printf("sleep %d seconds", nSleep);
		Sleep(nSleep * 1000);
		printf("send data");
		int nSendLen = netSetMsgVoiceInfo(pBufSend, BUFFER_LENGTH);
		SendPackage(sClient, pBufSend, nSendLen);

// 		int nSendCount = 200;
// 		for (int ii = 0; ii < nSendCount; ii++)
// 		{
// 			if (nSendLen != SendPackage(sClient, pBufSend, nSendLen))
// 			{
// 				throw 6;
// 			}
// 			Sleep(100);
// 		}
		shutdown(sClient, SD_BOTH);
	}
	catch (int nCode)
	{
		printf("SEND Error: %d\n", nCode);
		nRet = nCode;
	}

	// cleanup
	if (INVALID_SOCKET != sListen)
	{
		closesocket(sListen);
	}
	if (INVALID_SOCKET != sClient)
	{
		closesocket(sClient);
	}
	WSACleanup();

	delete pBufSend;
	pBufSend = NULL;

	printf("SEND finished.\n");

	return nRet;
}

DWORD WINAPI RecvThread(LPVOID nPort)
{
	int nRet = 0;

	SOCKET sListen = INVALID_SOCKET;
	SOCKET sClient = INVALID_SOCKET;
	int nRecvPort = (int)nPort;

	char* pBufRecv = new char[BUFFER_LENGTH];
	const int nHeader = sizeof(PackHead);
	PackHead* pBufHead = (PackHead*)pBufRecv;
	VoiceInfResult* pResult = (VoiceInfResult*)pBufRecv;
	char* pXml = (char*)(pBufRecv + sizeof(VoiceInfResult));
	try
	{
		// init
		WSADATA wsa;

		if (WSAStartup(MAKEWORD(2, 2), &wsa))
		{
			throw 1;
		}

		addrinfo aiHints;
		ZeroMemory(&aiHints, sizeof(aiHints));
		aiHints.ai_family = AF_INET;
		aiHints.ai_socktype = SOCK_STREAM;
		aiHints.ai_protocol = IPPROTO_TCP;
		aiHints.ai_flags = AI_PASSIVE;

		// create
		sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

		if (INVALID_SOCKET == sListen)
		{
			throw 2;
		}

		// bind
		sockaddr_in sAddrBind;
		sAddrBind.sin_family = AF_INET;
		sAddrBind.sin_port = htons(nRecvPort);
		sAddrBind.sin_addr.S_un.S_addr = ADDR_ANY;
		memset(&sAddrBind.sin_zero, 0, sizeof(sAddrBind.sin_zero));
		if (SOCKET_ERROR == bind(sListen, (sockaddr*)&sAddrBind, sizeof(sAddrBind)))
		{
			throw 3;
		}
RECVCODE:
		// listen
		if (SOCKET_ERROR == listen(sListen, SOMAXCONN))
		{
			throw 4;
		}

		printf("RECV waiting for connection...\n");

		SOCKET stemp = INVALID_SOCKET;
		// accept
		sockaddr sAddrClient;
		int nAddrClientLen = sizeof(sAddrClient);
		stemp = accept(sListen, &sAddrClient, &nAddrClientLen);
		if (INVALID_SOCKET != sClient)
		{
			closesocket(sClient);
		}
		sClient = stemp;

		if (INVALID_SOCKET == sClient)
		{
			throw 5;
		}
		if (sAddrClient.sa_family == AF_INET)
		{
			// print client address info
			printf("RECV client %u.%u.%u.%u connected on %u\n",
				(unsigned char)sAddrClient.sa_data[2],
				(unsigned char)sAddrClient.sa_data[3],
				(unsigned char)sAddrClient.sa_data[4],
				(unsigned char)sAddrClient.sa_data[5], 
				ntohs(*(unsigned short*)sAddrClient.sa_data));
		}
		// recv or send

		// recv header

		int nRecv = 0;
		while ((nRecv = recv(sClient, pBufRecv, BUFFER_LENGTH, NULL)) > 0)
		{
			CTime time = CTime::GetCurrentTime();
			CStringA str;
			str = time.Format("%Y%m%d %H:%M:%S");
			printf("%d bytes received at %s \n", nRecv, str.GetBuffer());
			if (nRecv == sizeof(HeartBeat))
			{
				HeartBeat* pMsg = (HeartBeat*)pBufRecv;
				pMsg->w_Head.ntoh();
				pMsg->ntoh();
				if (pMsg->w_Head.w_nMsgType == MSG_HEART_REQUEST)
				{
					CTime timeHeart(pMsg->w_nTimeStamp);
					printf("HeartBeat received at %s", timeHeart.Format("%Y%m%d %H:%M:%S").GetBuffer());
				}
			}
		}
		if (nRecv == 0)
		{
			printf("Recv finished.\n");
			goto RECVCODE;
		}
// 		int nCount = 200;
// 		for (int ii = 0; ii < nCount; ii++)
// 		{
// 			memset(pBufHead, 0, nHeader);
// 			int nRecv = 0;
// 			nRecv = RecvPackage(sClient, pBufRecv, nHeader);
// 			if (SOCKET_ERROR == nRecv)
// 			{
// 				throw 6;
// 			}
// 			if (nHeader == nRecv)
// 			{
// 				pBufHead->hton();
// 				nRecv = RecvPackage(sClient, pBufRecv + nHeader, pBufHead->w_nMsgLen);
// 				if (SOCKET_ERROR == nRecv)
// 				{
// 					throw 7;
// 				}
// 				if (pBufHead->w_nMsgLen == nRecv)
// 				{
// 					pResult->hton();
// 				}
// 			}
// 		}

		shutdown(sClient, SD_BOTH);
	}
	catch (int nCode)
	{
		printf("RECV Error: %d\n", nCode);
		nRet = nCode;
	}

	// cleanup
	if (INVALID_SOCKET != sListen)
	{
		closesocket(sListen);
	}
	if (INVALID_SOCKET != sClient)
	{
		closesocket(sClient);
	}
	WSACleanup();

	delete pBufRecv;
	pBufRecv = NULL;

	printf("RECV finished.\n");

	return nRet;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int nRet = 0;
	int nPortRecv = RECV_PORT;

// 	HANDLE hSendThread = NULL;
// 	hSendThread = CreateThread(NULL, NULL, SendThread, (LPVOID)SEND_PORT, NULL, NULL);

	HANDLE hRecvThread = NULL;
	hRecvThread = CreateThread(NULL, NULL, RecvThread, (LPVOID)RECV_PORT, NULL, NULL);

	getchar();

	return 0;
}

