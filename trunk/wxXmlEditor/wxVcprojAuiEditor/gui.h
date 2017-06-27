///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GUI_H__
#define __GUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/aui/aui.h>
#include <wx/aui/auibar.h>
#include <wx/statusbr.h>
#include <wx/dirctrl.h>
#include <wx/treectrl.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/dataview.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/combobox.h>
#include <wx/checkbox.h>
#include <wx/stattext.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrameBase
///////////////////////////////////////////////////////////////////////////////
class MainFrameBase : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* m_menubar;
		wxMenu* m_menuFile;
		wxAuiToolBar* m_auiToolBar1;
		wxAuiToolBarItem* m_toolOpen; 
		wxAuiToolBarItem* m_toolSave; 
		wxAuiToolBarItem* m_toolReload; 
		wxAuiToolBarItem* m_toolAutoDLL; 
		wxAuiToolBarItem* m_toolExpand; 
		wxAuiToolBarItem* m_toolExpandAll; 
		wxStatusBar* m_statusBar;
		wxGenericDirCtrl* m_dirExplorer;
		wxTreeCtrl* m_treeXml;
		wxPanel* m_panelConfigs;
		wxButton* m_buttonAuto;
		wxButton* m_buttonAutoNewDll;
		wxDataViewListCtrl* m_dvlistConfigs;
		wxPanel* m_panelPredef;
		wxButton* m_buttonAddDef;
		wxTextCtrl* m_textPreDef;
		wxDataViewListCtrl* m_dvlistPredef;
		wxPanel* m_panelMainSetting;
		wxComboBox* m_comboConfig;
		wxCheckBox* m_checkAll;
		wxCheckBox* m_checkDll;
		wxCheckBox* m_checkNoChangeMainSetting;
		wxStaticText* m_staticText1;
		wxTextCtrl* m_textBasePath;
		wxDataViewListCtrl* m_dvlistMainSetting;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCloseFrame( wxCloseEvent& event ) { event.Skip(); }
		virtual void menuFileOpenOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void menuFileSaveOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void menuReloadOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void menuAutoDLLOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void menuFileExpandOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void menuFileExpandAllOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void menuFileExitOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void toolOpenOnToolClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void toolSaveOnToolClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void toolReloadOnToolClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void toolAutoDLLOnToolClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void toolExpandOnToolClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void toolExpandAllOnToolClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void dirExplorerOnTreeItemActivated( wxTreeEvent& event ) { event.Skip(); }
		virtual void buttonAutoOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void buttonAutoNewDllOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void buttonAddDefOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void comboConfigOnCombobox( wxCommandEvent& event ) { event.Skip(); }
		virtual void checkAllOnCheckBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void checkDllOnCheckBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void dvlistMainSettingOnDataViewListCtrlItemActivated( wxDataViewEvent& event ) { event.Skip(); }
		virtual void dvlistMainSettingOnDataViewListCtrlItemEditingDone( wxDataViewEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("wxVcprojAuiEditor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,800 ), long style = wxCLOSE_BOX|wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		wxAuiManager m_mgr;
		
		~MainFrameBase();
	
};

#endif //__GUI_H__
