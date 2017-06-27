#include "main.h"

// initialize the application
IMPLEMENT_APP(MainApp);

/************************************************************************/
/*                       wxFileDropTarget BEGIN                         */
/************************************************************************/
#include <wx/dnd.h>

class MainFrame;

class MyFileDropTarget : public wxFileDropTarget
{
public:
	MyFileDropTarget(MainFrame* pFrame)
		: wxFileDropTarget()
		, m_pFrame(pFrame)
	{
	}

protected:
	MainFrame* m_pFrame;

	virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &filenames)
	{
		m_pFrame->ClearLogs();

		wxLongLong usec1 = wxGetUTCTimeMillis();
		
		for (size_t ii = 0, ni = filenames.GetCount(); ii < ni; ++ii)
		{
			m_pFrame->m_strFileName = filenames[ii];
			m_pFrame->LoadFile(m_pFrame->m_strFileName);
		}

		wxLongLong usec2 = wxGetUTCTimeMillis();
		m_pFrame->FillCtrl();
		wxLongLong usec3 = wxGetUTCTimeMillis();

		wxString wxMsg = 
			wxString::Format("time cost: %d %d", (usec2 - usec1).ToLong(), (usec3 - usec2).ToLong());
		m_pFrame->m_statusBar->SetStatusText(wxMsg);
		return true;
	}
};

/************************************************************************/
/*                         wxFileDropTarget END                         */
/************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// application class implementation 
////////////////////////////////////////////////////////////////////////////////

bool MainApp::OnInit()
{
	SetTopWindow(new MainFrame(NULL));
	GetTopWindow()->Show();

	// true = enter the main loop
	return true;
}

////////////////////////////////////////////////////////////////////////////////
// main application frame implementation 
////////////////////////////////////////////////////////////////////////////////

#include "..\wxCommon\wxCommon.h"
#include <wx/time.h>
#include <wx/fileconf.h>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "LogMsg.h"

using namespace std;

const int knMaxCtrlLogCount = 1000;

static const wxCommon::ListHeader gdvhdrLogs[] =
{
	{ 0, "D.T.", wxDATAVIEW_CELL_ACTIVATABLE, wxCOL_WIDTH_AUTOSIZE, 1 },
	{ 0, "Cat", wxDATAVIEW_CELL_ACTIVATABLE, wxCOL_WIDTH_AUTOSIZE, 1 },
	{ 0, "Tag", wxDATAVIEW_CELL_ACTIVATABLE, wxCOL_WIDTH_AUTOSIZE, 1 },
	{ 0, "Msg", wxDATAVIEW_CELL_EDITABLE, wxCOL_WIDTH_AUTOSIZE, 1 },
};

MainFrame::MainFrame( wxWindow* parent )
:
MainFrameBase( parent )
{
	wxCommon::SetDvlistHeader(m_dvLogs, gdvhdrLogs,
		sizeof(gdvhdrLogs) / sizeof(wxCommon::ListHeader));

	SetDropTarget(new MyFileDropTarget(this));

	LoadCfg();
}


int MainFrame::LoadFile(const wxString& wxstrFileName)
{
	if (!wxstrFileName.length())
	{
		return -1;
	}
	m_textFilename->SetValue(wxstrFileName);
	string strFileName = wxstrFileName.ToAscii();

	ifstream ifs;
	ifs.open(strFileName, ios_base::in);

	if (!ifs)
	{
		return -2;
	}

	SaveCfg();

	string strLine;
	vector<string> vtLines;
	
	while (getline(ifs, strLine))
	{
		if (!strLine.length())
		{
			continue;
		}
		if (LogMsg::CheckLogLeadin(strLine))
		{
			PushLogMsg(vtLines);
		}
		vtLines.push_back(strLine);
	}
	PushLogMsg(vtLines);	

	return 0;
}


bool MainFrame::FillCtrl(void)
{
	ClearCtrl();
	FillLogs();
	FillTags();
	FillCats();
	
	return false;
}

void MainFrame::FillCats()
{
	wxArrayString cats;
	for (auto itbeg = m_usetCats.cbegin();
		itbeg != m_usetCats.cend();
		++itbeg)
	{
		cats.Add(*itbeg);
	}
	m_ckListCategory->InsertItems(cats, 0);

	for (int ii = 0, ni = m_ckListCategory->GetCount();
		ii < ni; ++ii)
	{
		m_ckListCategory->Check(ii, true);
	}
}

void MainFrame::FillTags()
{
	wxArrayString tags;
	for (auto itbeg = m_usetTags.cbegin();
		itbeg != m_usetTags.cend();
		++itbeg)
	{
		tags.Add(*itbeg);
	}
	m_ckListTag->InsertItems(tags, 0);

	for (int ii = 0, ni = m_ckListTag->GetCount();
		ii < ni; ++ii)
	{
		m_ckListTag->Check(ii, true);
	}
}

void MainFrame::FillLogs()
{
	int iend = (int)m_vtLogs.size() - 1;
	int ibeg = iend - knMaxCtrlLogCount;

	int ni = (ibeg < 0) ? 0 : ibeg;

	int ipos = -1;
	for (int ii = iend; ii >= ni; --ii)
	{
		m_LogQuery.push_back(m_vtLogs[ii]);

		wxVector<wxVariant> vtItem;
		vtItem.push_back(m_vtLogs[ii]->dateTime);
		vtItem.push_back(m_vtLogs[ii]->category);
		vtItem.push_back(m_vtLogs[ii]->tag);
		vtItem.push_back(m_vtLogs[ii]->msg);
		m_dvLogs->InsertItem(++ipos, vtItem);
	}
}

void MainFrame::m_toolReloadOnToolClicked(wxCommandEvent& event)
{
	if (!m_strFileName.length())
	{
		return;
	}
	unordered_set<string> usetTags = GetSelTags();

	ClearLogs();
	LoadFile(m_strFileName);

	FillTags();
	FillCats();

	if (usetTags.size())
	{
		for (int ii = 0, ni = m_ckListTag->GetCount();
			ii < ni; ++ii)
		{
			wxString str = m_ckListTag->GetString(ii);
			if (usetTags.find(str.ToStdString()) == usetTags.end())
			{
				m_ckListTag->Check(ii, false);
			}
		}
	}

	m_toolFilterOnToolClicked(wxCommandEvent());
}

void MainFrame::m_toolFilterOnToolClicked(wxCommandEvent& event)
{
	auto usetSelTags = GetSelTags();
	auto userSelCats = GetSelCats();

	m_dvLogs->DeleteAllItems();

	int pgcnt = knMaxCtrlLogCount;

	int iend = (int)m_vtLogs.size() - 1;
	int ibeg = iend - pgcnt;

	int ni = (ibeg < 0) ? 0 : ibeg;

	int ipos = -1;
	for (size_t ii = iend; ii && ipos < pgcnt; --ii)
	{
		if (userSelCats.find(m_vtLogs[ii]->category) == userSelCats.cend())
		{
			continue;
		}
		if (usetSelTags.find(m_vtLogs[ii]->tag) == usetSelTags.cend())
		{
			continue;
		}
		m_LogQuery.push_back(m_vtLogs[ii]);

		wxVector<wxVariant> vtItem;
		vtItem.push_back(m_vtLogs[ii]->dateTime);
		vtItem.push_back(m_vtLogs[ii]->category);
		vtItem.push_back(m_vtLogs[ii]->tag);
		vtItem.push_back(m_vtLogs[ii]->msg);
		m_dvLogs->InsertItem(++ipos, vtItem);
	}
}

std::unordered_set<std::string> MainFrame::GetSelCats()
{
	wxArrayInt naSels;
	int nSels = m_ckListCategory->GetCheckedItems(naSels);

	std::unordered_set<std::string> usetSels;
	for (int ii = 0; ii < nSels; ++ii)
	{
		wxString str = m_ckListCategory->GetString(naSels[ii]);
		usetSels.insert(str.ToStdString());
	}
	return usetSels;
}

std::unordered_set<std::string> MainFrame::GetSelTags()
{
	wxArrayInt naSels;
	int nSels = m_ckListTag->GetCheckedItems(naSels);

	std::unordered_set<std::string> usetSels;
	for (int ii = 0; ii < nSels; ++ii)
	{
		wxString str = m_ckListTag->GetString(naSels[ii]);
		usetSels.insert(str.ToStdString());
	}
	return usetSels;
}

void MainFrame::m_buttonSearchOnButtonClick(wxCommandEvent& event)
{
	string strFind = m_textSearch->GetValue().ToStdString();
	int nPos = m_LogQuery.Find(strFind);
	if (nPos != -1)
	{
		m_dvLogs->SelectRow(nPos);
	}
}

void MainFrame::m_checkAutoRefreshOnCheckBox(wxCommandEvent& event)
{
	bool bCheck = m_checkAutoRefresh->IsChecked();
	if (bCheck)
	{
		int nSec = m_spinRefreshSecs->GetValue();
		if (nSec < 1)
		{
			nSec = 1;
		}
		m_timerRefresh.Start(nSec * 1000);
		m_statusBar->SetStatusText(wxString::Format("Timer set every %d secs.", nSec));
	}
	else
	{
		m_timerRefresh.Stop();
		m_statusBar->SetStatusText(wxString::Format("Timer stopped."));
	}
}

void MainFrame::m_checkAllTagOnCheckBox(wxCommandEvent& event)
{
	bool bCheck = m_checkAllTag->IsChecked();
	for (unsigned ii = 0, ni = m_ckListTag->GetCount();
		ii < ni; ++ii)
	{
		m_ckListTag->Check(ii, bCheck);
	}
}


void MainFrame::m_timerRefreshOnTimer(wxTimerEvent& event)
{
	m_statusBar->SetStatusText(wxString::Format("Auto refresh at %s", wxDateTime::Now().Format()));
	m_toolReloadOnToolClicked(wxCommandEvent());
}


void MainFrame::LoadCfg()
{
	wxString strIniFile = wxCommon::GetIniFileName();
	if (!wxFile::Exists(strIniFile))
	{
		return;
	}
	wxFileConfig cfg(wxEmptyString, wxEmptyString, strIniFile, wxEmptyString, wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH | wxCONFIG_USE_NO_ESCAPE_CHARACTERS);

	wxString strLogFile = cfg.Read("LogFile");
	if (strLogFile.length())
	{
		m_strFileName = strLogFile;
		m_textFilename->SetValue(m_strFileName);
	}
}


void MainFrame::SaveCfg()
{
	if (!m_strFileName.length())
	{
		return;
	}
	wxString strIniFile = wxCommon::GetIniFileName();
	wxFileConfig cfg(wxEmptyString, wxEmptyString, strIniFile, wxEmptyString, wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH | wxCONFIG_USE_NO_ESCAPE_CHARACTERS);

	cfg.Write("LogFile", m_strFileName);
}
