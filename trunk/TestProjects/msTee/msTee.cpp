#include <stdio.h>
#include <time.h>
#include <string.h>
#include <memory.h>
#include <crtdbg.h>

#pragma warning(disable: 4996)

bool DumpMessage(char* szMsg, FILE* pfLog)
{
	printf("%s", szMsg);
	size_t nLen = strlen(szMsg);
	if (pfLog)
	{
		if (!fwrite(szMsg, strlen(szMsg), 1, pfLog))
		{
			printf("\n!!!!!!!! write error !!!!!!!!\n%s\n\n", szMsg);
			return false;
		}
	}
	return true;
}

void DumpTime(FILE* pfLog)
{
	time_t tNow = time(NULL);
	tm tmNow = *localtime(&tNow);
	const int nBufLen = 1024;
	char szTime[nBufLen] = {0};
	sprintf(szTime, "    **** TIME STAMP **** %04d-%02d-%02d %02d:%02d:%02d\n",
		tmNow.tm_year + 1900, tmNow.tm_mon, tmNow.tm_mday, 
		tmNow.tm_hour, tmNow.tm_min, tmNow.tm_sec);
	DumpMessage(szTime, pfLog);
}

int main(int argc, char* argv[])
{
	FILE* pfLog = NULL;
	bool bForceDebugPoint = false;
	if (argc == 1)
	{
		printf("Usage: [cmd] | msTee [log-file-name] [force-debug]\n");
		printf("Show and save [cmd]'s output to [log-file-name].\n");
		printf("If [force-debug] present (can be anything), will trigger break point forcedly\n");
		printf("Press ^z to exit.\n");
	}
	else if (argc >= 2)
	{
		if (argc >= 4)
		{
			_CrtDbgBreak();
		}
		char* pszFile = argv[1];
		pfLog = fopen(pszFile, "at");
		if (!pfLog)
		{
			printf("\n    !!!!!!!! open file error !!!!!!!!\n%s\n\n", pszFile);
		}
	}

	DumpMessage("    ******** Time Stamp will insert every 100 lines or 10 secs ********\n", pfLog);
	DumpMessage("    ******** BEGIN DUMP MESSAGE ********\n", pfLog);
	DumpTime(pfLog);

	// insert time stamp every 100 lines or 10 secs
	const int nLineStep = 100;
	const int nTimeStep = 10;
	int nCheckLine = 0;
	time_t tCheckTime = time(NULL);

	// init buffer
	const int nBufLen = 1024*1024; // 1M
	char* pszBuffer = new char[nBufLen];
	memset(pszBuffer, 0, nBufLen);
	// loop
	while (fgets(pszBuffer, nBufLen, stdin))
	{
		DumpMessage(pszBuffer, pfLog);

		bool bTimeStamp = false;
		if (++nCheckLine >= nLineStep)
		{
			bTimeStamp = true;
		}
		if (time(NULL) - tCheckTime >= nTimeStep)
		{
			bTimeStamp = true;
		}
		if (bTimeStamp)
		{
			nCheckLine = 0;
			tCheckTime = time(NULL);
			DumpTime(pfLog);
		}
	}

	if (feof(stdin))
	{
		DumpMessage("    ******** END WITH EOF ********\n", pfLog);
	}
	else
	{
		DumpMessage("    ******** END ABNORMAL ********\n", pfLog);
	}

	DumpMessage("    ******** DUMPING MESSAGE FINISHED ********\n", pfLog);
	DumpTime(pfLog);

	if (pfLog)
	{
		fclose(pfLog);
		pfLog = NULL;
	}

	return 0;
}