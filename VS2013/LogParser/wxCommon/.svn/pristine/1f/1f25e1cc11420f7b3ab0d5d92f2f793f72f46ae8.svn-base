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

#include "wxCommon.h"
#include <wx/dir.h>
#include <wx/app.h>
#include <wx/stdpaths.h>
#include <wx/fileconf.h>

#include <iostream>
#include <deque>
#include <vector>
using namespace std;

namespace wxCommon
{


// force pane to show in best_size once, when use wxAUI
void ForceAuiBestSizeOnce(wxAuiManager& auiManager)
{
	wxAuiPaneInfoArray& panes = auiManager.GetAllPanes();
	deque<wxSize> dqSizes;
	for (size_t i = 0, pane_count = panes.GetCount(); i < pane_count; ++i)
	{
		wxAuiPaneInfo& pane = panes.Item(i);
		dqSizes.push_back(pane.min_size);
		pane.min_size = pane.best_size;
	}
	auiManager.Update();
	for (size_t i = 0, pane_count = panes.GetCount(); i < pane_count; ++i)
	{
		wxAuiPaneInfo& pane = panes.Item(i);
		pane.min_size = dqSizes[i];
	}
	auiManager.Update();
}


//////////////////////////////////////////////////////////////////////////
// GetIniFileName: prog_Debug_Win32.exe -> prog.ini

wxString GetIniFileName(void)
{
	wxString strIniName;

	wxString strAppName;
	if (wxTheApp)
	{
		strAppName = wxTheApp->GetAppName();
		if (strAppName.IsEmpty())
		{
			strAppName = "config";
		}
		else
		{
			strAppName = strAppName.Left(strAppName.Find('_'));
		}
	}

	if (!strAppName.length())
	{
		return "config.ini";
	}

	strIniName = wxString::Format("%s.ini", strAppName);

	wxString strAppPath = wxStandardPaths::Get().GetDataDir();
	if (strAppPath.length())
	{
		if (!strAppPath.IsEmpty())
		{
			strIniName = strAppPath + "\\" + strIniName;
		}
	}

	return strIniName;
}

//////////////////////////////////////////////////////////////////////////
// string process

// find string in deque, -1 if not found
int FindStringInDeque(const std::deque<wxString> dqString, const wxString str)
{
	for (size_t ii = 0, iLoop = dqString.size(); ii < iLoop; ++ii)
	{
		if (str == dqString[ii])
		{
			return (int)ii;
		}
	}
	return -1;
}

//////////////////////////////////////////////////////////////////////////
// set DataViewListCtrl header
void SetDvlistHeader(wxDataViewListCtrl* pCtrl, const ListHeader* pdvlistHeaderConfig, const int nColumn)
{
	for (int ii = 0; ii < nColumn; ++ii)
	{
		const ListHeader& header = pdvlistHeaderConfig[ii];
		int flag = wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE;
		if (header.isSortble)
		{
			flag |= wxDATAVIEW_COL_SORTABLE;
		}
		if (header.isToggle)
		{
			pCtrl->AppendToggleColumn(header.label, header.mode, header.width, 
				wxALIGN_LEFT, flag);
		}
		else
		{
			pCtrl->AppendTextColumn(header.label, header.mode, header.width,
				wxALIGN_LEFT, flag);
		}
	}
}

void SetDvlistHeader(wxDataViewListCtrl* pCtrl, const std::deque<std::string>& dqLabels,
					 bool toggle /*= false*/, 
					 wxDataViewCellMode m /*= wxDATAVIEW_CELL_EDITABLE*/, 
					 int w /*= wxCOL_WIDTH_AUTOSIZE*/, 
					 bool sortble /*= true*/)
{
	size_t nLabel = dqLabels.size();
	ListHeader* phdrs = new ListHeader[nLabel];
	for (size_t ii = 0; ii < nLabel; ++ii)
	{
		phdrs[ii].isToggle = toggle;
		phdrs[ii].label = dqLabels[ii];
		phdrs[ii].mode = m;
		phdrs[ii].width = w;
		phdrs[ii].isSortble = sortble;
	}

	SetDvlistHeader(pCtrl, phdrs, nLabel);
	delete[] phdrs;
}

//////////////////////////////////////////////////////////////////////////
// get wxLogLevel string
wxString GetLogString(wxLogLevel level)
{
	switch (level)
	{
	case wxLOG_FatalError: // program can't continue, abort immediately
		return "Fatal";
		break;

	case wxLOG_Error:      // a serious error, user must be informed about 
		return "Error";
		break;

	case wxLOG_Warning:    // user is normally informed about it but may be ignored
		return "Warning";
		break;

	case wxLOG_Message:    // normal message (i.e. normal output of a non GUI app)
		return "Message";
		break;

	case wxLOG_Status:     // informational: might go to the status line of GUI app
		return "Status";
		break;

	case wxLOG_Info:       // informational message (a.k.a. 'Verbose')
		return "Info";
		break;

	case wxLOG_Debug:      // never shown to the user, disabled in release mode
		return "Debug";
		break;

	case wxLOG_Trace:      // trace messages are also only enabled in debug mode
		return "Trace";
		break;

	case wxLOG_Progress:
		return "Progress";
		break;

	default :
		return "Undefined";
		break;
	}
}

wxString GetExePath(void)
{
	wxString str;
	if (wxTheApp)
	{
		str = wxTheApp->argv[0];

		size_t nPos = str.rfind('\\');
		if (wxString::npos != nPos)
		{
			str = str.Left(nPos);
		}
	}
	return str;
}


wxString GetExeTitle(void)
{
	wxString str;
	if (wxTheApp)
	{
		str = wxTheApp->argv[0];

		size_t nPos = 0;
		nPos = str.rfind('\\');
		if (wxString::npos != nPos)
		{
			str.Remove(0, nPos + 1);
		}
		nPos = str.rfind('.');
		if (wxString::npos != nPos)
		{
			str = str.Left(nPos);
		}
	}
	return str;
}

wxString GetExeTitleBeforeUnderline(void)
{
	wxString str = GetExeTitle();
	size_t nPos = str.find('_');
	if (wxString::npos != nPos)
	{
		str = str.Left(nPos);
	}
	return str;
}

// Traverser class for FindFilesAndDirs function.
class wxDirAndFiles : public wxDirTraverser
{
public:
	wxDirAndFiles(std::deque<wxString>* pdqFiles, std::deque<wxString>* pdqDirs)
		: m_pdqFiles(pdqFiles)
		, m_pdqDirs(pdqDirs)
	{
	}

	virtual wxDirTraverseResult OnFile(const wxString& filename)
	{
		if (m_pdqFiles)
		{
			m_pdqFiles->push_back(filename);
		}
		return wxDIR_CONTINUE;
	}
	virtual wxDirTraverseResult OnDir(const wxString& dirname)
	{
		if (m_pdqDirs)
		{
			m_pdqDirs->push_back(dirname);
		}
		return wxDIR_CONTINUE;
	}

	std::deque<wxString>* m_pdqFiles;
	std::deque<wxString>* m_pdqDirs;
};

// find all files and folders in strSrcFolder, return the number of files found
size_t FindFilesAndDirs(wxString strSrcFolder, std::deque<wxString>* pdqFiles, std::deque<wxString>* pdqDirs)
{
	wxDir dir(strSrcFolder);
	if ( !dir.IsOpened() )
	{
		// deal with the error here - wxDir would already log an error message
		// explaining the exact reason of the failure
		return 0;
	}

	wxDirAndFiles dnf(pdqFiles, pdqDirs);

	return dir.Traverse(dnf);
}


// act like FindFilesAndDirs, but use MSW functions, more efficient (30% to 100% up)
int MSWFindFiles(const wxString strSrcFolder, deque<wxString>* pdqFiles, deque<wxString>* pdqDirs)
{
#if defined(__WINDOWS__)

	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError=0;

	wxString szFolder = strSrcFolder + "\\*.*";
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
			strFolder = strSrcFolder + "\\" + ffd.cFileName;
			if (pdqDirs)
			{
				pdqDirs->push_back(strFolder);
			}

			MSWFindFiles(strFolder.wc_str(), pdqFiles, pdqDirs);
		}
		else
		{
			if (pdqFiles)
			{
				wxString strFolder = strSrcFolder + "\\" + ffd.cFileName;
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

#else

	FindFilesAndDirs(strSrcFolder, pdqFiles, pdqDirs);
	return 0;

#endif // __WINDOWS__

}

} // namespace wxXmlCommon