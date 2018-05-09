// ProtobufTest.cpp : 定义控制台应用程序的入口点。
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


// 唯一的应用程序对象

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

	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: MFC 初始化失败\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 在此处为应用程序的行为编写代码。
	}

	new char[100];

	TestRepeat();

	cout << "end" << endl;

	getchar();


	return nRetCode;
}
