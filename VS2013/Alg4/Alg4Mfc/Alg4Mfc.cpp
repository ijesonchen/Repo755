// Alg4Mfc.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Alg4Mfc.h"
#include <iostream>
#include <vector>

#include "ch1/ch1.h"
#include "Pub/Pub.h"
#include <filesystem>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

#include <cstdint>
struct ProtoHdr
{
	std::uint32_t syn = 0x55FF;
	std::uint32_t ver = 0x0100;
	std::uint32_t seqno = 0;
	std::uint32_t type = 0;
	std::uint32_t len = 0;
};


using namespace std;
using namespace std::tr2::sys;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{

	path p("Z:\\115Chrome\\Application\\");
	for (auto s : p)
	{
		cout << s << endl;
	}

	cout << p.leaf() << endl;
	cout << p.parent_path() << endl;


	return 0;
	std::cout.setf(ios::boolalpha);
	int nRetCode = 0;
	BindCpu();

	StopWatch s;

	ex0103LinkOperation();



	s.Stop();

	printf("\npress enter to exit...");
	getchar();
	
	return nRetCode;
}
