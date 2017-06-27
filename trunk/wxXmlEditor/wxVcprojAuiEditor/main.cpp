/*********************************************************************
 * Name:      	main.cpp
 * Purpose:   	Implements simple wxWidgets application with GUI
 * 				created using wxFormBuilder.
 * Author:    
 * Created:   
 * Copyright: 
 * License:   	wxWidgets license (www.wxwidgets.org)
 * 
 * Notes:		Note that all GUI creation code is implemented in
 * 				gui.cpp source file which is generated by wxFormBuilder.
 *********************************************************************/

#include "main.h"

// initialize the application
IMPLEMENT_APP(MainApp);

/************************************************************************/
/*                       wxFileDropTarget BEGIN                         */
/************************************************************************/
#include <wx/dnd.h>

class MainFrame;

class MyFileDropTarget: public wxFileDropTarget
{
public:
	MyFileDropTarget(MainFrame* pFrame)
		: wxFileDropTarget()
		, m_pFrame(pFrame)
	{
	}

protected:
	MainFrame* m_pFrame;

	virtual bool OnDropFiles (wxCoord x, wxCoord y, const wxArrayString &filenames)
	{
		if (filenames.GetCount())
		{
			// only first file accepted.
			m_pFrame->m_strXmlFileName = filenames[0];
			m_pFrame->LoadFile();
			return true;
		}
		return false;
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
	SetTopWindow( new MainFrame( NULL ) );
	GetTopWindow()->Show();
	
	// true = enter the main loop
	return true;
}

////////////////////////////////////////////////////////////////////////////////
// main application frame implementation 
////////////////////////////////////////////////////////////////////////////////
#include "..\wxXmlCommon\wxXmlCommon.h"
#include "..\wxCommon\wxCommon.h"
using namespace tinyxml2;
using namespace wxXmlCommon;

#include <wx/stdpaths.h>
#include <wx/fileconf.h>

#include <deque>
using namespace std;

static const wxCommon::ListHeader gdvlistHeaderConfig[] = 
{
	{1, "", wxDATAVIEW_CELL_ACTIVATABLE, wxCOL_WIDTH_AUTOSIZE, 0},
	{0, "AttrName", wxDATAVIEW_CELL_ACTIVATABLE, wxCOL_WIDTH_AUTOSIZE, 1},
	{0, "AttrValue", wxDATAVIEW_CELL_EDITABLE, wxCOL_WIDTH_AUTOSIZE, 0},
	{0, "RelativePath", wxDATAVIEW_CELL_EDITABLE, wxCOL_WIDTH_AUTOSIZE, 1},
};

static const wxCommon::ListHeader gdvlistHeaderConfigs[] = 
{
	{1, "", wxDATAVIEW_CELL_ACTIVATABLE, wxCOL_WIDTH_AUTOSIZE, 0},
	{0, "Configuration", wxDATAVIEW_CELL_ACTIVATABLE, wxCOL_WIDTH_AUTOSIZE, 1},
};

static const wxCommon::ListHeader gdvlistHeaderPredef[] = 
{
	{1, "", wxDATAVIEW_CELL_ACTIVATABLE, wxCOL_WIDTH_AUTOSIZE, 0},
	{0, "Configuration", wxDATAVIEW_CELL_ACTIVATABLE, wxCOL_WIDTH_AUTOSIZE, 1},
	{0, "PreprocessorDefinitions", wxDATAVIEW_CELL_EDITABLE, wxCOL_WIDTH_AUTOSIZE, 1},
};

MainFrame::MainFrame(wxWindow *parent) : MainFrameBase( parent )
{	
	wxCommon::SetDvlistHeader(m_dvlistMainSetting, gdvlistHeaderConfig,
							sizeof(gdvlistHeaderConfig) / sizeof(wxCommon::ListHeader));
	wxCommon::SetDvlistHeader(m_dvlistConfigs, gdvlistHeaderConfigs,
							sizeof(gdvlistHeaderConfigs) / sizeof(wxCommon::ListHeader));
	wxCommon::SetDvlistHeader(m_dvlistPredef, gdvlistHeaderPredef,
							sizeof(gdvlistHeaderPredef) / sizeof(wxCommon::ListHeader));

	m_comboConfig->Disable();

	LoadConfig();

	wxString strPath = wxStandardPaths::Get().GetExecutablePath();
	strPath = strPath.Left(strPath.rfind('\\'));
	m_dirExplorer->SetFilter("Files (*.vcproj;*.xml)|*.vcproj;*.xml|vcproj files|*.vcproj|xml files|*.xml|*.*|*");

	m_dirExplorer->SetPath(strPath);

	wxCommon::ForceAuiBestSizeOnce(m_mgr);

	SetDropTarget(new MyFileDropTarget(this));
}

MainFrame::~MainFrame()
{
}

void MainFrame::OnCloseFrame( wxCloseEvent& event )
{
	Destroy();
}

void MainFrame::menuFileOpenOnMenuSelection( wxCommandEvent& event )
{
	OpenFile();
}

void MainFrame::menuFileExpandOnMenuSelection( wxCommandEvent& event )
{
	wxTreeItemId idSel = m_treeXml->GetSelection();
	if (!idSel.IsOk())
	{
		idSel = m_treeXml->GetRootItem();
		if (!idSel.IsOk())
		{
			return;
		}
	}
	m_treeXml->IsExpanded(idSel) ? 
		m_treeXml->Collapse(idSel) :
	m_treeXml->Expand(idSel);

	if (!m_treeXml->IsVisible(idSel))
	{
		m_treeXml->ScrollTo(idSel);
	}
}

void MainFrame::menuFileExpandAllOnMenuSelection( wxCommandEvent& event )
{
	wxTreeItemId idSel = m_treeXml->GetSelection();
	if (!idSel.IsOk())
	{
		idSel = m_treeXml->GetRootItem();
		if (!idSel.IsOk())
		{
			return;
		}
	}
	m_treeXml->IsExpanded(idSel) ? 
		m_treeXml->CollapseAllChildren(idSel) :
		m_treeXml->ExpandAllChildren(idSel);

	if (!m_treeXml->IsVisible(idSel))
	{
		m_treeXml->ScrollTo(idSel);
	}
}

void MainFrame::menuFileSaveOnMenuSelection( wxCommandEvent& event )
{
	SaveDoc();
}

void MainFrame::menuReloadOnMenuSelection( wxCommandEvent& event )
{
	LoadFile();
}
void MainFrame::menuAutoDLLOnMenuSelection( wxCommandEvent& event )
{
	buttonAutoNewDllOnButtonClick(event);
}

void MainFrame::menuFileExitOnMenuSelection( wxCommandEvent& event )
{
	Destroy();
}

void MainFrame::toolOpenOnToolClicked( wxCommandEvent& event )
{
	OpenFile();
}

void MainFrame::toolSaveOnToolClicked( wxCommandEvent& event )
{
	SaveDoc();
}

void MainFrame::toolReloadOnToolClicked( wxCommandEvent& event )
{
	menuReloadOnMenuSelection(event);
}

void MainFrame::toolAutoDLLOnToolClicked( wxCommandEvent& event )
{
	buttonAutoNewDllOnButtonClick(event);
}


void MainFrame::toolExpandOnToolClicked( wxCommandEvent& event )
{
	menuFileExpandOnMenuSelection(event);
}

void MainFrame::toolExpandAllOnToolClicked( wxCommandEvent& event )
{
	menuFileExpandAllOnMenuSelection(event);
}

void MainFrame::dirExplorerOnTreeItemActivated( wxTreeEvent& event )
{
	wxString strName;
	strName = m_dirExplorer->GetPath();

	wxString strExt;
	wxFileName::SplitPath(strName, NULL, NULL, NULL, &strExt);
	if (!wxFileExists(strName))
	{
		event.Skip();
		return;
	}
	m_strXmlFileName = strName;
	LoadFile();
}

void MainFrame::buttonAutoOnButtonClick( wxCommandEvent& event )
{
	for (int ii = 0, iLoop = m_dvlistMainSetting->GetItemCount();
		ii < iLoop; ++ii)
	{
		wxString strName = m_dvlistMainSetting->GetTextValue(ii, 1);
		wxString strPath = m_dvlistMainSetting->GetTextValue(ii, 3);
		ConfigIni::AttrItem attr;
		if (m_checkDll->IsChecked())
		{
			attr = m_config.GetAttrItem(strPath, strName, m_config.dqDLLAttrs);

			if (!attr.attrDefault.length())
			{
				attr = m_config.GetAttrItem(strPath, strName, m_config.dqMainAttrs);
			}
		}
		else
		{
			attr = m_config.GetAttrItem(strPath, strName, m_config.dqMainAttrs);
		}
		if (attr.attrDefault.length())
		{
			m_dvlistMainSetting->SetTextValue(attr.attrDefault, ii,  2);
		}
	}
}

void MainFrame::buttonAutoNewDllOnButtonClick( wxCommandEvent& event )
{
	if (m_strXmlFileName.IsEmpty())
	{
		SetStatusText("file not loaded");
		return;
	}
	if (!m_dvlistConfigs->GetItemCount())
	{
		SetStatusText("no configs. reload file.");
		return;
	}


	// load config & element
	struct XmlConfigItem 
	{
		wxString name;
		const TiElement* pe;
	};
	deque<XmlConfigItem> dqConfigs;
	TiElement* peCfgRoot = NULL;
	// /VisualStudioProject/Configurations/Configuration [Name = Debug|Win32]
	if ((peCfgRoot = m_xmlDoc.RootElement()->FirstChildElement("Configurations")))
	{
		const TiElement* peCfg = peCfgRoot->FirstChildElement("Configuration");

		while (peCfg)
		{
			const TiAttribute* peAttr = peCfg->FindAttribute("Name");
			if (peAttr)
			{
				wxString strConfig = peAttr->Value();
				if (strConfig.Lower().Find("dll") != wxNOT_FOUND)
				{
					wxString strMsg = wxString::Format("Item %s may contain DLL config, continue?", strConfig);
					if (wxNO == wxMessageBox(strMsg, "Continue", wxYES_NO, this))
					{
						return;
					}
				}
				XmlConfigItem xmlConfig;
				xmlConfig.name = strConfig;
				xmlConfig.pe = peCfg;

				dqConfigs.push_back(xmlConfig);
			}
			peCfg = peCfg->NextSiblingElement();
		}
	}

	deque<wxString> dqDllName;

	for (size_t ii = 0, iLoop = dqConfigs.size();
		ii < iLoop; ++ii)
	{
		XmlConfigItem& xmlConfig = dqConfigs[ii];
		int nSepPos = xmlConfig.name.Find('|');
		if (nSepPos == wxNOT_FOUND)
		{
			continue;
		}
		wxString strDllName = xmlConfig.name.Left(nSepPos) + "-DLL" + xmlConfig.name.Right(xmlConfig.name.Length() - nSepPos);
		TiElement* peDll = peCfgRoot->CopyAndAppendNode(xmlConfig.pe);
		dqDllName.push_back(strDllName);
		// change setting
		peDll->SetAttribute("Name", strDllName.ToAscii());

		wxString basePath = wxString::Format("%s[%s=%s]", m_config.basePath, m_config.baseName, strDllName);
		for (size_t ii = 0, iLoop = m_config.dqDLLAttrs.size(); 
			ii < iLoop; ++ii)
		{
			ConfigIni::AttrItem& attrItem = m_config.dqDLLAttrs[ii];
			wxString strXmlPath = wxString::Format("%s/%s", basePath, attrItem.path);
			m_xmlDoc.SetAttribute(strXmlPath, attrItem.attrName, attrItem.attrDefault);
		}
	}

	ParseXml();

	// check dll-config item
	for (int ii = 0, iLoop = m_dvlistConfigs->GetItemCount(); ii < iLoop; ++ii)
	{
		wxString strConfig = m_dvlistConfigs->GetTextValue(ii, 1);
		if (wxCommon::FindStringInDeque(dqDllName, strConfig) >= 0)
		{
			// DLL		
			m_dvlistConfigs->SetToggleValue(true, ii, 0);
		}
		else
		{
			m_dvlistConfigs->SetToggleValue(false, ii, 0);
		}
	}

	// check dll-predef item
	for (int ii = 0, iLoop = m_dvlistPredef->GetItemCount(); ii < iLoop; ++ii)
	{
		wxString strConfig = m_dvlistPredef->GetTextValue(ii, 1);
		if (wxCommon::FindStringInDeque(dqDllName, strConfig) >= 0)
		{
			// DLL		
			m_dvlistPredef->SetToggleValue(true, ii, 0);
		}
		else
		{
			m_dvlistPredef->SetToggleValue(false, ii, 0);
		}
	}

	m_checkDll->SetValue(true);
	checkDllOnCheckBox(event);

	buttonAddDefOnButtonClick(event);

	m_checkAll->SetValue(false);
	m_comboConfig->Enable(true);
	// select one dll config
	for (int ii = 0, iLoop = m_comboConfig->GetCount();
		ii < iLoop; ++ii)
	{
		wxString strComboConfig = m_comboConfig->GetString(ii);
		if (wxCommon::FindStringInDeque(dqDllName, strComboConfig) >= 0)
		{
			m_comboConfig->Select(ii);
			break;
		}
	}
	m_checkNoChangeMainSetting->SetValue(true);
	comboConfigOnCombobox(event);
}

void MainFrame::dvlistMainSettingOnDataViewListCtrlItemActivated( wxDataViewEvent& event )
{
	if (2 == event.GetColumn())
	{
		wxDataViewListCtrl* pdv = m_dvlistMainSetting;
		if (pdv != event.GetEventObject())
		{
			return;
		}
		pdv->EditItem(event.GetItem(), event.GetDataViewColumn());
	}
}

void MainFrame::dvlistMainSettingOnDataViewListCtrlItemEditingDone( wxDataViewEvent& event )
{
	// use less. only demo for retrieving data in edited column when edit finished (real-time)
	return;

	//////////////////////////////////////////////////////////////////////////
	wxVariant value;
	event.GetModel()->GetValue( value, event.GetItem(), 1 );
	wxString strItemName = value;
	wxString strValue = event.GetValue();

	int nRow = m_dvlistMainSetting->ItemToRow(event.GetItem());

	SetStatusText(wxString::Format("%s = %s | %s = %s", strItemName, strValue, 
		m_dvlistMainSetting->GetTextValue(nRow, 1), 
		m_dvlistMainSetting->GetTextValue(nRow, 2)));

	return;
	const wxDataViewItem &  item = event.GetItem();
	const wxDataViewColumn *  column =event.GetDataViewColumn(); 
	if (column)
	{
		wxDataViewRenderer* pRender = column->GetRenderer();

		unsigned int col = pRender->GetOwner()->GetModelColumn();

		wxVariant value;
		m_dvlistMainSetting->GetModel()->GetValue( value, item, col );
		wxString str = value;
	}
}



//////////////////////////////////////////////////////////////////////////
// user functions
void MainFrame::SelectFile()
{
	wxFileDialog openFileDialog(this, _("Open XML file"), "", "",
		"VC Project files|*.vcproj; *.xml|XML files|*.xml|All Files|*.*", 
		wxFD_OPEN|wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return; // the user changed idea...
	// proceed loading the file chosen by the user;
	// this can be done with e.g. wxWidgets input streams:
	m_strXmlFileName = openFileDialog.GetPath();

	size_t nPos = m_strXmlFileName.rfind(_T("\\"));

	wxString strTemp = m_strXmlFileName.Right(m_strXmlFileName.Len() - nPos - 1);

	m_statusBar->SetStatusText(_T("loading file: ") + strTemp);
}

void MainFrame::LoadFile(void)
{
	if (m_strXmlFileName.IsEmpty())
	{
		return;
	}
	m_dirExplorer->SetPath(m_strXmlFileName);
	m_xmlDoc.Clear();
	if (XML_SUCCESS != m_xmlDoc.LoadFile(m_strXmlFileName.mb_str()))
	{				
		m_statusBar->SetStatusText(wxString::Format("xml load failed: %d: %s", m_xmlDoc.ErrorID(), m_strXmlFileName));
		return;
	}
	ParseXml();
	return;
}

void MainFrame::LoadConfig(void)
{
	wxString strStatus;
	if (m_config.ReadIni())
	{
		strStatus = "Default loaded.";
	}
	strStatus = wxString::Format("Config %s loaded.", m_config.GetIniName());
	SetStatusText(strStatus);
	m_textBasePath->SetValue(wxString::Format("%s[%s=$(config)]", m_config.basePath, m_config.baseName));
	m_textPreDef->SetValue(m_config.attrPredef.attrDefault);
	SetListItems();
}

void MainFrame::SaveDoc(void)
{
	if (m_strXmlFileName.IsEmpty())
	{
		SetStatusText("File not loaded.");
		return;
	}
	// backup file
	wxCopyFile(m_strXmlFileName, m_strXmlFileName + ".bak");

	// main setting
	if (!m_checkNoChangeMainSetting->IsChecked())
	{
		if (m_checkAll->IsChecked())
		{
			for (int iCfg = 0, nCfg = m_dvlistConfigs->GetItemCount();
				iCfg < nCfg; ++iCfg)
			{
				if (!m_dvlistConfigs->GetToggleValue(iCfg, 0))
				{
					continue;
				}

				wxString strConfig = m_dvlistConfigs->GetTextValue(iCfg, 1);
				SetConfigSetting(strConfig);
			}
		}
		else
		{
			wxString strConfig = m_comboConfig->GetStringSelection();
			SetConfigSetting(strConfig);
		}
	}

	// predef
	int nRow = m_dvlistPredef->GetItemCount();

	for (int iItem = 0; iItem < nRow; ++iItem)
	{
		if (!m_dvlistPredef->GetToggleValue(iItem, 0))
		{
			continue;
		}
		wxString strConfigName = m_dvlistPredef->GetTextValue(iItem, 1);
		wxString strPredef = m_dvlistPredef->GetTextValue(iItem, 2);

		wxString strXmlPath = wxString::Format("%s[%s=%s]/%s", 
			m_config.basePath, m_config.baseName, strConfigName, m_config.attrPredef.path);

		m_xmlDoc.SetAttribute(strXmlPath, m_config.attrPredef.attrName, strPredef.mb_str());
	}

	m_xmlDoc.SaveFile(m_strXmlFileName);
	// re-load the file.
	LoadFile();
}

void MainFrame::SetListItems()
{
	for (size_t ii = 0, iLoop = m_config.dqMainAttrs.size();
		ii < iLoop; ++ii)
	{
		ConfigIni::AttrItem& attr = m_config.dqMainAttrs[ii];
		wxVector<wxVariant> vtItem;
		vtItem.push_back((long)ii < m_config.selectCount);
		vtItem.push_back(attr.attrName);
		vtItem.push_back("");
		vtItem.push_back(attr.path);
		m_dvlistMainSetting->InsertItem(ii, vtItem);
	}
}

void MainFrame::buttonAddDefOnButtonClick( wxCommandEvent& event )
{
	wxString strAppendix = m_textPreDef->GetLineText(0);
	if (strAppendix.IsEmpty())
	{
		return;
	}
	if (strAppendix[strAppendix.Length() - 1] != ';')
	{
		strAppendix += ';';
	}

	int nRow = m_dvlistPredef->GetItemCount();

	for (int iItem = 0; iItem < nRow; ++iItem)
	{
		if (!m_dvlistPredef->GetToggleValue(iItem, 0))
		{
			continue;
		}
		wxVector<wxVariant> vtItem;
		wxString strPredef = m_dvlistPredef->GetTextValue(iItem, 2);
		if (strPredef[strPredef.Length() - 1] != ';')
		{
			strPredef += ';';
		}
		strPredef += strAppendix;
		m_dvlistPredef->SetTextValue(strPredef, iItem, 2);
	}
}
void MainFrame::comboConfigOnCombobox( wxCommandEvent& event )
{
	// load item value and show in main setting list ctrl
	wxString basePath;
	wxString strConfig = m_comboConfig->GetStringSelection();
	if (strConfig.IsEmpty())
	{
		wxMessageBox("combo config error.");
		return;
	}
	basePath = wxString::Format("%s[%s=%s]", m_config.basePath, m_config.baseName, strConfig);
	for (int ii = 0, iLoop = m_dvlistMainSetting->GetItemCount();
		ii < iLoop; ++ii)
	{
		wxString strName = m_dvlistMainSetting->GetTextValue(ii, 1);
		wxString strPath = m_dvlistMainSetting->GetTextValue(ii, 3);

		wxString strAttrValue;
		TiElement* pe = m_xmlDoc.ElementByPath(wxString::Format("%s/%s", basePath, strPath));
		if (pe)
		{
			const char* pAttrValue = pe->Attribute(strName);
			strAttrValue = pAttrValue ? pAttrValue : "";
		}
		m_dvlistMainSetting->SetTextValue(strAttrValue, ii, 2);
	}

}

void MainFrame::checkAllOnCheckBox( wxCommandEvent& event )
{
	if (m_checkAll->IsChecked())
	{
		m_comboConfig->Disable();
	}
	else
	{
		m_comboConfig->Enable(true);
	}
}

void MainFrame::checkDllOnCheckBox( wxCommandEvent& event ) 
{
	bool bCheck = m_checkDll->IsChecked();
	// predef
	if (bCheck)
	{
		m_textPreDef->SetValue(m_config.dllPredef);
	}
	else
	{
		m_textPreDef->SetValue(m_config.attrPredef.attrDefault);
	}
	// main setting
	for (int ii = 0, iLoop = m_dvlistMainSetting->GetItemCount();
		ii < iLoop; ++ii)
	{
		wxString strName = m_dvlistMainSetting->GetTextValue(ii, 1);
		wxString strPath = m_dvlistMainSetting->GetTextValue(ii, 3);
		ConfigIni::AttrItem attrDll;
		attrDll = m_config.GetAttrItem(strPath, strName, m_config.dqDLLAttrs);
		if (!attrDll.path.length())
		{
			// if [DLL/ITEMn] not exist, no need to modify
			continue;
		}
		if (bCheck)
		{
			m_dvlistMainSetting->SetToggleValue(true, ii, 0);
			m_dvlistMainSetting->SetTextValue(attrDll.attrDefault, ii,  2);
		}
		else
		{
			ConfigIni::AttrItem attrlib = m_config.GetAttrItem(strPath, strName, m_config.dqMainAttrs);
			m_dvlistMainSetting->SetTextValue(attrlib.attrDefault, ii,  2);
		}
	}
}

void MainFrame::ParseXml()
{
	const TiElement* peRoot = m_xmlDoc.RootElement();
	if (!peRoot)
	{
		return;
	}
	m_treeXml->DeleteAllItems();

	bool bUtf8 = false;
	if (GetDocEncoding(m_xmlDoc) == TiEncUTF8)
	{
		bUtf8 = true;
	}

	wxTreeItemId idRoot = AddElmtToTree(m_treeXml, wxTreeItemId(), peRoot, bUtf8);

	m_treeXml->Expand(idRoot);

	deque<wxString> dqConfigs;
	const TiElement* peCfgs = NULL;
	// /VisualStudioProject/Configurations/Configuration [Name = Debug|Win32]
	if ((peCfgs = m_xmlDoc.RootElement()->FirstChildElement("Configurations")))
	{
		const TiElement* peCfg = peCfgs->FirstChildElement("Configuration");

		while (peCfg)
		{
			const TiAttribute* peAttr = peCfg->FindAttribute("Name");
			if (peAttr)
			{
				dqConfigs.push_back(peAttr->Value());
			}
			peCfg = peCfg->NextSiblingElement();
		}
	}

	// load configurations & combo
	m_dvlistConfigs->DeleteAllItems();
	m_comboConfig->Clear();
	for (size_t ii = 0, iCount = dqConfigs.size(); ii < iCount; ++ii)
	{
		wxVector<wxVariant> vtItem;
		vtItem.push_back(true);
		vtItem.push_back(dqConfigs[ii]);
		m_dvlistConfigs->InsertItem(ii, vtItem);
		m_comboConfig->Append(dqConfigs[ii]);
	}
	if (dqConfigs.size())
	{
		m_comboConfig->Select(0);
	}

	// load item value and show in predef dvlist
	m_dvlistPredef->DeleteAllItems();
	if (dqConfigs.size())
	{
		wxVector<wxVariant> vtItem;
		for (size_t ii = 0, iCount = dqConfigs.size(); ii < iCount; ++ii)
		{
			vtItem.clear();
			vtItem.push_back(true);
			vtItem.push_back(dqConfigs[ii]);

			wxString strAttrValue;
			wxString strAttrPath = wxString::Format("%s[%s=%s]/%s", 
				m_config.basePath, m_config.baseName, dqConfigs[ii], m_config.attrPredef.path);
			TiElement* pe = m_xmlDoc.ElementByPath(strAttrPath);
			if (pe)
			{
				const char* pAttrValue = pe->Attribute(m_config.attrPredef.attrName);
				strAttrValue = pAttrValue ? pAttrValue : "";
			}
			vtItem.push_back(strAttrValue);

			m_dvlistPredef->InsertItem(ii, vtItem);
		}
	}

	// load item value and show in main setting list ctrl
	wxString basePath;
	if (!m_checkAll->IsChecked())
	{
		wxString strConfig = m_comboConfig->GetStringSelection();
		if (!strConfig.IsEmpty())
		{
			basePath = wxString::Format("%s[%s=%s]", m_config.basePath, m_config.baseName, strConfig);
		}
	}
	else
	{
		basePath = m_config.basePath;
	}
	for (int ii = 0, iLoop = m_dvlistMainSetting->GetItemCount();
		ii < iLoop; ++ii)
	{
		wxString strName = m_dvlistMainSetting->GetTextValue(ii, 1);
		wxString strPath = m_dvlistMainSetting->GetTextValue(ii, 3);

		wxString strAttrValue;
		TiElement* pe = m_xmlDoc.ElementByPath(wxString::Format("%s/%s", basePath, strPath));
		if (pe)
		{
			const char* pAttrValue = pe->Attribute(strName);
			strAttrValue = pAttrValue ? pAttrValue : "";
		}
		m_dvlistMainSetting->SetTextValue(strAttrValue, ii, 2);
	}
}

void MainFrame::SetConfigSetting( wxString strConfig )
{
	if (strConfig.IsEmpty())
	{
		wxMessageBox("Config name is empty when SetConfigSetting");
		return;
	}
	for (int iItem = 0, nItem = m_dvlistMainSetting->GetItemCount();
		iItem < nItem; ++iItem)
	{
		if (!m_dvlistMainSetting->GetToggleValue(iItem, 0))
		{
			continue;
		}
		wxString strAttrName = m_dvlistMainSetting->GetTextValue(iItem, 1);
		wxString strAttrValue = m_dvlistMainSetting->GetTextValue(iItem, 2);
		wxString strPath = m_dvlistMainSetting->GetTextValue(iItem, 3);

		wxString strXmlPath = wxString::Format("%s[%s=%s]/%s", 
			m_config.basePath, m_config.baseName, strConfig, strPath);

		m_xmlDoc.SetAttribute(strXmlPath, strAttrName, strAttrValue.mb_str());
	}
}