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

	wxString GetIniName(void)
	{
		return strIniName;
	};
protected:
	// INI config file name
	wxString strIniName;
	bool SetIniName( wxString* pIniName ) ;

	class AttrItem
	{
	public:
		wxString path;
		wxString attrName;
		wxString attrDefault;

		bool operator== (const AttrItem& another)
		{
			return (another.path == path) && 
				(another.attrName == attrName);
		}
	};

	void ReadAttrEntries(wxFileConfig& cfg, const wxString basePath, std::deque<AttrItem>& dqAttrs);
public:
	wxString basePath;
	wxString attrName;
	wxString selectCount;

	std::deque<AttrItem> dqMainAttrs;

	wxString dllPredef;
	std::deque<AttrItem> dqDLLAttrs;

};

#endif // ConfigIni_h__
