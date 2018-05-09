#include <windows.h>
#undef ERROR
#include "../glogsrc/logging.h"
#include <iostream>

using namespace std;

bool g_bRunning = true;

unsigned LogTest(int id)
{
	int ii = 0;
	while (g_bRunning)
	{
		stringstream ss;
		ss << "log by " << id << " , cnt " << ++ii;

		VLOG(10) << " 中文vlog " << ss.str();
		LOG(INFO) << " 中文info " << ss.str();
		LOG(WARNING) << " 中文warning" << ss.str();
		LOG(ERROR) << " 中文error " << ss.str();
		LOG_EVERY_N(ERROR, 10) << " 中文every10 " << google::COUNTER << "th " << ss.str();

		Sleep(1);
	}
	return 0;
}

int main(int argc, char** argv)
{
	// search GLOG_DEFINE_ for more FLAGS
	string str;
	str = argv[0];
	str = str.substr(0, str.rfind('\\'));
	FLAGS_log_dir = str;

	FLAGS_max_log_size = 10; // 10M
	FLAGS_colorlogtostderr = false;

	FLAGS_alsologtostderr = true;
	google::InitGoogleLogging(argv[0]);	

	LOG(INFO) << "this is a info log by main";
	int nThread = 1000;
	HANDLE* phThread = new HANDLE[nThread];
	for (int ii = 0; ii < nThread; ++ii)
	{
		phThread[ii] = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)LogTest, (LPVOID)ii, 0, 0);
	}

	char ch = getchar();
	g_bRunning = false;

	WaitForMultipleObjects(nThread, phThread, true, INFINITE);

	google::ShutdownGoogleLogging();

	return 0;
}