/*--------------------------------------------------------------------------
<WtpLog.cpp> : <WtpLog.h>

����˵����������־����
��Ȩ���У��Ϻ�������Ϣ�Ƽ��ɷ����޹�˾

�汾��ʷ��
2001-2002��		��һ�壻				���ߣ�����ƽ����飺

----------------------------------------------------------------------------*/

//////////////////////////////////////////////////////////////////////////
// RealWrite��������logʵ����ʾ��д��
// if (tickCount -  w_LastRealWriteTime > 100) ��ʾ���log��ʾ�������λms
// if (kNormalLogType != kLogType) ��ʾֻд��ǡ���Ϣ��log

#include "StdAfx.h"
#include "WiiLog.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace std;

#ifdef  ENABLE_WII_LOG

// ȫ����
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

// �õ�ȫ��log�����ָ��
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


// ѡ��log�ļ�
bool CWiiLog::Select(const CString strFile, bool bToEnd)
{

	CFileException ex;
#ifdef MULTI_THREADS_LOG_MODE
	CSingleLock singleLock(&w_csLogLock, TRUE);
#endif

	Close();

	DWORD dwRetFile = ::GetFileAttributes(strFile);
	if ( dwRetFile == INVALID_FILE_ATTRIBUTES || (dwRetFile & FILE_ATTRIBUTE_DIRECTORY) != 0) //�ļ�������
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
	// �������Ӧ�ü�¼����־���ͣ���ֱ�ӷ���
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
	// �������Ӧ�ü�¼����־���ͣ���ֱ�ӷ���
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

	//����ļ�δ��
	if (w_fileLog.m_hFile == CFile::hFileNull )
	{
		DWORD dwRetFile = ::GetFileAttributes(w_strFile);
		if ( dwRetFile == INVALID_FILE_ATTRIBUTES || (dwRetFile & FILE_ATTRIBUTE_DIRECTORY) != 0) //�ļ�������
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
		else//�ļ��Ѿ�����
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
		//��ʼд���ļ�

		CStringA straTmp(strTemp);//ת��ANSI������д�ļ�

#ifdef MULTI_THREADS_LOG_MODE
		CSingleLock singleLock(&w_csLogLock, TRUE);
#endif

		// ����ļ���С����10M�����ļ�����
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