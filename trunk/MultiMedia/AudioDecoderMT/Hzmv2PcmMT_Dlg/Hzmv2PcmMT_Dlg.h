
// Hzmv2PcmMT_Dlg.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CHzmv2PcmMT_DlgApp:
// See Hzmv2PcmMT_Dlg.cpp for the implementation of this class
//

class CHzmv2PcmMT_DlgApp : public CWinAppEx
{
public:
	CHzmv2PcmMT_DlgApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CHzmv2PcmMT_DlgApp theApp;