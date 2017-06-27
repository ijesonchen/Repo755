#pragma once

class ITask
{
public:
	virtual int Run(LPVOID pParam) = 0;
	virtual void WriteLog(LPVOID pParam, size_t nDqSize) = 0;
	virtual	~ITask()
	{
	}
};