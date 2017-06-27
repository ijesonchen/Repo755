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


MainFrame::MainFrame(wxWindow *parent) : MainFrameBase( parent )
, m_pLogEntry(NULL)
{
	m_pLogEntry = new LogEntry(true, "", m_styledMain);
	wxLog::SetActiveTarget(m_pLogEntry);
	m_styledMain->SetWrapMode(SC_WRAP_WORD);
}

MainFrame::~MainFrame()
{
	wxLog::SetActiveTarget(NULL);
	delete m_pLogEntry;
}

void MainFrame::OnCloseFrame(wxCloseEvent& event)
{
	Destroy();
}

#include <wx/dir.h>
#include <deque>
#include <Winbase.h>

using namespace std;

class wxDirAndFiles : public wxDirTraverser
{
public:
	wxDirAndFiles(std::deque<wxString>& dqFiles, std::deque<wxString>& dqDirs)
		: m_dqFiles(dqFiles)
		, m_dqDirs(dqDirs)
	{
	}

	virtual wxDirTraverseResult OnFile(const wxString& filename)
	{
		m_dqFiles.push_back(filename);
		return wxDIR_CONTINUE;
	}
	virtual wxDirTraverseResult OnDir(const wxString& dirname)
	{
		m_dqDirs.push_back(dirname);
		return wxDIR_CONTINUE;
	}

	std::deque<wxString>& m_dqFiles;
	std::deque<wxString>& m_dqDirs;
};

int msFindFiles(const wxString szDir, deque<wxString>* pdqFiles, deque<wxString>* pdqDirs)
{
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError=0;

	wxString szFolder = szDir + "\\*.*";
	hFind = FindFirstFile(szFolder.wc_str(), &ffd);

	if (INVALID_HANDLE_VALUE == hFind) 
	{
		wxLogMessage("FindFirstFile INVALID_HANDLE_VALUE: %s", szFolder);
		return -1;
	} 

	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			wxString strFolder = ffd.cFileName;
			if (strFolder == "." ||
				strFolder == "..")
			{
				continue;
			}
			strFolder = szDir + "\\" + ffd.cFileName;
			if (pdqDirs)
			{
				pdqDirs->push_back(strFolder);
			}

			msFindFiles(strFolder.wc_str(), pdqFiles, pdqDirs);
		}
		else
		{
			if (pdqFiles)
			{
				wxString strFolder = szDir + "\\" + ffd.cFileName;
				pdqFiles->push_back(strFolder);
			}
		}
	}
	while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES) 
	{
		wxLogMessage("FindFirstFile ERROR_NO_MORE_FILES: %s", szFolder);
	}

	FindClose(hFind);

	return 0;
}

#include "..\wxCommon\wxCommon.h"

void MainFrame::toolTestOnToolClicked(wxCommandEvent& event)
{
	wxLogMessage("message");
	wxLogWarning("warning");
	wxLogError("error log");


	DWORD dw = 0;

	std::deque<wxString> dqFiles;
	std::deque<wxString> dqDirs;
	wxString dirName("C:\\WINDOWS\\system32");


	dw = GetTickCount();

	int nRet = msFindFiles(dirName.wc_str(), &dqFiles, &dqDirs);

	dw = GetTickCount() - dw;
	wxLogMessage("msFindFiles: %d, %u msecs, %u files, %u dirs", nRet, (unsigned)dw, dqFiles.size(), dqDirs.size());

	dqFiles.clear();
	dqDirs.clear();


	dw = GetTickCount();

	wxArrayString files;
	wxDir::GetAllFiles(dirName, &files);

	dw = GetTickCount() - dw;

	wxLogMessage("wxDir::GetAllFiles %u msecs, %u files.", (unsigned)dw, files.GetCount());


	dw = GetTickCount();
	wxDir dir(dirName);
	if ( !dir.IsOpened() )
	{
		// deal with the error here - wxDir would already log an error message
		// explaining the exact reason of the failure
	}
	else
	{
		wxDirAndFiles dnf(dqFiles, dqDirs);

		dir.Traverse(dnf);

		dw = GetTickCount() - dw;
		wxLogMessage("wxDir::Traverse %u msecs, %u files, %u dirs", (unsigned)dw, dqFiles.size(), dqDirs.size());

		dqFiles.clear();
		dqDirs.clear();
	}


	dw = GetTickCount();

	size_t uCount = wxCommon::FindFilesAndDirs(dirName, &dqFiles);

	dw = GetTickCount() - dw;
	wxLogMessage("FindFilesAndDirs: %u; %u msecs, %u files, %u dirs", uCount, (unsigned)dw, dqFiles.size(), dqDirs.size());


	dqFiles.clear();
	dqDirs.clear();






	bool bRet = false;
	bRet = wxFileName::DirExists("D:\\SvnRoot\\swap");
	bRet = wxFileName::FileExists("D:\\SvnRoot\\swap");

	bRet = wxFileName::DirExists("D:\\SvnRoot\\swap\\voiceedit.bmp");
	bRet = wxFileName::FileExists("D:\\SvnRoot\\swap\\voiceedit.bmp");
}