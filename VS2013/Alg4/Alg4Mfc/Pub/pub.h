#ifndef Pub_h__
#define Pub_h__

#include <string>
#include <iostream>
#include <memory>

// ���̵߳�ָ��CPU��Ĭ�ϵ��ڶ���CPU
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