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

#include "LogEntry.h"

#include <wx/textfile.h>

#include "wxCommon.h"
using namespace wxCommon;

LogEntry::LogEntry(bool bFileOutput /*= true*/, 
				   wxString strLogFile /*= ""*/,
				   wxTextEntryBase* pTextCtrl /*= NULL*/, 
				   bool bClsOutput /*= false*/)
: m_bLog2File(true)
, m_strLogFileName(strLogFile)
, m_nMaxFileLength(10*1024*1024)	// 10M
, m_pTextCtrl(pTextCtrl)
, m_nMaxCtrlLength(0x7FFF)
, m_bLog2Cls(false)
, m_strTimeFormat("%Y-%m-%d %H:%M:%S")
, m_detailLevel(wxLOG_FatalError)
{
	// set default log file name
	if (!m_strLogFileName)
	{
		m_strLogFileName = wxString::Format("%s\\%s.log", GetExePath(), GetExeTitleBeforeUnderline());
	}
}

LogEntry::~LogEntry(void)
{
	m_fileLog.Close();
}


void LogEntry::DoLogRecord(wxLogLevel level,
						 const wxString& msg, 
						 const wxLogRecordInfo& info)
{
	// 2014-09-05 10:54:10 [Message] message string.[LINE TERMINATOR]
	wxString str;
	if (level <= m_detailLevel)
	{
		str += wxString::Format("%sDetail Log: Thread [%u] Function: [%s]%s  from file [%s] in line [%d]%s",
					 wxTextFile::GetEOL(), 
					 info.threadId, info.func, wxTextFile::GetEOL(), 
					 info.filename, info.line, wxTextFile::GetEOL());
	}
	str += wxString::Format("%s [%s] %s%s", 
									wxDateTime(info.timestamp).Format(m_strTimeFormat),
									GetLogString(level),
									msg,
									wxTextFile::GetEOL());

	// to text ctrl
	if (m_pTextCtrl)
	{
		m_pTextCtrl->AppendText(str);

		// avoid too much message
		int nLength = m_pTextCtrl->GetLastPosition();
		if (nLength >= m_nMaxCtrlLength)
		{
			m_pTextCtrl->Remove(0, m_nMaxCtrlLength / 2);
		}
	}

	// to file
	if (m_bLog2File)
	{
		if (!m_fileLog.IsOpened())
		{
			if (!m_fileLog.Open(m_strLogFileName, wxFile::write_append))
			{
				m_pTextCtrl->AppendText(wxString::Format("%s [Log Internal] can not open log file: %s.\n", 
											wxDateTime::Now().FormatISOCombined(' '), 
											m_strLogFileName));
			}		
		}
		if (m_fileLog.IsOpened())
		{
			if (m_fileLog.Length() + str.Length() > m_nMaxFileLength)
			{
				m_fileLog.Close();

				// new file name
				wxString strNewFileName = m_strLogFileName;
				strNewFileName.insert(m_strLogFileName.rfind('.'),
									wxDateTime::Now().Format("_%Y%m%d_%H%M%S"));
				if (!wxRenameFile(m_strLogFileName, strNewFileName))
				{
					m_pTextCtrl->AppendText(wxString::Format("%s [Log Internal] RenameFile failed from %s to %s\n", 
													wxDateTime::Now().FormatISOCombined(' '), 
													m_strLogFileName, strNewFileName));
					wxRemove(m_strLogFileName);
				}
				if (!m_fileLog.Open(m_strLogFileName, wxFile::write))
				{
					m_pTextCtrl->AppendText(wxString::Format("%s [Log Internal] can not open log file: %s.\n", 
													wxDateTime::Now().FormatISOCombined(' '), 
													m_strLogFileName));
				}
			}
		}
		if (m_fileLog.IsOpened())
		{
			m_fileLog.Write(str);
		}
	}

	if (m_bLog2Cls)
	{
		wxPrintf("%s", str);
	}
}

// set log file name
void LogEntry::SetLogFileName(const wxString& strFileName)
{
	if (strFileName != m_strLogFileName)
	{
		m_fileLog.Close();
		m_strLogFileName = strFileName;
	}
}
