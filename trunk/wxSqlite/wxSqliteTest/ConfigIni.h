#ifndef ConfigIni_h__
#define ConfigIni_h__
#include <deque>
#include <wx/string.h>
#include <wx/fileconf.h>
#include <wx/treectrl.h>

class wxTreeItemId;

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

	class Item
	{
	public:
		int entry;
		wxString name;
		wxString path;
		wxTreeItemId treeid;

		Item(){};

		Item(const int& e, const wxString& n, const wxString& p)
		: entry(e), name(n), path(p)
		{}
	protected:
	private:
	};

protected:
	// INI config file name
	wxString strIniName;
	bool SetIniName( wxString* pIniName ) ;

	void LoadDefault(void);
//	void ReadAttrEntries(wxFileConfig& cfg, const wxString basePath, std::deque<AttrItem>& dqAttrs);

public:
	std::deque<Item> dqItems;
	Item GetItem(const wxTreeItemId treeid);
};

#endif // ConfigIni_h__
