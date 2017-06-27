#include "wxXmlCommon.h"

using namespace tinyxml2;

#include <iostream>
#include <deque>
#include <vector>
using namespace std;

namespace wxXmlCommon
{


void PrintXml(wchar_t* strFileName)
{
	TiDocument xmlDoc;
	TiError tiRet = xmlDoc.LoadFile(strFileName);

	TiElement* pRoot = xmlDoc.RootElement();
	TiElement* peCfg;
	peCfg = pRoot->FirstChildElement("Configurations")->FirstChildElement("Configuration");
	
	while (peCfg)
	{
		PrintElement(peCfg);
		const TiElement* peLinker = NULL;
		peLinker = peCfg->FirstChildElement("Tool");
		while (peLinker)
		{
			const TiAttribute* peAttr = peLinker->FindAttribute("Name");
			if (!strcmp(peAttr->Value(), "VCLinkerTool"))
			{
				continue;
			}
			PrintElement(peLinker);
			peLinker = peLinker->NextSiblingElement("Tool");
		}

		peCfg = peCfg->NextSiblingElement();
	}
}


void PrintAllText(const tinyxml2::TiElement* pe)
{
	if (!pe)
	{
		return;
	}
	PrintElement(pe);
	const TiElement* peChild = pe->FirstChildElement();
	while (peChild)
	{
		PrintAllText(peChild);
		peChild = peChild->NextSiblingElement();
	}
}


void PrintElement(const tinyxml2::TiElement* pe)
{
	if (!pe)
	{
		return;
	}
	const char* psz = NULL;
	psz = pe->Name();
	if (psz)
	{
		cout<<pe->Name();
	}
	{
		const TiAttribute* pa = pe->FirstAttribute();
		bool bAttr = false;
		if (pa)
		{
			cout<<endl;
		}
		while (pa)
		{
			cout<<"\t";
			psz = pa->Name();
			if (psz)
			{
				cout<<psz;
			}
			cout<<" = ";
			psz = pa->Value();
			if (psz)
			{
				cout<<psz;
			}
			cout<<endl;
			pa = pa->Next();
		}
	}

	psz = pe->GetText();
	if (psz)
	{
		cout<<": "<<psz;
	}
	else
	{
		cout<<"/";
	}
	cout<<endl;
}


wxTreeItemId AddElmtOnlyToTree(wxTreeCtrl* const pTree, const wxTreeItemId& idParent, const tinyxml2::TiElement* const pe, bool bUtf8 /*= false*/)
{
	if (!pe)
	{
		return wxTreeItemId();
	}

	// utf-8 encoding
	if (bUtf8)
	{

		// name
		wxTreeItemId idThis;
		const char* pszText = NULL;
		pszText = pe->Name() ? pe->Name() : "";
		idThis = pTree->AppendItem(idParent, wxString::FromUTF8(pszText));

		// attributes
		const TiAttribute* peAttr = pe->FirstAttribute();
		wxTreeItemId idAttr;
		if (peAttr)
		{
			idAttr = pTree->AppendItem(idThis, "*Attributes");
		}
		while (peAttr)
		{
			pszText = peAttr->Name() ? peAttr->Name() : "";
			wxTreeItemId idAttrNode = pTree->AppendItem(idAttr, wxString::FromUTF8(pszText));

			if (!strcmp(pszText, "Name") || !strcmp(pszText, "name"))
			{
				wxString strNodeName = wxString::Format("%s [%s]",
					pe->Name() ? wxString::FromUTF8(pe->Name()) : "",
					peAttr->Value() ? wxString::FromUTF8(peAttr->Value()) : "");
				pTree->SetItemText(idThis, strNodeName);
			}

			pszText = peAttr->Value() ? peAttr->Value() : "";
			pTree->AppendItem(idAttrNode, wxString::FromUTF8(pszText));

			peAttr = peAttr->Next();
		}

		if (pe->GetText())
		{
			pTree->AppendItem(idThis, wxString::FromUTF8(pe->GetText()));
		}

		return idThis;
	}

	// name
	wxTreeItemId idThis;
	const char* pszText = NULL;
	pszText = pe->Name() ? pe->Name() : "";
	idThis = pTree->AppendItem(idParent, pszText);

	// attributes
	const TiAttribute* peAttr = pe->FirstAttribute();
	wxTreeItemId idAttr;
	if (peAttr)
	{
		idAttr = pTree->AppendItem(idThis, "*Attributes");
	}
	while (peAttr)
	{
		pszText = peAttr->Name() ? peAttr->Name() : "";
		wxTreeItemId idAttrNode = pTree->AppendItem(idAttr, pszText);

		if (!strcmp(pszText, "Name") || !strcmp(pszText, "name"))
		{
			wxString strNodeName = wxString::Format("%s [%s]",
										pe->Name() ? pe->Name() : "",
										peAttr->Value() ? peAttr->Value() : "");
			pTree->SetItemText(idThis, strNodeName);
		}

		pszText = peAttr->Value() ? peAttr->Value() : "";
		pTree->AppendItem(idAttrNode, pszText);

		peAttr = peAttr->Next();
	}

	if (pe->GetText())
	{
		pTree->AppendItem(idThis, pe->GetText());
	}

	return idThis;
}

wxTreeItemId AddElmtToTree(wxTreeCtrl* const pTree, const wxTreeItemId& idParent, const tinyxml2::TiElement* const pe, bool bUtf8 /*= false*/)
{
	if (!pe)
	{
		return wxTreeItemId();
	}
	wxTreeItemId idThis = AddElmtOnlyToTree(pTree, idParent, pe, bUtf8);

	const TiElement* peChild = pe->FirstChildElement();
	while (peChild)
	{
		AddElmtToTree(pTree, idThis, peChild, bUtf8);
		peChild = peChild->NextSiblingElement();
	}

	return idThis;
}


} // namespace wxXmlCommon