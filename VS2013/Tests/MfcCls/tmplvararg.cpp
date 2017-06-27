#include "stdafx.h"
#include <iostream>
#include <sstream>
using namespace std;

//////////////////////////////////////////////////////////////////////////
// 可变参数模板
template<typename T>
void Print(std::wstringstream& wss, const T& t)
{
	wss << t;
}

template<typename T, typename... Args>
void Print(std::wstringstream& wss, const T& t, const Args& ... rest)
{
	wss << t << ": ";
	Print(wss, rest...);
}

template<typename... Args>
void Log(const Args&... all)
{
	std::wstringstream wss;
	Print(wss, all...);
	std::wcout << wss.str() << std::endl;
}

void TestTmplVarArg()
{
	Log(L"0");
	Log(L"a", L"b", L"c", L"d");
}

