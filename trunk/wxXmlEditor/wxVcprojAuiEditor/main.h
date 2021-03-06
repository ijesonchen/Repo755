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

#include "..\tinyxml2\tinyxml2.h"
#include "..\wxXmlCommon\wxXmlCommon.h"
#include <deque>
#include "ConfigIni.h"

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

// class ListHeader 
// {
// public:
// 	bool isToggle;
// 	wxString label;
// 	wxDataViewCellMode mode;
// 	int width;
// 	bool isSortble;
// };

class MyFileDropTarget;
class MainFrame : public MainFrameBase
{
	public:
		MainFrame( wxWindow *parent );


		virtual ~MainFrame();
		
	protected:
		// protected event handlers
		virtual void OnCloseFrame( wxCloseEvent& event );
		virtual void menuFileOpenOnMenuSelection( wxCommandEvent& event );
		virtual void menuFileSaveOnMenuSelection( wxCommandEvent& event );
		virtual void menuReloadOnMenuSelection( wxCommandEvent& event );
		virtual void menuAutoDLLOnMenuSelection( wxCommandEvent& event );
		virtual void menuFileExpandOnMenuSelection( wxCommandEvent& event );
		virtual void menuFileExpandAllOnMenuSelection( wxCommandEvent& event );
		virtual void menuFileExitOnMenuSelection( wxCommandEvent& event );
		virtual void toolOpenOnToolClicked( wxCommandEvent& event );
		virtual void toolSaveOnToolClicked( wxCommandEvent& event );
		virtual void toolReloadOnToolClicked( wxCommandEvent& event );
		virtual void toolAutoDLLOnToolClicked( wxCommandEvent& event ) ;
		virtual void toolExpandOnToolClicked( wxCommandEvent& event );
		virtual void toolExpandAllOnToolClicked( wxCommandEvent& event );
		virtual void dirExplorerOnTreeItemActivated( wxTreeEvent& event );
		virtual void buttonAutoOnButtonClick( wxCommandEvent& event );
		virtual void buttonAutoNewDllOnButtonClick( wxCommandEvent& event );
		virtual void buttonAddDefOnButtonClick( wxCommandEvent& event );
		virtual void comboConfigOnCombobox( wxCommandEvent& event );
		virtual void checkAllOnCheckBox( wxCommandEvent& event ) ;
		virtual void checkDllOnCheckBox( wxCommandEvent& event ) ;
		virtual void dvlistMainSettingOnDataViewListCtrlItemActivated( wxDataViewEvent& event );
		virtual void dvlistMainSettingOnDataViewListCtrlItemEditingDone( wxDataViewEvent& event );



		//////////////////////////////////////////////////////////////////////////
		// data members
		wxString m_strXmlFileName;
		tinyxml2::TiDocument m_xmlDoc;

		// configuration
		ConfigIni m_config;

		//////////////////////////////////////////////////////////////////////////
		// user functions
		void OpenFile()
		{
			SelectFile();
			LoadFile();
			return;
		}
		void LoadFile();

		void ParseXml();

		void SelectFile();
		void LoadConfig();

		void SetListItems();

		void SaveDoc();

		void SetConfigSetting( wxString strConfig );

		//////////////////////////////////////////////////////////////////////////
		friend MyFileDropTarget;
};

#endif //__main__
