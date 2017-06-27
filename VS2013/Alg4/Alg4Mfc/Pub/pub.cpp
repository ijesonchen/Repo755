#include "..\stdafx.h"
#include "Pub.h"
#include <iostream>
#include <locale>

using namespace std;

// 绑定线程到指定CPU，默认到第二个CPU
void BindCpu(unsigned long mask /*= 2*/)
{
	auto h = GetCurrentThread();
	auto p = SetThreadAffinityMask(h, mask);
	StopWatch::Init();
}

//////////////////////////////////////////////////////////////////////////
// StopWatch
LARGE_INTEGER StopWatch::freq;

StopWatch::StopWatch()
{
	Reset();
}

StopWatch::~StopWatch()
{
	;
}

void StopWatch::Init()
{
	QueryPerformanceFrequency(&freq);
}

void StopWatch::Reset()
{
	QueryPerformanceCounter(&start);
}

float StopWatch::Stop()
{
	LARGE_INTEGER stop;
	QueryPerformanceCounter(&stop);

	auto counter = stop.QuadPart - start.QuadPart;
	cout << "**** time counter " << counter << " ";
	float timeval = static_cast<float>(counter) / freq.QuadPart;
	
	if (timeval > 1)
	{
		cout << "** " << timeval << " secs" << endl;
	}
	else if (timeval > 0.001)
	{
		cout << "** " << timeval * 1000 << " millisecs" << endl;
	}
	else if (timeval > 0.000001)
	{
		cout << "** " << timeval * 1000000 << " microsecs" << endl;
	}
	else if (timeval > 0.000000001)
	{
		cout << "** " << timeval * 1000000000 << " nanosecs" << endl;
	}
	return timeval;
}