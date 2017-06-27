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
#include <wx/treectrl.h>
#include <wx/dataview.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/stc/stc.h>
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
		wxMenuBar* m_mbarMain;
		wxMenu* m_menuFile;
		wxAuiToolBar* m_auiToolBar;
		wxAuiToolBarItem* m_toolOpen; 
		wxTreeCtrl* m_treeDatabase;
		wxNotebook* m_nbMain;
		wxPanel* m_panelTable;
		wxDataViewListCtrl* m_dvTable;
		wxPanel* m_panelQuery;
		wxButton* m_buttonQuery;
		wxStyledTextCtrl* m_scintilla1;
		wxDataViewListCtrl* m_dataViewListCtrl3;
		wxStatusBar* m_statusBar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCloseFrame( wxCloseEvent& event ) { event.Skip(); }
		virtual void m_miOpenOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_toolOpenOnToolClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_treeDatabaseOnTreeItemActivated( wxTreeEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("wxSqliteExplorer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxCLOSE_BOX|wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		wxAuiManager m_mgr;
		
		~MainFrameBase();
	
};

#endif //__GUI_H__
