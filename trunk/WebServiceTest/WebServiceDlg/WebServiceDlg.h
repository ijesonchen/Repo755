// WebServiceDlg.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CWebServiceDlgApp:
// �йش����ʵ�֣������ WebServiceDlg.cpp
//

class CWebServiceDlgApp : public CWinApp
{
public:
	CWebServiceDlgApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CWebServiceDlgApp theApp;