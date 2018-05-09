// ProtobufTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "ProtobufTest.h"

#ifdef _DEBUG
#pragma comment(lib, "libprotobuf_Debug_Win32.lib")
#else
#pragma comment(lib, "libprotobuf_Release_Win32.lib")
#endif // _DEBUG

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;


void trap(void);
void TestParse(void);
void TestSerialize(void);
void TestRepeat(void);
// test perf of memory operation for proto & user class
void TestPerf(void);


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
	}

	new char[100];

	TestRepeat();

	cout << "end" << endl;

	getchar();


	return nRetCode;
}
