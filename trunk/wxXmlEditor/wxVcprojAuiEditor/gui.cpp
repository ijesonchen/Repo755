///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "gui.h"

///////////////////////////////////////////////////////////////////////////

MainFrameBase::MainFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	m_mgr.SetManagedWindow(this);
	m_mgr.SetFlags(wxAUI_MGR_DEFAULT);
	
	m_menubar = new wxMenuBar( 0 );
	m_menuFile = new wxMenu();
	wxMenuItem* m_menuFileOpen;
	m_menuFileOpen = new wxMenuItem( m_menuFile, wxID_OPEN, wxString( _("&Open") ) + wxT('\t') + wxT("alt+o"), wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menuFileOpen->SetBitmaps( wxArtProvider::GetBitmap( wxART_FILE_OPEN, wxART_MENU ) );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_menuFileOpen->SetBitmap( wxArtProvider::GetBitmap( wxART_FILE_OPEN, wxART_MENU ) );
	#endif
	m_menuFile->Append( m_menuFileOpen );
	
	wxMenuItem* m_menuFileSave;
	m_menuFileSave = new wxMenuItem( m_menuFile, wxID_SAVE, wxString( _("&SaveConfig") ) + wxT('\t') + wxT("alt+s"), wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menuFileSave->SetBitmaps( wxArtProvider::GetBitmap( wxART_FILE_SAVE, wxART_MENU ) );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_menuFileSave->SetBitmap( wxArtProvider::GetBitmap( wxART_FILE_SAVE, wxART_MENU ) );
	#endif
	m_menuFile->Append( m_menuFileSave );
	
	wxMenuItem* m_menuReload;
	m_menuReload = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("Reload") ) + wxT('\t') + wxT("alt+r"), wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menuReload->SetBitmaps( wxArtProvider::GetBitmap( wxART_UNDO, wxART_MENU ) );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_menuReload->SetBitmap( wxArtProvider::GetBitmap( wxART_UNDO, wxART_MENU ) );
	#endif
	m_menuFile->Append( m_menuReload );
	
	m_menuFile->AppendSeparator();
	
	wxMenuItem* m_menuAutoDLL;
	m_menuAutoDLL = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("AutoDLL") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menuAutoDLL->SetBitmaps( wxArtProvider::GetBitmap( wxART_UNDO, wxART_MENU ) );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_menuAutoDLL->SetBitmap( wxArtProvider::GetBitmap( wxART_UNDO, wxART_MENU ) );
	#endif
	m_menuFile->Append( m_menuAutoDLL );
	
	wxMenuItem* m_menuFileExpand;
	m_menuFileExpand = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("&Expand") ) + wxT('\t') + wxT("alt+e"), wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuFileExpand );
	
	wxMenuItem* m_menuFileExpandAll;
	m_menuFileExpandAll = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("Expand&All") ) + wxT('\t') + wxT("alt+a"), _("Expand all children item"), wxITEM_NORMAL );
	m_menuFile->Append( m_menuFileExpandAll );
	
	m_menuFile->AppendSeparator();
	
	wxMenuItem* m_menuFileExit;
	m_menuFileExit = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("E&xit") ) + wxT('\t') + wxT("alt+x"), wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuFileExit );
	
	m_menubar->Append( m_menuFile, _("File") ); 
	
	this->SetMenuBar( m_menubar );
	
	m_auiToolBar1 = new wxAuiToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_HORZ_LAYOUT ); 
	m_toolOpen = m_auiToolBar1->AddTool( wxID_ANY, _("tool"), wxArtProvider::GetBitmap( wxART_FILE_OPEN, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, _("Open"), _("Open a file"), NULL ); 
	
	m_toolSave = m_auiToolBar1->AddTool( wxID_ANY, _("tool"), wxArtProvider::GetBitmap( wxART_FILE_SAVE, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, _("SaveConfig"), _("Save config modifation"), NULL ); 
	
	m_toolReload = m_auiToolBar1->AddTool( wxID_ANY, _("tool"), wxArtProvider::GetBitmap( wxART_UNDO, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, _("Reload"), _("Reload the file."), NULL ); 
	
	m_toolAutoDLL = m_auiToolBar1->AddTool( wxID_ANY, _("tool"), wxArtProvider::GetBitmap( wxART_EXECUTABLE_FILE, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, _("AutoDLL"), _("Auto set for wxDLL"), NULL ); 
	
	m_toolExpand = m_auiToolBar1->AddTool( wxID_ANY, _("tool"), wxArtProvider::GetBitmap( wxART_LIST_VIEW, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_toolExpandAll = m_auiToolBar1->AddTool( wxID_ANY, _("tool"), wxArtProvider::GetBitmap( wxART_REPORT_VIEW, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_auiToolBar1->Realize();
	m_mgr.AddPane( m_auiToolBar1, wxAuiPaneInfo().Top().PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ).DockFixed( false ).Layer( 10 ).ToolbarPane() );
	
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	m_dirExplorer = new wxGenericDirCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRCTRL_3D_INTERNAL|wxDIRCTRL_SHOW_FILTERS|wxSUNKEN_BORDER, wxEmptyString, 0 );
	
	m_dirExplorer->ShowHidden( false );
	m_mgr.AddPane( m_dirExplorer, wxAuiPaneInfo() .Right() .Caption( _("Explorer") ).CloseButton( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxSize( 260,400 ) ).DockFixed( false ).BestSize( wxSize( 260,-1 ) ) );
	
	m_treeXml = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
	m_mgr.AddPane( m_treeXml, wxAuiPaneInfo() .Left() .Caption( _("XmlContent") ).CaptionVisible( false ).CloseButton( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxSize( 200,200 ) ).DockFixed( false ).Row( 1 ).CentrePane() );
	
	m_panelConfigs = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_panelConfigs, wxAuiPaneInfo() .Left() .Caption( _("Configurations") ).CloseButton( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxSize( 200,400 ) ).DockFixed( false ).BestSize( wxSize( 200,-1 ) ) );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonAuto = new wxButton( m_panelConfigs, wxID_ANY, _("Auto"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( m_buttonAuto, 0, wxALL, 5 );
	
	m_buttonAutoNewDll = new wxButton( m_panelConfigs, wxID_ANY, _("NewDLL"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( m_buttonAutoNewDll, 0, wxALL, 5 );
	
	
	bSizer5->Add( bSizer7, 0, wxEXPAND, 5 );
	
	m_dvlistConfigs = new wxDataViewListCtrl( m_panelConfigs, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_dvlistConfigs, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panelConfigs->SetSizer( bSizer5 );
	m_panelConfigs->Layout();
	bSizer5->Fit( m_panelConfigs );
	m_panelPredef = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_panelPredef, wxAuiPaneInfo() .Bottom() .Caption( _("Predef") ).CloseButton( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxSize( 200,400 ) ).DockFixed( false ).Row( 1 ).BestSize( wxSize( -1,150 ) ) );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonAddDef = new wxButton( m_panelPredef, wxID_ANY, _("AddDef"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_buttonAddDef, 0, wxALL, 5 );
	
	m_textPreDef = new wxTextCtrl( m_panelPredef, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 150,-1 ), 0 );
	bSizer9->Add( m_textPreDef, 0, wxALL|wxEXPAND, 5 );
	
	
	bSizer4->Add( bSizer9, 0, wxEXPAND, 5 );
	
	m_dvlistPredef = new wxDataViewListCtrl( m_panelPredef, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_dvlistPredef, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panelPredef->SetSizer( bSizer4 );
	m_panelPredef->Layout();
	bSizer4->Fit( m_panelPredef );
	m_panelMainSetting = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_panelMainSetting, wxAuiPaneInfo() .Bottom() .Caption( _("MainSetting") ).CloseButton( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxSize( 400,235 ) ).DockFixed( false ).BestSize( wxSize( -1,235 ) ) );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	m_comboConfig = new wxComboBox( m_panelMainSetting, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 150,-1 ), 0, NULL, wxCB_READONLY ); 
	bSizer6->Add( m_comboConfig, 0, wxALL|wxEXPAND, 5 );
	
	m_checkAll = new wxCheckBox( m_panelMainSetting, wxID_ANY, _("All Config"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkAll->SetValue(true); 
	bSizer6->Add( m_checkAll, 0, wxALL|wxEXPAND, 5 );
	
	m_checkDll = new wxCheckBox( m_panelMainSetting, wxID_ANY, _("DLL Setting"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_checkDll, 0, wxALL|wxEXPAND, 5 );
	
	m_checkNoChangeMainSetting = new wxCheckBox( m_panelMainSetting, wxID_ANY, _("NoChange"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_checkNoChangeMainSetting, 0, wxALL|wxEXPAND, 5 );
	
	
	bSizer1->Add( bSizer6, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer71;
	bSizer71 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText1 = new wxStaticText( m_panelMainSetting, wxID_ANY, _("BasePath"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer71->Add( m_staticText1, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_textBasePath = new wxTextCtrl( m_panelMainSetting, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	bSizer71->Add( m_textBasePath, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer1->Add( bSizer71, 0, wxEXPAND, 5 );
	
	m_dvlistMainSetting = new wxDataViewListCtrl( m_panelMainSetting, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_dvlistMainSetting, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panelMainSetting->SetSizer( bSizer1 );
	m_panelMainSetting->Layout();
	bSizer1->Fit( m_panelMainSetting );
	
	m_mgr.Update();
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::OnCloseFrame ) );
	this->Connect( m_menuFileOpen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::menuFileOpenOnMenuSelection ) );
	this->Connect( m_menuFileSave->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::menuFileSaveOnMenuSelection ) );
	this->Connect( m_menuReload->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::menuReloadOnMenuSelection ) );
	this->Connect( m_menuAutoDLL->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::menuAutoDLLOnMenuSelection ) );
	this->Connect( m_menuFileExpand->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::menuFileExpandOnMenuSelection ) );
	this->Connect( m_menuFileExpandAll->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::menuFileExpandAllOnMenuSelection ) );
	this->Connect( m_menuFileExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::menuFileExitOnMenuSelection ) );
	this->Connect( m_toolOpen->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::toolOpenOnToolClicked ) );
	this->Connect( m_toolSave->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::toolSaveOnToolClicked ) );
	this->Connect( m_toolReload->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::toolReloadOnToolClicked ) );
	this->Connect( m_toolAutoDLL->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::toolAutoDLLOnToolClicked ) );
	this->Connect( m_toolExpand->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::toolExpandOnToolClicked ) );
	this->Connect( m_toolExpandAll->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::toolExpandAllOnToolClicked ) );
	m_dirExplorer->Connect( wxEVT_COMMAND_TREE_ITEM_ACTIVATED, wxTreeEventHandler( MainFrameBase::dirExplorerOnTreeItemActivated ), NULL, this );
	m_buttonAuto->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::buttonAutoOnButtonClick ), NULL, this );
	m_buttonAutoNewDll->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::buttonAutoNewDllOnButtonClick ), NULL, this );
	m_buttonAddDef->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::buttonAddDefOnButtonClick ), NULL, this );
	m_comboConfig->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( MainFrameBase::comboConfigOnCombobox ), NULL, this );
	m_checkAll->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MainFrameBase::checkAllOnCheckBox ), NULL, this );
	m_checkDll->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MainFrameBase::checkDllOnCheckBox ), NULL, this );
	this->Connect( wxID_ANY, wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler( MainFrameBase::dvlistMainSettingOnDataViewListCtrlItemActivated ) );
	this->Connect( wxID_ANY, wxEVT_COMMAND_DATAVIEW_ITEM_EDITING_DONE, wxDataViewEventHandler( MainFrameBase::dvlistMainSettingOnDataViewListCtrlItemEditingDone ) );
}

MainFrameBase::~MainFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::OnCloseFrame ) );
	this->Disconnect( wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::menuFileOpenOnMenuSelection ) );
	this->Disconnect( wxID_SAVE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::menuFileSaveOnMenuSelection ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::menuReloadOnMenuSelection ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::menuAutoDLLOnMenuSelection ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::menuFileExpandOnMenuSelection ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::menuFileExpandAllOnMenuSelection ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::menuFileExitOnMenuSelection ) );
	this->Disconnect( m_toolOpen->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::toolOpenOnToolClicked ) );
	this->Disconnect( m_toolSave->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::toolSaveOnToolClicked ) );
	this->Disconnect( m_toolReload->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::toolReloadOnToolClicked ) );
	this->Disconnect( m_toolAutoDLL->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::toolAutoDLLOnToolClicked ) );
	this->Disconnect( m_toolExpand->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::toolExpandOnToolClicked ) );
	this->Disconnect( m_toolExpandAll->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::toolExpandAllOnToolClicked ) );
	m_dirExplorer->Disconnect( wxEVT_COMMAND_TREE_ITEM_ACTIVATED, wxTreeEventHandler( MainFrameBase::dirExplorerOnTreeItemActivated ), NULL, this );
	m_buttonAuto->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::buttonAutoOnButtonClick ), NULL, this );
	m_buttonAutoNewDll->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::buttonAutoNewDllOnButtonClick ), NULL, this );
	m_buttonAddDef->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::buttonAddDefOnButtonClick ), NULL, this );
	m_comboConfig->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( MainFrameBase::comboConfigOnCombobox ), NULL, this );
	m_checkAll->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MainFrameBase::checkAllOnCheckBox ), NULL, this );
	m_checkDll->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MainFrameBase::checkDllOnCheckBox ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler( MainFrameBase::dvlistMainSettingOnDataViewListCtrlItemActivated ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_DATAVIEW_ITEM_EDITING_DONE, wxDataViewEventHandler( MainFrameBase::dvlistMainSettingOnDataViewListCtrlItemEditingDone ) );
	
	m_mgr.UnInit();
	
}
