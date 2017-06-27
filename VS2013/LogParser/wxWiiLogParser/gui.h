///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GUI_H__
#define __GUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gdicmn.h>
#include <wx/aui/aui.h>
#include <wx/aui/auibar.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/spinctrl.h>
#include <wx/checkbox.h>
#include <wx/checklst.h>
#include <wx/dataview.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/statusbr.h>
#include <wx/timer.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrameBase
///////////////////////////////////////////////////////////////////////////////
class MainFrameBase : public wxFrame 
{
	private:
	
	protected:
		wxAuiToolBar* m_auiToolBar;
		wxAuiToolBarItem* m_toolReload; 
		wxAuiToolBarItem* m_toolFilter; 
		wxTextCtrl* m_textFilename;
		wxTextCtrl* m_textSearch;
		wxButton* m_buttonSearch;
		wxSpinCtrl* m_spinRefreshSecs;
		wxCheckBox* m_checkAutoRefresh;
		wxCheckBox* m_checkAllTag;
		wxCheckListBox* m_ckListTag;
		wxCheckListBox* m_ckListCategory;
		wxPanel* m_panel1;
		wxDataViewListCtrl* m_dvLogs;
		wxStatusBar* m_statusBar;
		wxTimer m_timerRefresh;
		
		// Virtual event handlers, overide them in your derived class
		virtual void m_toolReloadOnToolClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_toolFilterOnToolClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_buttonSearchOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_checkAutoRefreshOnCheckBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_checkAllTagOnCheckBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_timerRefreshOnTimer( wxTimerEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("wxLogParser"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		wxAuiManager m_mgr;
		
		~MainFrameBase();
	
};

#endif //__GUI_H__
