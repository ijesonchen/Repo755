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
	
	m_filePicker = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, _("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	m_mgr.AddPane( m_filePicker, wxAuiPaneInfo() .Top() .CaptionVisible( false ).CloseButton( false ).Dock().Resizable().FloatingSize( wxDefaultSize ).DockFixed( false ) );
	
	m_notebookMain = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_mgr.AddPane( m_notebookMain, wxAuiPaneInfo() .Center() .CaptionVisible( false ).CloseButton( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ).DockFixed( false ) );
	
	m_penelDetail = new wxPanel( m_notebookMain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	m_buttonAddDet = new wxButton( m_penelDetail, wxID_ANY, _("增加"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_buttonAddDet, 0, wxALL, 5 );
	
	m_buttonUpdateDet = new wxButton( m_penelDetail, wxID_ANY, _("修改"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_buttonUpdateDet, 0, wxALL, 5 );
	
	m_buttonRemoveDet = new wxButton( m_penelDetail, wxID_ANY, _("删除"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_buttonRemoveDet, 0, wxALL, 5 );
	
	m_buttonListDet = new wxButton( m_penelDetail, wxID_ANY, _("列表"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_buttonListDet, 0, wxALL, 5 );
	
	m_datePickerBeginDet = new wxDatePickerCtrl( m_penelDetail, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT );
	bSizer2->Add( m_datePickerBeginDet, 0, wxALL, 5 );
	
	m_datePickerEndDet = new wxDatePickerCtrl( m_penelDetail, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT );
	bSizer2->Add( m_datePickerEndDet, 0, wxALL, 5 );
	
	
	bSizer1->Add( bSizer2, 0, wxEXPAND, 5 );
	
	m_dvlistDetail = new wxDataViewListCtrl( m_penelDetail, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_dvlistDetail, 1, wxALL|wxEXPAND, 5 );
	
	
	m_penelDetail->SetSizer( bSizer1 );
	m_penelDetail->Layout();
	bSizer1->Fit( m_penelDetail );
	m_notebookMain->AddPage( m_penelDetail, _("明细"), false );
	m_panelAccount = new wxPanel( m_notebookMain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxVERTICAL );
	
	m_buttonAddAcc = new wxButton( m_panelAccount, wxID_ANY, _("增加"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( m_buttonAddAcc, 0, wxALL, 5 );
	
	m_buttonUpdateAcc = new wxButton( m_panelAccount, wxID_ANY, _("修改"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( m_buttonUpdateAcc, 0, wxALL, 5 );
	
	m_buttonRemoveAcc = new wxButton( m_panelAccount, wxID_ANY, _("删除"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( m_buttonRemoveAcc, 0, wxALL, 5 );
	
	m_buttonListAcc = new wxButton( m_panelAccount, wxID_ANY, _("列表"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( m_buttonListAcc, 0, wxALL, 5 );
	
	
	bSizer3->Add( bSizer21, 0, wxEXPAND, 5 );
	
	m_dvlistAccount = new wxDataViewListCtrl( m_panelAccount, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_dvlistAccount, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panelAccount->SetSizer( bSizer3 );
	m_panelAccount->Layout();
	bSizer3->Fit( m_panelAccount );
	m_notebookMain->AddPage( m_panelAccount, _("账户"), false );
	
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	m_mgr.Update();
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_ACTIVATE, wxActivateEventHandler( MainFrameBase::MainFrameBaseOnActivate ) );
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::OnCloseFrame ) );
	m_filePicker->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( MainFrameBase::m_filePickerOnFileChanged ), NULL, this );
	m_buttonAddDet->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::m_buttonAddDetOnButtonClick ), NULL, this );
	m_buttonUpdateDet->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::m_buttonUpdateDetOnButtonClick ), NULL, this );
	m_buttonRemoveDet->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::m_buttonRemoveDetOnButtonClick ), NULL, this );
	m_buttonListDet->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::m_buttonListDetOnButtonClick ), NULL, this );
	m_buttonAddAcc->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::m_buttonAddAccOnButtonClick ), NULL, this );
	m_buttonUpdateAcc->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::m_buttonUpdateAccOnButtonClick ), NULL, this );
	m_buttonRemoveAcc->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::m_buttonRemoveAccOnButtonClick ), NULL, this );
	m_buttonListAcc->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::m_buttonListAccOnButtonClick ), NULL, this );
}

MainFrameBase::~MainFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_ACTIVATE, wxActivateEventHandler( MainFrameBase::MainFrameBaseOnActivate ) );
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::OnCloseFrame ) );
	m_filePicker->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( MainFrameBase::m_filePickerOnFileChanged ), NULL, this );
	m_buttonAddDet->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::m_buttonAddDetOnButtonClick ), NULL, this );
	m_buttonUpdateDet->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::m_buttonUpdateDetOnButtonClick ), NULL, this );
	m_buttonRemoveDet->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::m_buttonRemoveDetOnButtonClick ), NULL, this );
	m_buttonListDet->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::m_buttonListDetOnButtonClick ), NULL, this );
	m_buttonAddAcc->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::m_buttonAddAccOnButtonClick ), NULL, this );
	m_buttonUpdateAcc->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::m_buttonUpdateAccOnButtonClick ), NULL, this );
	m_buttonRemoveAcc->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::m_buttonRemoveAccOnButtonClick ), NULL, this );
	m_buttonListAcc->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::m_buttonListAccOnButtonClick ), NULL, this );
	
	m_mgr.UnInit();
	
}

DlgDetailBase::DlgDetailBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer3->AddGrowableCol( 1 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText23 = new wxStaticText( this, wxID_ANY, _("从账户"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText23->Wrap( -1 );
	fgSizer3->Add( m_staticText23, 0, wxALL, 5 );
	
	m_comboBox11 = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizer3->Add( m_comboBox11, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText24 = new wxStaticText( this, wxID_ANY, _("到账户"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24->Wrap( -1 );
	fgSizer3->Add( m_staticText24, 0, wxALL, 5 );
	
	m_comboBox12 = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizer3->Add( m_comboBox12, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText25 = new wxStaticText( this, wxID_ANY, _("日期"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText25->Wrap( -1 );
	fgSizer3->Add( m_staticText25, 0, wxALL, 5 );
	
	m_datePicker3 = new wxDatePickerCtrl( this, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT );
	fgSizer3->Add( m_datePicker3, 0, wxALL, 5 );
	
	m_staticText26 = new wxStaticText( this, wxID_ANY, _("金额"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText26->Wrap( -1 );
	fgSizer3->Add( m_staticText26, 0, wxALL, 5 );
	
	m_textCtrl1 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_textCtrl1, 0, wxALL, 5 );
	
	m_staticText27 = new wxStaticText( this, wxID_ANY, _("备注"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText27->Wrap( -1 );
	fgSizer3->Add( m_staticText27, 0, wxALL, 5 );
	
	m_textCtrl2 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_textCtrl2, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText28 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText28->Wrap( -1 );
	fgSizer3->Add( m_staticText28, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonOK = new wxButton( this, wxID_ANY, _("确定"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer10->Add( m_buttonOK, 0, wxALL, 5 );
	
	m_buttonCancel = new wxButton( this, wxID_ANY, _("取消"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer10->Add( m_buttonCancel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer3->Add( bSizer10, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer3 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( DlgDetailBase::DlgDetailBaseOnInitDialog ) );
}

DlgDetailBase::~DlgDetailBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( DlgDetailBase::DlgDetailBaseOnInitDialog ) );
	
}
