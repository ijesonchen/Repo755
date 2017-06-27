#ifndef LogEntry_h__
#define LogEntry_h__


/************************************************************************

Original code by Jeson Chen (jesonsara@gmail.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.

************************************************************************/


/************************************************************************

Purpose: 
log message in file, text control and console.

Usage:
1. add member to MainFrame: LogEntry* m_pLogEntry
2. in MainFrame():
	m_pLogEntry = new LogEntry();
	wxLog::SetActiveTarget(m_pLogEntry);
	wxLogMessage("Log Inited.");
3. in ~MainFrame():
 	wxLog::SetActiveTarget(NULL);
 	delete m_pLogEntry;
4. use wxLogXXX function to log.

Note:
1. since wxWidgets 2.9.1, wxLogXXX functions can used in multi-threads.
2. log can be combined in file, text controls and console (wxPrintf)
   this can set in constructor or use separate functions.
3. all text controls derived from wxTextEntryBase is supported:
     wxTextCtrl, wxStyledTextCtrl, wxRichTextStyledComboCtrl etc.
4. for wxStyledTextCtrl, use SetWrapMode to wrap word.
	// from $(WXPATH)\src\stc\scintilla\include\Scintilla.h
	#define SC_WRAP_NONE 0
	#define SC_WRAP_WORD 1
	#define SC_WRAP_CHAR 2
5. auto rename log file when exceeded size m_nMaxFileLength.
   old log will renamed to $(LogFileTile)_YYYYmmdd_HHMMSS.log
6. when log level <= m_detailLevel, wxLogRecordInfo will be logged.
   default detail level is wxLOG_FatalError, so no wxLogRecordInfo will logged. 
7. if log file enabled and no filename set, use default log file name:
   $(ExePath)\$(ExeTile).log
   if $(ExeTile) has underline, rest chars will be eliminated.
   eg. 
   exe file: d:\bin\test_debug.exe
   log file: d:\bin\test.log

************************************************************************/


#include <wx/log.h>
#include <wx/textentry.h>
#include <wx/string.h>
#include <wx/file.h>

class LogEntry :
	public wxLog
{
public:
	LogEntry(bool bFileOutput = true, wxString strLogFile = "", wxTextEntryBase* pTextCtrl = NULL, bool bClsOutput = false);
	~LogEntry(void);

protected:
	// weather and where log file output.
	bool m_bLog2File;
	// if empty, log to default file name.
	wxString m_strLogFileName;
	wxFile m_fileLog;
	long m_nMaxFileLength;

	// text control for log. NULL if no ctrl output.
	wxTextEntryBase* m_pTextCtrl;
	// max string length of text control
	long m_nMaxCtrlLength;

	// weather console output
	bool m_bLog2Cls;

	// time format string
	wxString m_strTimeFormat;
	// when log level <= m_detailLevel, wxLogRecordInfo will be logged.
	wxLogLevel m_detailLevel;

	virtual void DoLogRecord(wxLogLevel level,
		const wxString& msg, 
		const wxLogRecordInfo& info);
public:
	// weather lot to file
	void SetLog2File(const bool bLog2File)
	{
		m_bLog2File = bLog2File;
	}

	// set log file name
	void SetLogFileName(const wxString& strFileName);

	// set max log file length. auto rename if exceeded.
	void SetMaxFileLength(long nMaxFileLength)
	{
		m_nMaxFileLength = nMaxFileLength;
	}

	// Set text control for log. NULL to disable
	void SetTextCtrl(wxTextEntryBase* pTextCtrl)
	{
		m_pTextCtrl = pTextCtrl;
	}

	// set max length of text control
	void SetMaxCtrlLength(long nMaxCtrlLength)
	{
		m_nMaxCtrlLength = nMaxCtrlLength;
	}

	// weather log to console
	void SetConsoleOutput(bool bLog2Cls)
	{
		m_bLog2Cls = bLog2Cls;
	}

	// set time format string
	bool SetTimeFormat(wxString& strTimeFormat)
	{
		m_strTimeFormat = strTimeFormat;
	}

	void SetDetailLevel(wxLogLevel detailLevel)
	{
		m_detailLevel = detailLevel;
	}
	
	wxLogLevel GetDetailLevel(void)
	{
		return m_detailLevel;
	}

};

#endif // LogEntry_h__
