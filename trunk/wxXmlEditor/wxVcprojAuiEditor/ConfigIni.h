#ifndef ConfigIni_h__
#define ConfigIni_h__
#include <wx/string.h>
#include <wx/fileconf.h>
#include <deque>

class ConfigIni
{
public:
	ConfigIni(void);
	~ConfigIni(void);


	int ReadIni(wxString*  pIniName = NULL);

	int SaveIni(wxString*  pIniName= NULL);

	wxString GetIniName(void) const
	{
		return strIniName;
	};

	class AttrItem
	{
	public:
		wxString path;
		wxString attrName;
		wxString attrDefault;

		AttrItem(){};

		AttrItem(const wxString p, const wxString n, const wxString d = "")
			: path(p), attrName(n), attrDefault(d)
		{}

		bool operator== (const AttrItem& another) const
		{
			return (another.path == path) && 
				(another.attrName == attrName);
		}
	};
protected:
	// INI config file name
	wxString strIniName;
	bool SetIniName( wxString* pIniName ) ;

	void LoadDefault(void);
	void ReadAttrEntries(wxFileConfig& cfg, const wxString basePath, std::deque<AttrItem>& dqAttrs);

public:

	wxString basePath;
	wxString baseName;
	long selectCount;

	std::deque<AttrItem> dqMainAttrs;

	AttrItem attrPredef;

	wxString dllPredef;
	std::deque<AttrItem> dqDLLAttrs;

	AttrItem GetAttrItem(const wxString& path, const wxString& name, const std::deque<AttrItem>& dqAttrs) const;
};

#endif // ConfigIni_h__
