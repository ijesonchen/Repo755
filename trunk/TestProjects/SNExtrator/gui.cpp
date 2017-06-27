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
	this->SetMenuBar( m_menubar );
	
	m_dirExplorer = new wxGenericDirCtrl( this, wxID_ANY, _("D:"), wxDefaultPosition, wxDefaultSize, wxDIRCTRL_3D_INTERNAL|wxSUNKEN_BORDER, wxEmptyString, 0 );
	
	m_dirExplorer->ShowHidden( false );
	m_mgr.AddPane( m_dirExplorer, wxAuiPaneInfo() .Left() .PinButton( true ).Dock().Resizable().FloatingSize( wxSize( 200,-1 ) ).DockFixed( false ).BestSize( wxSize( 300,-1 ) ) );
	
	m_panel2 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_panel2, wxAuiPaneInfo() .Center() .PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ).DockFixed( false ) );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1 = new wxStaticText( m_panel2, wxID_ANY, _("Folder"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer1->Add( m_staticText1, 0, wxALL, 5 );
	
	m_textCtrl1 = new wxTextCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_textCtrl1, 0, wxALL, 5 );
	
	m_staticText2 = new wxStaticText( m_panel2, wxID_ANY, _("RegEx"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer1->Add( m_staticText2, 0, wxALL, 5 );
	
	m_textCtrl2 = new wxTextCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_textCtrl2, 0, wxALL, 5 );
	
	m_staticText4 = new wxStaticText( m_panel2, wxID_ANY, _("MoveTo"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	fgSizer1->Add( m_staticText4, 0, wxALL, 5 );
	
	m_textCtrl3 = new wxTextCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_textCtrl3, 0, wxALL, 5 );
	
	
	bSizer1->Add( fgSizer1, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText3 = new wxStaticText( m_panel2, wxID_ANY, _("Extents"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer2->Add( m_staticText3, 0, wxALL, 5 );
	
	wxGridBagSizer* gbSizer1;
	gbSizer1 = new wxGridBagSizer( 0, 0 );
	gbSizer1->SetFlexibleDirection( wxVERTICAL );
	gbSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );
	
	m_checkBox1 = new wxCheckBox( m_panel2, wxID_ANY, _("Check Me!"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( m_checkBox1, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_checkBox2 = new wxCheckBox( m_panel2, wxID_ANY, _("Check Me!"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( m_checkBox2, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_checkBox3 = new wxCheckBox( m_panel2, wxID_ANY, _("Check Me!"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer1->Add( m_checkBox3, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	
	bSizer2->Add( gbSizer1, 1, wxEXPAND, 5 );
	
	
	bSizer1->Add( bSizer2, 0, wxEXPAND, 5 );
	
	m_dvlistFiles = new wxDataViewListCtrl( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_dvlistFiles, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panel2->SetSizer( bSizer1 );
	m_panel2->Layout();
	bSizer1->Fit( m_panel2 );
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	m_mgr.Update();
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::OnCloseFrame ) );
	m_dirExplorer->Connect( wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK, wxTreeEventHandler( MainFrameBase::m_dirExplorerOnTreeItemRightClick ), NULL, this );
}

MainFrameBase::~MainFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::OnCloseFrame ) );
	m_dirExplorer->Disconnect( wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK, wxTreeEventHandler( MainFrameBase::m_dirExplorerOnTreeItemRightClick ), NULL, this );
	
	m_mgr.UnInit();
	
}
