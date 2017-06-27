#ifndef wxCommon_h__
#define wxCommon_h__

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

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/aui/framemanager.h>
#include <wx/dataview.h>
#include <deque>


namespace wxCommon
{
	//////////////////////////////////////////////////////////////////////////
	// test functions
	void TestTemp(void);


	//////////////////////////////////////////////////////////////////////////
	// UI related

	// force pane to show in best_size once, when use wxAUI
	void ForceAuiBestSizeOnce(wxAuiManager& auiManager);

	//////////////////////////////////////////////////////////////////////////
	// GetIniFileName: prog_Debug_Win32.exe -> prog.ini
	wxString GetIniFileName(void);


	//////////////////////////////////////////////////////////////////////////
	// string process

	// find string in deque, -1 if not found
	int FindStringInDeque(const std::deque<wxString> dqString, const wxString str);


	// split strSrc with splitter, store in container cntrStr.
	// cntrStr must support push_back() method. splitter can be either char or string
	template<class CntrType, class TextType>
	void SpliteString(CntrType& cntrStr, const wxString& strSrc, const TextType& splitter)
	{
		size_t nPos1 = 0;
		size_t nPos2 = 0;

		int nPosAdjust = sizeof(splitter);
		if (nPosAdjust != 1)
		{
			// splitter is string
			--nPosAdjust;
		}

		if (strSrc.find(splitter, nPos1))
		{
			// if string not leading by splitter, first nPos1 can be 0
			nPos2 = - nPosAdjust;
		}

		do 
		{
			nPos1 = nPos2 + nPosAdjust;
			if (nPos1 >= strSrc.length())
			{
				break;
			}
			nPos2 = strSrc.find(splitter, nPos1);
			cntrStr.push_back(strSrc.Mid(nPos1, nPos2 - nPos1));
		} while (nPos2 != wxString::npos);
	}

	//////////////////////////////////////////////////////////////////////////
	// set DataViewListCtrl header
	class ListHeader 
	{
	public:
		bool isToggle;
		wxString label;
		wxDataViewCellMode mode;
		int width;
		bool isSortble;
	};

	void SetDvlistHeader(wxDataViewListCtrl* pCtrl, const ListHeader* pdvlistHeaderConfig, const int nColumn);
	void SetDvlistHeader(wxDataViewListCtrl* pCtrl, const std::deque<std::string>& dqLabels,
		bool toggle = false, 
		wxDataViewCellMode m = wxDATAVIEW_CELL_EDITABLE, 
		int w = wxCOL_WIDTH_AUTOSIZE, 
		bool sortble = true);


	//////////////////////////////////////////////////////////////////////////
	// get wxLogLevel string
	wxString GetLogString(wxLogLevel level);
	wxString GetExePath(void);
	wxString GetExeTitle(void);
	wxString GetExeTitleBeforeUnderline(void);


	//////////////////////////////////////////////////////////////////////////
	// file handling

	// find all files and folders in strSrcFolder, return the number of files found
	// wxDir::GetAllFiles is very low efficient. 250 msecs (GetAllFiles) v.s. 79 msecs (FindFilesAndDirs) (6162 files, 266 dirs, cached).
	size_t FindFilesAndDirs(wxString strSrcFolder, std::deque<wxString>* pdqFiles, std::deque<wxString>* pdqFolders = NULL);

	// act like FindFilesAndDirs, but with MSW functions, more efficient (30% to 100% up)
	int MSWFindFiles(const wxString strSrcFolder, std::deque<wxString>* pdqFiles, std::deque<wxString>* pdqDirs = NULL);

}


#endif // wxXmlCommon_h__
