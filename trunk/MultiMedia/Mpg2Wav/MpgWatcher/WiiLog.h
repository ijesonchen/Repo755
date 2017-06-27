
//内容说明：定义日志功能

#pragma once


/* NOTE:
 * (1) 必须在一个而且只能一个模块中使用DECLARE_WTP_LOG或OPEN_WTP_LOG，其中，
 *     DECLARE_WTP_LOG并不打开一个具体的LOG文件，在使用前必须调用SELECT_WTP_LOG打开一个具体的log文件
 *     这个模块称为主日志模块；
 * (2) 在每个模块的 
 *        #include "WtpLog.h" 
 *     前使用	
 *        #define DISABLE_WTP_LOG
 *     可以关闭本模块的日志功能，但是不能应用于主日志模块；
 * (3) 打开WtpLog.h中的
 *			#define DISABLE_WTP_LOG_IN_ALL_MODULES
 *     将关闭所有模块的日志功能
 * (4) 打开或关闭WtpLog.h中的
 *          #define MULTI_THREADS_MODE
 *     可以打开或关闭多线程模式；
 *
 * (5) 对外的宏定义
 *     DECLARE_WTP_LOG：				声明日志功能，应用于主日志模块
 *     OPEN_WTP_LOG(strFile, bApp)：	声明并打开一个日志文件，bApp表示是否是往后添加模式
 *     SELECT_WTP_LOG(strFile, bApp):	打开或重新打开一个日志文件
 *     WtpLog(strFormat, ...):			输出日志
 *     GET_WTP_LOG：					返回当前的日志文件名，返回类型是（const char*）类型
 */

//#define DISABLE_WTP_LOG_IN_ALL_MODULES
#define MULTI_THREADS_LOG_MODE

#ifdef DISABLE_WII_LOG_IN_ALL_MODULES
  #undef  ENABLE_WII_LOG
#else
  #ifdef DISABLE_WII_LOG
    #undef  ENABLE_WII_LOG
  #else
    #define ENABLE_WII_LOG
  #endif
#endif

#ifndef  ENABLE_WII_LOG

#define SELECT_WII_LOG(strFile, bApp, pFunctionPtr)
#define WiiLog
#define GET_WII_LOG
#define SET_LOG_MODE
#define SET_LOG_HEAD(strHead)

#else

#define SELECT_WII_LOG(strFile, bToEnd)	CWiiLog::GetLogHandle()->Select(strFile, bToEnd);
#define WiiLog							CWiiLog::GetLogHandle()->WriteLog
#define GET_WII_LOG						(CWiiLog::GetLogHandle()->GetFile())
#define SET_LOG_MODE(nLogTypeMask)		CWiiLog::GetLogHandle()->SetType(nLogTypeMask);
#define SET_NO_CALLBACK		CWiiLog::GetLogHandle()->UseCallback(FALSE);
#define SHOW_LOG_TIME					CWiiLog::GetLogHandle()->ShowTime
#define SET_LOG_HEAD(strHead)			CWiiLog::GetLogHandle()->SetCustomLogHead(strHead);

// 常量定义
// log文件最大大小
const long kvWiiLogFileMaxLength = (long)1024 * 1024 * 10;
// log文件中一条记录大小
const int  kvWiiLogMaxBuffer = 10240; 

// 日志类型
enum WII_LOG_TYPE
{
	kDebugLogType = 1,		// 调试Log
	kWarningLogType = 2,	// 警告Log
	kErrorLogType = 4,		// 错误Log
	kNormalLogType = 8,		// 正常LOG
	kStatistLogType = 16,	// 统计LOG
	kSystemLogType = 32,	// 系统LOG
};

inline CString GetWiiLogTypeCString(WII_LOG_TYPE kLogType)
{
	switch (kLogType)
	{
		case kDebugLogType:
			return _T("调试");
		case kWarningLogType:
			return _T("警告");
		case kErrorLogType:
			return _T("错误");
		case kNormalLogType:
			return _T("信息");
		case kStatistLogType:
			return _T("统计");
		case kSystemLogType:
			return _T("系统");
		default:
			return _T("");
	}
}

#define WM_WRITELOGTOTEXT WM_USER + 3333
// 日志类
class CWiiLog
{
private:
	CWiiLog();
	//CWiiLog(const char *strFile,  bool bApp, FUNCTIONPTR pFunctionPtr = NULL); // bApp for append log

public:
	// 析构函数
	~CWiiLog();

	static void SetMsgReceiver(CWnd *pWnd){ w_pMessageReceiver = pWnd; }
	bool WriteLogToList(WII_LOG_TYPE kLogType, const CString& strLogContent);

	bool IsOpen()
	{
		return w_fileLog.m_hFile != CFile::hFileNull;
	}

	void Close();
	
	bool Select(const CString strFile, bool bToEnd);
	
	// 设置日志类型
	inline void SetType(const int nLogTypeMask)
	{
		w_nLogTypeMask = nLogTypeMask;
	};

	inline void SetCustomLogHead(CString& strHead)
	{
		w_strLogHead = strHead;
	}

	inline void ShowTime(bool bShow = true)
	{
		w_bShowTime = bShow;
	}

	inline void ShowLogType(bool bShow = true)
	{
		w_bShowLogType = bShow;
	}
	
	inline void UseCallback(bool bUse = false)
	{
		w_bUseCallback = bUse;
	}

	// 是否设置了某个日志类型
	inline bool IsLoggedType(const WII_LOG_TYPE kLogType)
	{
		return (kLogType & w_nLogTypeMask) > 0 ? true : false;
	};

	// 获得日志对象的指针
	static CWiiLog* GetLogHandle();
	
	// 写日志
	bool WriteLog(WII_LOG_TYPE kLogType, const TCHAR *strFormat, ...);

	bool WriteLog(const TCHAR *strFormat,...);
	
	// 获得log文件的名称
	const CString& GetFile() const;

protected:

	bool RealWrite(WII_LOG_TYPE kLogType, const TCHAR* strLog);


protected:
	// 日志文件名称
	CString w_strFile;
	// 日志输出流
	CFile w_fileLog;
	// 输出的日志类型
	int	w_nLogTypeMask;
	// 自定义的日志头
	CString w_strLogHead;
	// 是否显示日志时间
	bool w_bShowTime;
	// 是否显示日志类型
	bool w_bShowLogType;
	// 是否调用回调
	bool w_bUseCallback;

	static CWnd* w_pMessageReceiver;

	static CWiiLog __g_wii_log__;

	DWORD w_LastRealWriteTime;


#ifdef MULTI_THREADS_LOG_MODE
	CCriticalSection w_csLogLock;
#endif
};

#endif // ]  ENABLE_WII_LOG


