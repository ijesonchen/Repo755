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
	
	m_mbarMain = new wxMenuBar( 0 );
	m_menuFile = new wxMenu();
	wxMenuItem* m_miOpen;
	m_miOpen = new wxMenuItem( m_menuFile, wxID_OPEN, wxString( _("Open") ) + wxT('\t') + wxT("alt+o"), wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_miOpen->SetBitmaps( wxArtProvider::GetBitmap( wxART_FILE_OPEN, wxART_MENU ) );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_miOpen->SetBitmap( wxArtProvider::GetBitmap( wxART_FILE_OPEN, wxART_MENU ) );
	#endif
	m_menuFile->Append( m_miOpen );
	
	m_mbarMain->Append( m_menuFile, _("MyMenu") ); 
	
	this->SetMenuBar( m_mbarMain );
	
	m_auiToolBar = new wxAuiToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_HORZ_LAYOUT ); 
	m_toolOpen = m_auiToolBar->AddTool( wxID_OPEN, _("Open"), wxArtProvider::GetBitmap( wxART_FILE_OPEN, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, _("Open File"), _("Open Sqlite DB file"), NULL ); 
	
	m_auiToolBar->Realize();
	m_mgr.AddPane( m_auiToolBar, wxAuiPaneInfo().Top().CaptionVisible( false ).CloseButton( false ).PinButton( true ).PaneBorder( false ).Dock().Resizable().FloatingSize( wxDefaultSize ).DockFixed( false ).Layer( 10 ).ToolbarPane() );
	
	m_treeDatabase = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( 200,-1 ), wxTR_DEFAULT_STYLE );
	m_mgr.AddPane( m_treeDatabase, wxAuiPaneInfo() .Left() .CloseButton( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ).DockFixed( false ) );
	
	m_nbMain = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_mgr.AddPane( m_nbMain, wxAuiPaneInfo() .Center() .CloseButton( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ).DockFixed( false ).CentrePane() );
	
	m_panelTable = new wxPanel( m_nbMain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_dvTable = new wxDataViewListCtrl( m_panelTable, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_dvTable, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panelTable->SetSizer( bSizer1 );
	m_panelTable->Layout();
	bSizer1->Fit( m_panelTable );
	m_nbMain->AddPage( m_panelTable, _("Table"), true );
	m_panelQuery = new wxPanel( m_nbMain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonQuery = new wxButton( m_panelQuery, wxID_ANY, _("Query"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_buttonQuery, 0, wxALL, 5 );
	
	
	bSizer2->Add( bSizer3, 0, wxEXPAND, 5 );
	
	m_scintilla1 = new wxStyledTextCtrl( m_panelQuery, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), 0, wxEmptyString );
	m_scintilla1->SetUseTabs( true );
	m_scintilla1->SetTabWidth( 4 );
	m_scintilla1->SetIndent( 4 );
	m_scintilla1->SetTabIndents( true );
	m_scintilla1->SetBackSpaceUnIndents( true );
	m_scintilla1->SetViewEOL( false );
	m_scintilla1->SetViewWhiteSpace( false );
	m_scintilla1->SetMarginWidth( 2, 0 );
	m_scintilla1->SetIndentationGuides( true );
	m_scintilla1->SetMarginType( 1, wxSTC_MARGIN_SYMBOL );
	m_scintilla1->SetMarginMask( 1, wxSTC_MASK_FOLDERS );
	m_scintilla1->SetMarginWidth( 1, 16);
	m_scintilla1->SetMarginSensitive( 1, true );
	m_scintilla1->SetProperty( wxT("fold"), wxT("1") );
	m_scintilla1->SetFoldFlags( wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED );
	m_scintilla1->SetMarginType( 0, wxSTC_MARGIN_NUMBER );
	m_scintilla1->SetMarginWidth( 0, m_scintilla1->TextWidth( wxSTC_STYLE_LINENUMBER, wxT("_99999") ) );
	m_scintilla1->MarkerDefine( wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS );
	m_scintilla1->MarkerSetBackground( wxSTC_MARKNUM_FOLDER, wxColour( wxT("BLACK") ) );
	m_scintilla1->MarkerSetForeground( wxSTC_MARKNUM_FOLDER, wxColour( wxT("WHITE") ) );
	m_scintilla1->MarkerDefine( wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS );
	m_scintilla1->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPEN, wxColour( wxT("BLACK") ) );
	m_scintilla1->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPEN, wxColour( wxT("WHITE") ) );
	m_scintilla1->MarkerDefine( wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY );
	m_scintilla1->MarkerDefine( wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUS );
	m_scintilla1->MarkerSetBackground( wxSTC_MARKNUM_FOLDEREND, wxColour( wxT("BLACK") ) );
	m_scintilla1->MarkerSetForeground( wxSTC_MARKNUM_FOLDEREND, wxColour( wxT("WHITE") ) );
	m_scintilla1->MarkerDefine( wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUS );
	m_scintilla1->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPENMID, wxColour( wxT("BLACK") ) );
	m_scintilla1->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPENMID, wxColour( wxT("WHITE") ) );
	m_scintilla1->MarkerDefine( wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY );
	m_scintilla1->MarkerDefine( wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY );
	m_scintilla1->SetSelBackground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );
	m_scintilla1->SetSelForeground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHTTEXT ) );
	m_scintilla1->SetMaxSize( wxSize( -1,200 ) );
	
	bSizer2->Add( m_scintilla1, 1, wxEXPAND | wxALL, 5 );
	
	m_dataViewListCtrl3 = new wxDataViewListCtrl( m_panelQuery, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_dataViewListCtrl3, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panelQuery->SetSizer( bSizer2 );
	m_panelQuery->Layout();
	bSizer2->Fit( m_panelQuery );
	m_nbMain->AddPage( m_panelQuery, _("Query"), false );
	
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	m_mgr.Update();
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::OnCloseFrame ) );
	this->Connect( m_miOpen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::m_miOpenOnMenuSelection ) );
	this->Connect( m_toolOpen->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::m_toolOpenOnToolClicked ) );
	m_treeDatabase->Connect( wxEVT_COMMAND_TREE_ITEM_ACTIVATED, wxTreeEventHandler( MainFrameBase::m_treeDatabaseOnTreeItemActivated ), NULL, this );
}

MainFrameBase::~MainFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::OnCloseFrame ) );
	this->Disconnect( wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::m_miOpenOnMenuSelection ) );
	this->Disconnect( m_toolOpen->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::m_toolOpenOnToolClicked ) );
	m_treeDatabase->Disconnect( wxEVT_COMMAND_TREE_ITEM_ACTIVATED, wxTreeEventHandler( MainFrameBase::m_treeDatabaseOnTreeItemActivated ), NULL, this );
	
	m_mgr.UnInit();
	
}
