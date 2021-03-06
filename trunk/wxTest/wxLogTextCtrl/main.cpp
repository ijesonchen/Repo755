/*********************************************************************
 * Name:      	main.cpp
 * Purpose:   	Implements simple wxWidgets application with GUI
 * 				created using wxFormBuilder.
 * Author:    
 * Created:   
 * Copyright: 
 * License:   	wxWidgets license (www.wxwidgets.org)
 * 
 * Notes:		Note that all GUI creation code is implemented in
 * 				gui.cpp source file which is generated by wxFormBuilder.
 *********************************************************************/

#include "main.h"

#include "..\wxCommon\wxCommon.h"
using namespace wxCommon;

const int g_nMaxLogLength = 2000;

// initialize the application
IMPLEMENT_APP(MainApp);

////////////////////////////////////////////////////////////////////////////////
// application class implementation 
////////////////////////////////////////////////////////////////////////////////

bool MainApp::OnInit()
{
	SetTopWindow( new MainFrame( NULL ) );
	GetTopWindow()->Show();
	
	// true = enter the main loop
	return true;
}

////////////////////////////////////////////////////////////////////////////////
// main application frame implementation 
////////////////////////////////////////////////////////////////////////////////

// from $(WXPATH)\src\stc\scintilla\include\Scintilla.h
#define SC_WRAP_NONE 0
#define SC_WRAP_WORD 1
#define SC_WRAP_CHAR 2

#include <wx/textfile.h>
MainFrame::MainFrame(wxWindow *parent) : MainFrameBase( parent )
{

	const wxChar* psz =  wxTextFile::GetEOL();

	m_oldLogger = wxLog::GetActiveTarget();
	wxLog::SetActiveTarget(this);
	m_styleLog->SetWrapMode(SC_WRAP_WORD);

	wxString str = GetExeTitle();
	str = GetExeTitleBeforeUnderline();
}

MainFrame::~MainFrame()
{
	wxLog::SetActiveTarget(m_oldLogger);
}

void MainFrame::OnCloseFrame(wxCloseEvent& event)
{
	Destroy();
}

void MainFrame::toolBookMarkOnToolClicked(wxCommandEvent& event)
{
	wxLogMessage("test log message. test log message. test log message. test log message.");
}

void MainFrame::DoLogRecord(wxLogLevel level,
						 const wxString& msg,
						 const wxLogRecordInfo& info)
{
	// 2014-09-05 10:54:10 [Message] message string.
	wxString str = wxString::Format("%s [%s] %s\n", 
						wxDateTime(info.timestamp).Format("%Y-%m-%d %H:%M:%S"),
						GetLogString(level),
						msg);

	// to window
	m_styleLog->AppendText(str);

	// avoid too much message
	int nLength = m_styleLog->GetTextLength();
	if (nLength >= g_nMaxLogLength)
	{
		m_styleLog->Remove(0, g_nMaxLogLength / 2);
	}

	// to file
	if (!m_fileLog.IsOpened())
	{
		if (!m_fileLog.Open("d:\\log.txt", wxFile::write_append))
		{
			m_styleLog->AppendText("can not open log file.\r\n");
		}		
	}
	if (m_fileLog.IsOpened())
	{
		m_fileLog.Write(str);
	}
}
