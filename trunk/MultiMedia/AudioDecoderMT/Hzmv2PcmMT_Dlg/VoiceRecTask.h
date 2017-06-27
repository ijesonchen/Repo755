#pragma once

#include "ITask.h"

class CVoiceRecTask : public ITask
{
public:
	

	CVoiceRecTask(const CString& strVoiceFile)
		: w_strVoiceFile(strVoiceFile)
	{
	}

	virtual ~CVoiceRecTask(){}

	virtual int Run(LPVOID pParam);

	virtual void WriteLog(LPVOID pParam, size_t nDqSize);


private:

	CString w_strVoiceFile;
	CString w_strSausDir;
	CString w_strTmpDir;
};
