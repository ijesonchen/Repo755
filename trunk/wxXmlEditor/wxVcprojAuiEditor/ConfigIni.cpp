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


void ConfigIni::LoadDefault(void)
{
	basePath = "/VisualStudioProject/Configurations/Configuration";
	baseName = "Name";
	selectCount = 3;

	dqMainAttrs.push_back(AttrItem("", "OutputDirectory", "$(SolutionDir)bin\\run"));
	dqMainAttrs.push_back(AttrItem("", "IntermediateDirectory", "$(SolutionDir)bin\\inter\\$(ProjectName)_$(ConfigurationName)_$(PlatformName)"));
	dqMainAttrs.push_back(AttrItem("Tool [Name=VCLinkerTool]", "OutputFile", "$(OutDir)\\$(ProjectName)_$(ConfigurationName)_$(PlatformName).exe"));
	dqMainAttrs.push_back(AttrItem("Tool [Name=VCCLCompilerTool]", "AdditionalIncludeDirectories", "\"$(WXPATH3)\\include\";\"$(WXPATH3)\\include\\msvc\""));
	dqMainAttrs.push_back(AttrItem("Tool [Name=VCLinkerTool]", "AdditionalLibraryDirectories", "$(WXPATH3)\\lib\\vc_lib"));
	dqMainAttrs.push_back(AttrItem("Tool [Name=VCLinkerTool]", "AdditionalDependencies"));
	dqMainAttrs.push_back(AttrItem("Tool [Name=VCPostBuildEventTool]", "CommandLine", "dumpbin /dependents $(TargetPath)"));

	attrPredef.path = "Tool [Name=VCCLCompilerTool]";
	attrPredef.attrName = "PreprocessorDefinitions";
	dllPredef = "WXUSINGDLL";
	dqDLLAttrs.push_back(AttrItem("Tool [Name=VCLinkerTool]", "AdditionalLibraryDirectories", "$(WXPATH3)\\lib\\vc_dll"));
	dqDLLAttrs.push_back(AttrItem("Tool [Name=VCPostBuildEventTool]", "CommandLine", "dumpbin /dependents $(TargetPath)"));
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

	// main
	cfg.SetPath("/MAIN");
	basePath = cfg.Read("BasePath");
	baseName = cfg.Read("BaseName");
	selectCount = cfg.ReadLong("SelectCount", 0);

	// main/item[n]
	ReadAttrEntries(cfg, "/Main", dqMainAttrs);

	// predef
	cfg.SetPath("/Predef");
	attrPredef.path =	cfg.Read("Path");
	attrPredef.attrName = cfg.Read("AttrName");
	attrPredef.attrDefault = cfg.Read("AttrDefault");

	// DLL
	cfg.SetPath("/dll");
	dllPredef = cfg.Read("Predef");

	// DLL/item[n]
	ReadAttrEntries(cfg, "/DLL", dqDLLAttrs);

	return 0;
}

void ConfigIni::ReadAttrEntries(wxFileConfig& cfg, const wxString basePath, std::deque<AttrItem>& dqAttrs)
{
	wxString oldPath = cfg.GetPath();
	cfg.SetPath(basePath);

	dqAttrs.clear();

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
		if (attr.path.length() || attr.attrName.length() || attr.attrDefault.length())
		{
			dqAttrs.push_back(attr);
		}
		cfg.SetPath("..");
	}

	cfg.SetPath(oldPath);
}


int ConfigIni::SaveIni(wxString*  pIniName /*= NULL*/)
{
	// not finished.
	return -1;
}


ConfigIni::AttrItem ConfigIni::GetAttrItem(const wxString& path, const wxString& name, const std::deque<AttrItem>& dqAttrs) const
{
	AttrItem attr(path, name);
	for (int ii = 0, iLoop = dqAttrs.size();
		ii < iLoop; ++ii)
	{
		if (dqAttrs[ii] == attr)
		{
			return dqAttrs[ii];
		}
	}

	return AttrItem("", "");
}