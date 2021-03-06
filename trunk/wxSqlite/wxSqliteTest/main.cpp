/*********************************************************************
 * Name:      	main.cpp
 * Purpose:   	Implements simple wxWidgets application with GUI
 * 				created using wxFormBuilder.
 * Author:    
 * Created:   
 * Copyright: 
 * License:   	wxWidgets license (www.wxwidgets.org)
 * 
 * Notes:		Note that all GUI creation code is implemented in
 * 				gui.cpp source file which is generated by wxFormBuilder.
 *********************************************************************/

#include "main.h"
#include <string>
#include <deque>
using namespace std;

// initialize the application
IMPLEMENT_APP(MainApp);

////////////////////////////////////////////////////////////////////////////////
// application class implementation 
////////////////////////////////////////////////////////////////////////////////

bool MainApp::OnInit()
{
	SetTopWindow( new MainFrame( NULL ) );
	GetTopWindow()->Show();
	
	// true = enter the main loop
	return true;
}

////////////////////////////////////////////////////////////////////////////////
// main application frame implementation 
////////////////////////////////////////////////////////////////////////////////
#include "..\wxCommon\wxCommon.h"
#include "Table.h"
using namespace wxCommon;

MainFrame::MainFrame(wxWindow *parent) : MainFrameBase( parent )
{
	m_config.ReadIni();

	m_treeDatabase->DeleteAllItems();
	std::deque<ConfigIni::Item>& dqItems = m_config.dqItems;
	for (size_t ii = 0, ni = dqItems.size();
		ii < ni; ++ii)
	{
		dqItems[ii].treeid = m_treeDatabase->AddRoot(dqItems[ii].name);
	}
}

MainFrame::~MainFrame()
{
	if (m_db.IsOpen())
	{
		m_db.Close();
	}
}

void MainFrame::OnCloseFrame(wxCloseEvent& event)
{
	Destroy();
}

void MainFrame::OnExitClick(wxCommandEvent& event)
{
	Destroy();
}

void MainFrame::LoadConfig(void)
{
	wxString strStatus;
	if (m_config.ReadIni())
	{
		strStatus = "Default loaded.";
	}
}

void MainFrame::m_treeDatabaseOnTreeItemActivated( wxTreeEvent& event )
{
	wxTreeItemId idchild = event.GetItem();
	wxTreeItemId idparent = m_treeDatabase->GetItemParent(idchild);
	wxTreeItemId iddb;

	bool bSelTable = idparent.IsOk();
	iddb = bSelTable ? idparent : idchild;
	ConfigIni::Item item = m_config.GetItem(iddb);
	if (item.entry == -1)
	{
		SetStatusText("get db item error.");
		return;
	}
	m_itemDB = item;
	LoadDB(!bSelTable);
	if (bSelTable)
	{
		m_table = m_treeDatabase->GetItemText(idchild);
		LoadTable();
	}
}


void MainFrame::LoadDB(bool bAutoExpand /*= true*/)
{
	if (m_db.IsOpen() && 
		(m_db.GetDBFile() == m_itemDB.path))
	{
		// db already loaded.
		if (bAutoExpand)
		{
			wxTreeItemId id = m_itemDB.treeid;
			if (m_treeDatabase->IsExpanded(id))
			{
				m_treeDatabase->Collapse(id);
			}
			else
			{
				m_treeDatabase->Expand(id);
			}
		}
		return;
	}
	int nRet = 0;
	nRet = m_db.Open(m_itemDB.path.utf8_str().data());
	nRet = m_db.LoadSchema();

	m_treeDatabase->DeleteChildren(m_itemDB.treeid);

	deque<string> dqTables = m_db.GetTables();
	for (size_t ii = 0, ni = dqTables.size(); 
		ii < ni; ++ii)
	{
		m_treeDatabase->AppendItem(m_itemDB.treeid, dqTables[ii]);
	}
	m_treeDatabase->Expand(m_itemDB.treeid);
}


void MainFrame::LoadTable(void)
{
	sqlite3* pdb  = m_db.GetDBPtr();
	if (!pdb)
	{
		return;
	}
	m_dvTable->ClearColumns();
	m_dvTable->DeleteAllItems();
	TTable table;
	int nRet = table.Load(pdb, m_table.c_str());
	SetDvlistHeader(m_dvTable, table.colname);
	for (size_t ii = 0, ni  = table.records.size();
		ii < ni; ++ii)
	{
		TTable::DQString& record = table.records[ii];
		wxVector<wxVariant> vtItem;
		for (size_t jj = 0, nj = record.size();
			jj < nj; ++jj)
		{
			vtItem.push_back(record[jj]);
		}
		m_dvTable->AppendItem(vtItem);
	}
	
}