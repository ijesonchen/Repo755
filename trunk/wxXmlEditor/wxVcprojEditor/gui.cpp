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
	
	m_menuBar = new wxMenuBar( 0 );
	m_menuFile = new wxMenu();
	wxMenuItem* menuFileOpen;
	menuFileOpen = new wxMenuItem( m_menuFile, wxID_OPEN, wxString( _("&Open") ) + wxT('\t') + wxT("Alt+O"), wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( menuFileOpen );
	
	wxMenuItem* m_menuFileExpand;
	m_menuFileExpand = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("&ExpandChild") ) + wxT('\t') + wxT("Alt+E"), _("Expand the node"), wxITEM_NORMAL );
	m_menuFile->Append( m_menuFileExpand );
	
	wxMenuItem* menuFileExit;
	menuFileExit = new wxMenuItem( m_menuFile, wxID_EXIT, wxString( _("E&xit") ) + wxT('\t') + wxT("Alt+X"), wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( menuFileExit );
	
	m_menuBar->Append( m_menuFile, _("&File") ); 
	
	this->SetMenuBar( m_menuBar );
	
	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	m_dirExplorer = new wxGenericDirCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRCTRL_3D_INTERNAL|wxDIRCTRL_SHOW_FILTERS|wxSUNKEN_BORDER, wxEmptyString, 0 );
	
	m_dirExplorer->ShowHidden( false );
	bSizer4->Add( m_dirExplorer, 1, wxEXPAND | wxALL, 5 );
	
	m_treeXml = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
	bSizer4->Add( m_treeXml, 2, wxALL|wxEXPAND, 5 );
	
	
	mainSizer->Add( bSizer4, 1, wxEXPAND, 5 );
	
	m_nbConfigs = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panelGeneral = new wxPanel( m_nbConfigs, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panelGeneral->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	m_panelGeneral->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER ) );
	
	wxBoxSizer* sizerBookGeneral;
	sizerBookGeneral = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* sizerConfig;
	sizerConfig = new wxBoxSizer( wxHORIZONTAL );
	
	m_comboConfig = new wxComboBox( m_panelGeneral, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	sizerConfig->Add( m_comboConfig, 0, wxALL, 5 );
	
	m_buttonDefault = new wxButton( m_panelGeneral, wxID_ANY, _("Default"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerConfig->Add( m_buttonDefault, 0, wxALL, 5 );
	
	m_buttonSave = new wxButton( m_panelGeneral, wxID_ANY, _("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerConfig->Add( m_buttonSave, 0, wxALL, 5 );
	
	
	sizerBookGeneral->Add( sizerConfig, 0, wxEXPAND, 5 );
	
	m_dvlistConfig = new wxDataViewListCtrl( m_panelGeneral, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_SINGLE );
	m_dvlistConfig->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT ) );
	
	sizerBookGeneral->Add( m_dvlistConfig, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panelGeneral->SetSizer( sizerBookGeneral );
	m_panelGeneral->Layout();
	sizerBookGeneral->Fit( m_panelGeneral );
	m_nbConfigs->AddPage( m_panelGeneral, _("General"), true );
	
	mainSizer->Add( m_nbConfigs, 1, wxEXPAND | wxALL, 5 );
	
	
	this->SetSizer( mainSizer );
	this->Layout();
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::OnCloseFrame ) );
	this->Connect( menuFileOpen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnMenuFileOpen ) );
	this->Connect( m_menuFileExpand->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnMenuFileExpand ) );
	this->Connect( menuFileExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnExitClick ) );
	m_dirExplorer->Connect( wxEVT_COMMAND_TREE_ITEM_ACTIVATED, wxTreeEventHandler( MainFrameBase::dirExplorerOnTreeItemActivated ), NULL, this );
	m_buttonDefault->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::buttonDefaultOnButtonClick ), NULL, this );
	m_buttonSave->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::buttonSaveOnButtonClick ), NULL, this );
	this->Connect( wxID_ANY, wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler( MainFrameBase::dvlistConfigOnDataViewListCtrlItemActivated ) );
	this->Connect( wxID_ANY, wxEVT_COMMAND_DATAVIEW_ITEM_EDITING_DONE, wxDataViewEventHandler( MainFrameBase::dvlistConfigOnDataViewListCtrlItemEditingDone ) );
}

MainFrameBase::~MainFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::OnCloseFrame ) );
	this->Disconnect( wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnMenuFileOpen ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnMenuFileExpand ) );
	this->Disconnect( wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnExitClick ) );
	m_dirExplorer->Disconnect( wxEVT_COMMAND_TREE_ITEM_ACTIVATED, wxTreeEventHandler( MainFrameBase::dirExplorerOnTreeItemActivated ), NULL, this );
	m_buttonDefault->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::buttonDefaultOnButtonClick ), NULL, this );
	m_buttonSave->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::buttonSaveOnButtonClick ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler( MainFrameBase::dvlistConfigOnDataViewListCtrlItemActivated ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_DATAVIEW_ITEM_EDITING_DONE, wxDataViewEventHandler( MainFrameBase::dvlistConfigOnDataViewListCtrlItemEditingDone ) );
	
}
