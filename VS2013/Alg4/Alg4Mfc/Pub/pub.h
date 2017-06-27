#ifndef Pub_h__
#define Pub_h__

#include <string>
#include <iostream>
#include <memory>

// 绑定线程到指定CPU，默认到第二个CPU
void BindCpu(unsigned long mask = 2);


//////////////////////////////////////////////////////////////////////////
// StopWatch
class StopWatch
{
public:
	StopWatch();
	~StopWatch();
	static void Init();
	void Reset();
	float Stop();
private:
	static LARGE_INTEGER freq;
	LARGE_INTEGER start;
};

#endif // Pub_h__