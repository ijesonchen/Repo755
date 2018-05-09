#include <windows.h>

// avoid ERROR defination in windows.h & logging.h
#undef ERROR
// static link to glog lib
#define GOOGLE_GLOG_DLL_DECL
#include <glog/logging.h>
#include <iostream>
#include <deque>

using namespace std;

// atuo link lib

#ifdef _DEBUG
#pragma comment(lib, "libglog_static_Debug_Win32")
#else
#pragma comment(lib, "libglog_static_Release_Win32.lib")
#endif // _DEBUG


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

		VLOG(10) << " 中文vlog " << ss.str();
		LOG(INFO) << " 中文info " << ss.str();
		LOG(WARNING) << " 中文warning" << ss.str();
		LOG(ERROR) << " 中文error " << ss.str();
		LOG_EVERY_N(ERROR, 10) << " 中文every10 " << google::COUNTER << "th " << ss.str();

		VLOG(10) << " 中文vlog " << ss.str();
		LOG(INFO) << " 中文info " << ss.str();
		LOG(WARNING) << " 中文warning" << ss.str();
		LOG(ERROR) << " 中文error " << ss.str();
		LOG_EVERY_N(ERROR, 10) << " 中文every10 " << google::COUNTER << "th " << ss.str();

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

	// set log path to program path
	string str;
	str = argv[0];
	str = str.substr(0, str.rfind('\\'));
	FLAGS_log_dir = str;

	// max log size MB
	FLAGS_max_log_size = 10; 
	FLAGS_colorlogtostderr = false;

	// all log to stderr (console)
	FLAGS_alsologtostderr = true;


	// set log main name
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

	google::ShutdownGoogleLogging();

	Sleep(10000);


	for (int ii = 0; ii < nThread; ++ii)
	{
		WaitForSingleObject(phThread[ii], INFINITE);
	}

	// clear glog global memory


	return 0;
}