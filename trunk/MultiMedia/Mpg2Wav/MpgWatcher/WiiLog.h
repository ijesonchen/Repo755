
//����˵����������־����

#pragma once


/* NOTE:
 * (1) ������һ������ֻ��һ��ģ����ʹ��DECLARE_WTP_LOG��OPEN_WTP_LOG�����У�
 *     DECLARE_WTP_LOG������һ�������LOG�ļ�����ʹ��ǰ�������SELECT_WTP_LOG��һ�������log�ļ�
 *     ���ģ���Ϊ����־ģ�飻
 * (2) ��ÿ��ģ��� 
 *        #include "WtpLog.h" 
 *     ǰʹ��	
 *        #define DISABLE_WTP_LOG
 *     ���Թرձ�ģ�����־���ܣ����ǲ���Ӧ��������־ģ�飻
 * (3) ��WtpLog.h�е�
 *			#define DISABLE_WTP_LOG_IN_ALL_MODULES
 *     ���ر�����ģ�����־����
 * (4) �򿪻�ر�WtpLog.h�е�
 *          #define MULTI_THREADS_MODE
 *     ���Դ򿪻�رն��߳�ģʽ��
 *
 * (5) ����ĺ궨��
 *     DECLARE_WTP_LOG��				������־���ܣ�Ӧ��������־ģ��
 *     OPEN_WTP_LOG(strFile, bApp)��	��������һ����־�ļ���bApp��ʾ�Ƿ����������ģʽ
 *     SELECT_WTP_LOG(strFile, bApp):	�򿪻����´�һ����־�ļ�
 *     WtpLog(strFormat, ...):			�����־
 *     GET_WTP_LOG��					���ص�ǰ����־�ļ��������������ǣ�const char*������
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

// ��������
// log�ļ�����С
const long kvWiiLogFileMaxLength = (long)1024 * 1024 * 10;
// log�ļ���һ����¼��С
const int  kvWiiLogMaxBuffer = 10240; 

// ��־����
enum WII_LOG_TYPE
{
	kDebugLogType = 1,		// ����Log
	kWarningLogType = 2,	// ����Log
	kErrorLogType = 4,		// ����Log
	kNormalLogType = 8,		// ����LOG
	kStatistLogType = 16,	// ͳ��LOG
	kSystemLogType = 32,	// ϵͳLOG
};

inline CString GetWiiLogTypeCString(WII_LOG_TYPE kLogType)
{
	switch (kLogType)
	{
		case kDebugLogType:
			return _T("����");
		case kWarningLogType:
			return _T("����");
		case kErrorLogType:
			return _T("����");
		case kNormalLogType:
			return _T("��Ϣ");
		case kStatistLogType:
			return _T("ͳ��");
		case kSystemLogType:
			return _T("ϵͳ");
		default:
			return _T("");
	}
}

#define WM_WRITELOGTOTEXT WM_USER + 3333
// ��־��
class CWiiLog
{
private:
	CWiiLog();
	//CWiiLog(const char *strFile,  bool bApp, FUNCTIONPTR pFunctionPtr = NULL); // bApp for append log

public:
	// ��������
	~CWiiLog();

	static void SetMsgReceiver(CWnd *pWnd){ w_pMessageReceiver = pWnd; }
	bool WriteLogToList(WII_LOG_TYPE kLogType, const CString& strLogContent);

	bool IsOpen()
	{
		return w_fileLog.m_hFile != CFile::hFileNull;
	}

	void Close();
	
	bool Select(const CString strFile, bool bToEnd);
	
	// ������־����
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

	// �Ƿ�������ĳ����־����
	inline bool IsLoggedType(const WII_LOG_TYPE kLogType)
	{
		return (kLogType & w_nLogTypeMask) > 0 ? true : false;
	};

	// �����־�����ָ��
	static CWiiLog* GetLogHandle();
	
	// д��־
	bool WriteLog(WII_LOG_TYPE kLogType, const TCHAR *strFormat, ...);

	bool WriteLog(const TCHAR *strFormat,...);
	
	// ���log�ļ�������
	const CString& GetFile() const;

protected:

	bool RealWrite(WII_LOG_TYPE kLogType, const TCHAR* strLog);


protected:
	// ��־�ļ�����
	CString w_strFile;
	// ��־�����
	CFile w_fileLog;
	// �������־����
	int	w_nLogTypeMask;
	// �Զ������־ͷ
	CString w_strLogHead;
	// �Ƿ���ʾ��־ʱ��
	bool w_bShowTime;
	// �Ƿ���ʾ��־����
	bool w_bShowLogType;
	// �Ƿ���ûص�
	bool w_bUseCallback;

	static CWnd* w_pMessageReceiver;

	static CWiiLog __g_wii_log__;

	DWORD w_LastRealWriteTime;


#ifdef MULTI_THREADS_LOG_MODE
	CCriticalSection w_csLogLock;
#endif
};

#endif // ]  ENABLE_WII_LOG


