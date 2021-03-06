/*********************************************************************
 * Name:      	main.h
 * Purpose:   	Declares simple wxWidgets application with GUI
 * 				created using wxFormBuilder.
 * Author:    
 * Created:   
 * Copyright: 
 * License:   	wxWidgets license (www.wxwidgets.org)
 * 
 * Notes:		Note that all GUI creation code is declared in
 * 				gui.h source file which is generated by wxFormBuilder.
 *********************************************************************/

#ifndef __main__
#define __main__

// main wxWidgets header file
#include <wx/wx.h>
// gui classes generated by wxFormBuilder
#include "gui.h"
#include <deque>

#include "DBDailyMoney.h"
////////////////////////////////////////////////////////////////////////////////
// application class declaration 
////////////////////////////////////////////////////////////////////////////////

class MainApp : public wxApp
{
	public:
		virtual bool OnInit();
};

// declare global static function wxGetApp()
DECLARE_APP(MainApp)

////////////////////////////////////////////////////////////////////////////////
// main application frame declaration 
////////////////////////////////////////////////////////////////////////////////

class MainFrame : public MainFrameBase
{
	public:
		MainFrame( wxWindow *parent );
		virtual ~MainFrame();
		
	protected:
		// protected event handlers
		virtual void MainFrameBaseOnActivate( wxActivateEvent& event ) ;
		virtual void OnCloseFrame( wxCloseEvent& event );
		virtual void OnExitClick( wxCommandEvent& event );

		// derived from GUI
		virtual void m_filePickerOnFileChanged( wxFileDirPickerEvent& event );
		virtual void m_buttonAddDetOnButtonClick( wxCommandEvent& event );
		virtual void m_buttonUpdateDetOnButtonClick( wxCommandEvent& event );
// 		virtual void m_buttonRemoveDetOnButtonClick( wxCommandEvent& event );
		virtual void m_buttonListDetOnButtonClick( wxCommandEvent& event );
// 		virtual void m_buttonAddAccOnButtonClick( wxCommandEvent& event );
// 		virtual void m_buttonUpdateAccOnButtonClick( wxCommandEvent& event );
// 		virtual void m_buttonRemoveAccOnButtonClick( wxCommandEvent& event );
		virtual void m_buttonListAccOnButtonClick( wxCommandEvent& event );

		// user members
		DailyMoney::DBDailyMoney m_dbDailyMoney;

		// user functions
		bool DbNotOpen();
		void SetDetailListData(const std::deque<DailyMoney::Detail>& dqDetails);
		void SetAccountListData(const std::deque<DailyMoney::Account>& dqAccounts);
};

//////////////////////////////////////////////////////////////////////////
// DlgDetailBase
//////////////////////////////////////////////////////////////////////////

class DlgDetail: public DlgDetailBase
{
public:
	enum DlgDetailMode
	{
		DlgDetailModeInvalid,
		DlgDetailModeAdd,
		DlgDetailModeUpdate,
		DlgDetailModeShow
	};

	// DlgDetailBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 366,246 ), long style = wxDEFAULT_DIALOG_STYLE ); 
	DlgDetail(wxWindow* parent, DailyMoney::DBDailyMoney* pdb, DlgDetailMode mode, const wxString& title = wxEmptyString);

	DailyMoney::DBDailyMoney* m_pdbDailyMoney;
	DailyMoney::Detail m_detail;

	DlgDetailMode m_nDlgMode;
protected:
	virtual void DlgDetailBaseOnInitDialog( wxInitDialogEvent& event );

private:
};


#endif //__main__
