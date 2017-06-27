
#include <wx/app.h>
#include "ConfigIni.h"
#include <wx/stdpaths.h>

ConfigIni::ConfigIni(void)
{
}

ConfigIni::~ConfigIni(void)
{
}


bool ConfigIni::SetIniName( wxString* pIniName ) 
{
	if (pIniName && pIniName->length())
	{
		strIniName = *pIniName;
	}
	else
	{
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
		strIniName = wxString::Format("%s.ini", strAppName);

		wxString strAppPath = wxStandardPaths::Get().GetDataDir();
		if (!strAppPath.IsEmpty())
		{
			strIniName = strAppPath + "\\" + strIniName;
		}
	}

	if (!wxFile::Exists(strIniName))
	{
		return false;
	}

	return true;
}


void ConfigIni::LoadDefault(void)
{
	dqItems.push_back(Item(0, "testdb", "D:\\data\\sqlite\\wxtest.db"));
}

int ConfigIni::ReadIni(wxString*  pIniName /*= NULL*/)
{
	if (!SetIniName(pIniName))
	{
		// ini file not exist
		LoadDefault();
		return 1;
	}

	wxFileConfig cfg(wxEmptyString, wxEmptyString, strIniName, wxEmptyString, wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH | wxCONFIG_USE_NO_ESCAPE_CHARACTERS);
	cfg.SetExpandEnvVars(false);

	return 0;
}

// void ConfigIni::ReadAttrEntries(wxFileConfig& cfg, const wxString basePath, std::deque<AttrItem>& dqAttrs)
// {
// 	wxString oldPath = cfg.GetPath();
// 	cfg.SetPath(basePath);
// 
// 	dqAttrs.clear();
// 
// 	long nIdx = 0;
// 	wxString strEntry;
// 	std::deque<wxString> dqEntries;
// 	if (cfg.GetFirstGroup(strEntry, nIdx))
// 	{
// 		dqEntries.push_back(strEntry);
// 		while (cfg.GetNextGroup(strEntry, nIdx))
// 		{
// 			dqEntries.push_back(strEntry);
// 		}
// 	}
// 
// 	for (size_t ii = 0, iLoop = dqEntries.size();
// 		ii < iLoop; ++ii)
// 	{
// 		cfg.SetPath(dqEntries[ii]);
// 
// 		AttrItem attr;
// 		attr.path =	cfg.Read("Path");
// 		attr.attrName = cfg.Read("AttrName");
// 		attr.attrDefault = cfg.Read("AttrDefault");
// 		if (attr.path.length() || attr.attrName.length() || attr.attrDefault.length())
// 		{
// 			dqAttrs.push_back(attr);
// 		}
// 		cfg.SetPath("..");
// 	}
// 
// 	cfg.SetPath(oldPath);
// }
// 
// 
int ConfigIni::SaveIni(wxString*  pIniName /*= NULL*/)
{
	// not finished.
	return -1;
}


ConfigIni::Item ConfigIni::GetItem(const wxTreeItemId treeid)
{
	for (size_t ii = 0, ni = dqItems.size();
		ii < ni; ++ii)
	{
		if (dqItems[ii].treeid == treeid)
		{
			return dqItems[ii];
		}
	}
	return Item(-1, "", "");
}