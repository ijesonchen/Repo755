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
	
	m_auiToolBar = new wxAuiToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_HORZ_LAYOUT ); 
	m_toolTest = m_auiToolBar->AddTool( wxID_ANY, _("toolTest"), wxArtProvider::GetBitmap( wxART_EXECUTABLE_FILE, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_auiToolBar->Realize();
	m_mgr.AddPane( m_auiToolBar, wxAuiPaneInfo().Top().CloseButton( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ).DockFixed( false ).Layer( 10 ).ToolbarPane() );
	
	m_stcLog = new wxStyledTextCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, wxEmptyString );
	m_stcLog->SetUseTabs( true );
	m_stcLog->SetTabWidth( 4 );
	m_stcLog->SetIndent( 4 );
	m_stcLog->SetTabIndents( true );
	m_stcLog->SetBackSpaceUnIndents( true );
	m_stcLog->SetViewEOL( false );
	m_stcLog->SetViewWhiteSpace( false );
	m_stcLog->SetMarginWidth( 2, 0 );
	m_stcLog->SetIndentationGuides( true );
	m_stcLog->SetMarginType( 1, wxSTC_MARGIN_SYMBOL );
	m_stcLog->SetMarginMask( 1, wxSTC_MASK_FOLDERS );
	m_stcLog->SetMarginWidth( 1, 16);
	m_stcLog->SetMarginSensitive( 1, true );
	m_stcLog->SetProperty( wxT("fold"), wxT("1") );
	m_stcLog->SetFoldFlags( wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED );
	m_stcLog->SetMarginType( 0, wxSTC_MARGIN_NUMBER );
	m_stcLog->SetMarginWidth( 0, m_stcLog->TextWidth( wxSTC_STYLE_LINENUMBER, wxT("_99999") ) );
	m_stcLog->MarkerDefine( wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS );
	m_stcLog->MarkerSetBackground( wxSTC_MARKNUM_FOLDER, wxColour( wxT("BLACK") ) );
	m_stcLog->MarkerSetForeground( wxSTC_MARKNUM_FOLDER, wxColour( wxT("WHITE") ) );
	m_stcLog->MarkerDefine( wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS );
	m_stcLog->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPEN, wxColour( wxT("BLACK") ) );
	m_stcLog->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPEN, wxColour( wxT("WHITE") ) );
	m_stcLog->MarkerDefine( wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY );
	m_stcLog->MarkerDefine( wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUS );
	m_stcLog->MarkerSetBackground( wxSTC_MARKNUM_FOLDEREND, wxColour( wxT("BLACK") ) );
	m_stcLog->MarkerSetForeground( wxSTC_MARKNUM_FOLDEREND, wxColour( wxT("WHITE") ) );
	m_stcLog->MarkerDefine( wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUS );
	m_stcLog->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPENMID, wxColour( wxT("BLACK") ) );
	m_stcLog->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPENMID, wxColour( wxT("WHITE") ) );
	m_stcLog->MarkerDefine( wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY );
	m_stcLog->MarkerDefine( wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY );
	m_stcLog->SetSelBackground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );
	m_stcLog->SetSelForeground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHTTEXT ) );
	m_mgr.AddPane( m_stcLog, wxAuiPaneInfo() .Left() .Caption( _("Log") ).CloseButton( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ).DockFixed( false ).CentrePane() );
	
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	m_mgr.Update();
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::OnCloseFrame ) );
	this->Connect( m_toolTest->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::m_toolTestOnToolClicked ) );
}

MainFrameBase::~MainFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::OnCloseFrame ) );
	this->Disconnect( m_toolTest->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::m_toolTestOnToolClicked ) );
	
	m_mgr.UnInit();
	
}
