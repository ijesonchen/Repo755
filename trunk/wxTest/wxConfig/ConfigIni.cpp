#include "ConfigIni.h"
#include <wx/app.h>
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

int ConfigIni::ReadIni(wxString*  pIniName /*= NULL*/)
{
	if (SetIniName(pIniName))
	{
		// ini file not exist
		return -1;
	}

	wxFileConfig cfg(wxEmptyString, wxEmptyString, strIniName, wxEmptyString, wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH | wxCONFIG_USE_NO_ESCAPE_CHARACTERS);
	cfg.SetExpandEnvVars(false);

	// main
	cfg.SetPath("/MAIN");
	basePath = cfg.Read("BasePath");
	attrName = cfg.Read("AttrName");
	selectCount = cfg.Read("SelectCount");

	// main/item[n]
	ReadAttrEntries(cfg, "/Main", dqMainAttrs);

	// DLL
	cfg.SetPath("/dll");
	dllPredef = cfg.Read("PreDef");

	// DLL/item[n]
	ReadAttrEntries(cfg, "/DLL", dqDLLAttrs);

	return 0;
}

void ConfigIni::ReadAttrEntries(wxFileConfig& cfg, const wxString basePath, std::deque<AttrItem>& dqAttrs)
{
	cfg.SetPath(basePath);

	long nIdx = 0;
	wxString strEntry;
	std::deque<wxString> dqEntries;
	if (cfg.GetFirstGroup(strEntry, nIdx))
	{
		dqEntries.push_back(strEntry);
		while (cfg.GetNextGroup(strEntry, nIdx))
		{
			dqEntries.push_back(strEntry);
		}
	}

	for (size_t ii = 0, iLoop = dqEntries.size();
		ii < iLoop; ++ii)
	{
		cfg.SetPath(dqEntries[ii]);

		AttrItem attr;
		attr.path =	cfg.Read("Path");
		attr.attrName = cfg.Read("AttrName");
		attr.attrDefault = cfg.Read("AttrDefault");
		dqAttrs.push_back(attr);
	}
}


int ConfigIni::SaveIni(wxString*  pIniName /*= NULL*/)
{
	// not finished.
	return -1;
}
