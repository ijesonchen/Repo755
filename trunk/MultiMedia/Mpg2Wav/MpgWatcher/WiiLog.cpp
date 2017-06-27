/*--------------------------------------------------------------------------
<WtpLog.cpp> : <WtpLog.h>

内容说明：定义日志功能
版权所有：上海优浪信息科技股份有限公司

版本历史：
2001-2002：		第一稿；				作者：吴田平，审查：

----------------------------------------------------------------------------*/

//////////////////////////////////////////////////////////////////////////
// RealWrite函数控制log实际显示和写入
// if (tickCount -  w_LastRealWriteTime > 100) 表示最短log显示间隔，单位ms
// if (kNormalLogType != kLogType) 表示只写入非【信息】log

#include "StdAfx.h"
#include "WiiLog.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace std;

#ifdef  ENABLE_WII_LOG

// 全局类
CWiiLog CWiiLog::__g_wii_log__;

CWiiLog::CWiiLog()
#ifdef DEBUG
: w_nLogTypeMask(kDebugLogType | kWarningLogType | kErrorLogType | kNormalLogType | kSystemLogType)
#else
: w_nLogTypeMask(kWarningLogType | kErrorLogType | kNormalLogType | kSystemLogType)
#endif
, w_bShowLogType(true), w_bShowTime(true), w_bUseCallback(true)
, w_LastRealWriteTime(0)
{
}



CWiiLog::~CWiiLog()
{
	Close();
}

// 得到全局log对象的指针
CWiiLog* CWiiLog::GetLogHandle()
{
	return &__g_wii_log__;
}

void CWiiLog::Close()
{
	if (w_fileLog.m_hFile != CFile::hFileNull )
	{
		w_fileLog.Flush();
		w_fileLog.Close();
	}
	w_strFile = _T("");
}


// 选择log文件
bool CWiiLog::Select(const CString strFile, bool bToEnd)
{

	CFileException ex;
#ifdef MULTI_THREADS_LOG_MODE
	CSingleLock singleLock(&w_csLogLock, TRUE);
#endif

	Close();

	DWORD dwRetFile = ::GetFileAttributes(strFile);
	if ( dwRetFile == INVALID_FILE_ATTRIBUTES || (dwRetFile & FILE_ATTRIBUTE_DIRECTORY) != 0) //文件不存在
	{
		if (!w_fileLog.Open(strFile, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite, &ex))
		{
			return false;
		}
	}
	else
	{
		if (!w_fileLog.Open(strFile, CFile::modeWrite | CFile::shareDenyWrite, &ex))
		{
			return false;
		}
		if (bToEnd)
		{
			w_fileLog.SeekToEnd();
		}
	}


#ifdef MULTI_THREADS_LOG_MODE
	singleLock.Unlock();
#endif


	w_strFile = strFile;
	return true;
}

const CString& CWiiLog::GetFile() const
{
	return w_strFile;
}

#pragma warning(disable: 4996)

bool CWiiLog::WriteLog(WII_LOG_TYPE kLogType, const TCHAR *strFormat, ...)
{
	// 如果不是应该记录的日志类型，则直接返回
	if (!IsLoggedType(kLogType))
	{
		return true;
	}

	TCHAR strLog[kvWiiLogMaxBuffer];
	va_list va;
	va_start(va, strFormat);
	_vstprintf(strLog, strFormat, va);
	va_end(va);

	return RealWrite(kLogType, strLog);
}

bool CWiiLog::WriteLog(const TCHAR *strFormat, ...)
{
	// 如果不是应该记录的日志类型，则直接返回
	if (!IsLoggedType(kErrorLogType))
	{
		return true;
	}

	TCHAR strLog[kvWiiLogMaxBuffer];
	va_list va;
	va_start(va, strFormat);
	_vstprintf(strLog, strFormat, va);
	va_end(va);

	return RealWrite(kErrorLogType, strLog);
}

#pragma warning(default: 4996)

bool CWiiLog::RealWrite(WII_LOG_TYPE kLogType, const TCHAR* strLog)
{
	CTime tm = CTime::GetCurrentTime();

	CString strTemp = _T("");
	if (w_bShowTime)
	{
		strTemp += tm.Format(_T("%Y-%m-%d %H:%M:%S")) + _T(" ");
	}

	if (w_bShowLogType)
	{
		strTemp += _T("[") + GetWiiLogTypeCString(kLogType) + _T("] ");
	}

	if (!w_strLogHead.IsEmpty())
	{
		strTemp += w_strLogHead + _T(" ");
	}

	strTemp += strLog;
	strTemp += _T("\r\n");

	DWORD tickCount = GetTickCount();
	if (tickCount -  w_LastRealWriteTime > 10)
	{
		w_LastRealWriteTime = tickCount;
		WriteLogToList(kLogType, strTemp);
	}
	else if (tickCount < w_LastRealWriteTime)
	{
		w_LastRealWriteTime = tickCount;
	}

	//如果文件未打开
	if (w_fileLog.m_hFile == CFile::hFileNull )
	{
		DWORD dwRetFile = ::GetFileAttributes(w_strFile);
		if ( dwRetFile == INVALID_FILE_ATTRIBUTES || (dwRetFile & FILE_ATTRIBUTE_DIRECTORY) != 0) //文件不存在
		{
#ifdef MULTI_THREADS_LOG_MODE
			CSingleLock singleLock(&w_csLogLock, TRUE);
#endif

			if (!w_fileLog.Open(w_strFile, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite))
			{
				return false;
			}

#ifdef MULTI_THREADS_LOG_MODE
			singleLock.Unlock();
#endif

		}
		else//文件已经存在
		{

#ifdef MULTI_THREADS_LOG_MODE
			CSingleLock singleLock(&w_csLogLock, TRUE);
#endif

			if (!w_fileLog.Open(w_strFile, CFile::modeWrite | CFile::shareDenyWrite))
			{
				return false;
			}
			w_fileLog.SeekToEnd();

#ifdef MULTI_THREADS_LOG_MODE
			singleLock.Unlock();
#endif

		}

	}

//	if (kNormalLogType != kLogType)
	{
		//开始写入文件

		CStringA straTmp(strTemp);//转成ANSI，用于写文件

#ifdef MULTI_THREADS_LOG_MODE
		CSingleLock singleLock(&w_csLogLock, TRUE);
#endif

		// 如果文件大小大于10M，则将文件改名
		if (((w_fileLog.GetLength()) + straTmp.GetLength()) >= kvWiiLogFileMaxLength)
		{
			w_fileLog.Close();

			CString strNewFile;

			strNewFile = w_strFile;

			strNewFile.Insert(strNewFile.GetLength() - 4, _T("_") + CString(tm.Format(_T("%Y%m%d%H%M%S"))));

			if (!::MoveFile(w_strFile, strNewFile))
			{
				return false;
			}

			if (!w_fileLog.Open(w_strFile, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite))
			{
				return false;
			}
		}

		try
		{
			w_fileLog.Write(straTmp, straTmp.GetLength());
			w_fileLog.Flush();
		}
		catch (CFileException* pe)
		{
			pe->Delete();
			return false;
		}

#ifdef MULTI_THREADS_LOG_MODE
		singleLock.Unlock();
#endif
	}

	return true;
}


CWnd* CWiiLog::w_pMessageReceiver = NULL;

bool CWiiLog::WriteLogToList(WII_LOG_TYPE kLogType, const CString& strLogContent)
{
	if (!w_pMessageReceiver)
	{
		return false;
	}

	if (!w_pMessageReceiver->GetSafeHwnd() || strLogContent.IsEmpty())
	{
		return false;
	}

	CString *pStrLogContent = new CString(strLogContent);

	if (!::PostMessage(w_pMessageReceiver->GetSafeHwnd(), WM_WRITELOGTOTEXT, 
		WPARAM(kLogType), LPARAM(pStrLogContent)))
	{
		delete pStrLogContent;
		pStrLogContent = NULL;
		return false;
	}
	return true;
}
#endif