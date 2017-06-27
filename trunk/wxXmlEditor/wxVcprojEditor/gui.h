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
#include <wx/dirctrl.h>
#include <wx/treectrl.h>
#include <wx/sizer.h>
#include <wx/combobox.h>
#include <wx/button.h>
#include <wx/dataview.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrameBase
///////////////////////////////////////////////////////////////////////////////
class MainFrameBase : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* m_menuBar;
		wxMenu* m_menuFile;
		wxGenericDirCtrl* m_dirExplorer;
		wxTreeCtrl* m_treeXml;
		wxNotebook* m_nbConfigs;
		wxPanel* m_panelGeneral;
		wxComboBox* m_comboConfig;
		wxButton* m_buttonDefault;
		wxButton* m_buttonSave;
		wxDataViewListCtrl* m_dvlistConfig;
		wxStatusBar* m_statusBar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCloseFrame( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnMenuFileOpen( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuFileExpand( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnExitClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void dirExplorerOnTreeItemActivated( wxTreeEvent& event ) { event.Skip(); }
		virtual void buttonDefaultOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void buttonSaveOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void dvlistConfigOnDataViewListCtrlItemActivated( wxDataViewEvent& event ) { event.Skip(); }
		virtual void dvlistConfigOnDataViewListCtrlItemEditingDone( wxDataViewEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 600,800 ), long style = wxCLOSE_BOX|wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MainFrameBase();
	
};

#endif //__GUI_H__
