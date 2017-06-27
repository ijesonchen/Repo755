///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
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
	
	m_auiToolBar = new wxAuiToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_HORZ_LAYOUT ); 
	m_toolReload = m_auiToolBar->AddTool( wxID_ANY, wxT("tool"), wxArtProvider::GetBitmap( wxART_REDO, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_toolFilter = m_auiToolBar->AddTool( wxID_ANY, wxT("tool"), wxArtProvider::GetBitmap( wxART_EXECUTABLE_FILE, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_auiToolBar->Realize();
	m_mgr.AddPane( m_auiToolBar, wxAuiPaneInfo().Top().CaptionVisible( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxSize( 37,46 ) ) );
	
	m_textFilename = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_mgr.AddPane( m_textFilename, wxAuiPaneInfo() .Top() .CaptionVisible( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ).Row( 1 ) );
	
	m_textSearch = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_mgr.AddPane( m_textSearch, wxAuiPaneInfo() .Top() .CaptionVisible( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ).Row( 2 ) );
	
	m_buttonSearch = new wxButton( this, wxID_ANY, wxT("Search"), wxDefaultPosition, wxDefaultSize, 0 );
	m_mgr.AddPane( m_buttonSearch, wxAuiPaneInfo() .Top() .CaptionVisible( false ).PinButton( true ).Dock().Fixed().Row( 2 ) );
	
	m_spinRefreshSecs = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 600, 10 );
	m_mgr.AddPane( m_spinRefreshSecs, wxAuiPaneInfo() .Top() .CaptionVisible( false ).PinButton( true ).Dock().Fixed().Row( 2 ) );
	
	m_checkAutoRefresh = new wxCheckBox( this, wxID_ANY, wxT("AutoRefresh"), wxDefaultPosition, wxSize( -1,-1 ), wxCHK_2STATE );
	m_mgr.AddPane( m_checkAutoRefresh, wxAuiPaneInfo() .Top() .CaptionVisible( false ).PinButton( true ).Dock().Fixed().Row( 2 ) );
	
	m_checkAllTag = new wxCheckBox( this, wxID_ANY, wxT("Check All Tag"), wxDefaultPosition, wxSize( -1,-1 ), wxCHK_2STATE );
	m_checkAllTag->SetValue(true); 
	m_mgr.AddPane( m_checkAllTag, wxAuiPaneInfo() .Left() .CaptionVisible( false ).PinButton( true ).Dock().Fixed() );
	
	wxArrayString m_ckListTagChoices;
	m_ckListTag = new wxCheckListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_ckListTagChoices, 0 );
	m_mgr.AddPane( m_ckListTag, wxAuiPaneInfo() .Left() .Caption( wxT("Tag") ).CloseButton( false ).Dock().Resizable().FloatingSize( wxDefaultSize ) );
	
	wxArrayString m_ckListCategoryChoices;
	m_ckListCategory = new wxCheckListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_ckListCategoryChoices, wxLB_MULTIPLE|wxLB_NEEDED_SB );
	m_mgr.AddPane( m_ckListCategory, wxAuiPaneInfo() .Left() .Caption( wxT("Category") ).CloseButton( false ).Dock().Resizable().FloatingSize( wxDefaultSize ) );
	
	m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_panel1, wxAuiPaneInfo() .Center() .CaptionVisible( false ).CloseButton( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ).CentrePane() );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_dvLogs = new wxDataViewListCtrl( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_dvLogs, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panel1->SetSizer( bSizer1 );
	m_panel1->Layout();
	bSizer1->Fit( m_panel1 );
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	m_timerRefresh.SetOwner( this, wxID_ANY );
	
	m_mgr.Update();
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( m_toolReload->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::m_toolReloadOnToolClicked ) );
	this->Connect( m_toolFilter->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::m_toolFilterOnToolClicked ) );
	m_buttonSearch->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::m_buttonSearchOnButtonClick ), NULL, this );
	m_checkAutoRefresh->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MainFrameBase::m_checkAutoRefreshOnCheckBox ), NULL, this );
	m_checkAllTag->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MainFrameBase::m_checkAllTagOnCheckBox ), NULL, this );
	this->Connect( wxID_ANY, wxEVT_TIMER, wxTimerEventHandler( MainFrameBase::m_timerRefreshOnTimer ) );
}

MainFrameBase::~MainFrameBase()
{
	// Disconnect Events
	this->Disconnect( m_toolReload->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::m_toolReloadOnToolClicked ) );
	this->Disconnect( m_toolFilter->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::m_toolFilterOnToolClicked ) );
	m_buttonSearch->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::m_buttonSearchOnButtonClick ), NULL, this );
	m_checkAutoRefresh->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MainFrameBase::m_checkAutoRefreshOnCheckBox ), NULL, this );
	m_checkAllTag->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MainFrameBase::m_checkAllTagOnCheckBox ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_TIMER, wxTimerEventHandler( MainFrameBase::m_timerRefreshOnTimer ) );
	
	m_mgr.UnInit();
	
}
