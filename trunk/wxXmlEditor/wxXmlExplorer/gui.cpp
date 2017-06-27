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
	this->SetSizeHints( wxSize( -1,100 ), wxSize( -1,-1 ) );
	
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
	
	m_treeXml = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTR_DEFAULT_STYLE|wxTR_EDIT_LABELS|wxTR_HAS_BUTTONS|wxTR_TWIST_BUTTONS|wxSUNKEN_BORDER );
	mainSizer->Add( m_treeXml, 2, wxALL|wxEXPAND, 5 );
	
	m_textMessage = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,30 ), wxTE_MULTILINE|wxTE_NOHIDESEL|wxTE_READONLY|wxTE_WORDWRAP|wxSUNKEN_BORDER );
	m_textMessage->SetMinSize( wxSize( -1,30 ) );
	m_textMessage->SetMaxSize( wxSize( -1,50 ) );
	
	mainSizer->Add( m_textMessage, 1, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( mainSizer );
	this->Layout();
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::OnCloseFrame ) );
	this->Connect( menuFileOpen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnMenuFileOpen ) );
	this->Connect( m_menuFileExpand->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnMenuFileExpand ) );
	this->Connect( menuFileExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnExitClick ) );
	m_treeXml->Connect( wxEVT_COMMAND_TREE_KEY_DOWN, wxTreeEventHandler( MainFrameBase::m_xmlTreeCtrlOnTreeKeyDown ), NULL, this );
	m_treeXml->Connect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( MainFrameBase::m_xmlTreeCtrlOnTreeSelChanged ), NULL, this );
}

MainFrameBase::~MainFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::OnCloseFrame ) );
	this->Disconnect( wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnMenuFileOpen ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnMenuFileExpand ) );
	this->Disconnect( wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnExitClick ) );
	m_treeXml->Disconnect( wxEVT_COMMAND_TREE_KEY_DOWN, wxTreeEventHandler( MainFrameBase::m_xmlTreeCtrlOnTreeKeyDown ), NULL, this );
	m_treeXml->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( MainFrameBase::m_xmlTreeCtrlOnTreeSelChanged ), NULL, this );
	
}
