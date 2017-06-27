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
#include <wx/filepicker.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/sizer.h>
#include <wx/dataview.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/notebook.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/aui/aui.h>
#include <wx/stattext.h>
#include <wx/combobox.h>
#include <wx/textctrl.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrameBase
///////////////////////////////////////////////////////////////////////////////
class MainFrameBase : public wxFrame 
{
	private:
	
	protected:
		wxFilePickerCtrl* m_filePicker;
		wxNotebook* m_notebookMain;
		wxPanel* m_penelDetail;
		wxButton* m_buttonAddDet;
		wxButton* m_buttonUpdateDet;
		wxButton* m_buttonRemoveDet;
		wxButton* m_buttonListDet;
		wxDatePickerCtrl* m_datePickerBeginDet;
		wxDatePickerCtrl* m_datePickerEndDet;
		wxDataViewListCtrl* m_dvlistDetail;
		wxPanel* m_panelAccount;
		wxButton* m_buttonAddAcc;
		wxButton* m_buttonUpdateAcc;
		wxButton* m_buttonRemoveAcc;
		wxButton* m_buttonListAcc;
		wxDataViewListCtrl* m_dvlistAccount;
		wxStatusBar* m_statusBar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void MainFrameBaseOnActivate( wxActivateEvent& event ) { event.Skip(); }
		virtual void OnCloseFrame( wxCloseEvent& event ) { event.Skip(); }
		virtual void m_filePickerOnFileChanged( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void m_buttonAddDetOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_buttonUpdateDetOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_buttonRemoveDetOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_buttonListDetOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_buttonAddAccOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_buttonUpdateAccOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_buttonRemoveAccOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_buttonListAccOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxCLOSE_BOX|wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		wxAuiManager m_mgr;
		
		~MainFrameBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class DlgDetailBase
///////////////////////////////////////////////////////////////////////////////
class DlgDetailBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText23;
		wxComboBox* m_comboBox11;
		wxStaticText* m_staticText24;
		wxComboBox* m_comboBox12;
		wxStaticText* m_staticText25;
		wxDatePickerCtrl* m_datePicker3;
		wxStaticText* m_staticText26;
		wxTextCtrl* m_textCtrl1;
		wxStaticText* m_staticText27;
		wxTextCtrl* m_textCtrl2;
		wxStaticText* m_staticText28;
		wxButton* m_buttonOK;
		wxButton* m_buttonCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void DlgDetailBaseOnInitDialog( wxInitDialogEvent& event ) { event.Skip(); }
		
	
	public:
		
		DlgDetailBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 366,246 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~DlgDetailBase();
	
};

#endif //__GUI_H__
