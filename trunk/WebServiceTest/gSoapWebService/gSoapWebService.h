
// gSoapWebService.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CgSoapWebServiceApp:
// �йش����ʵ�֣������ gSoapWebService.cpp
//

class CgSoapWebServiceApp : public CWinAppEx
{
public:
	CgSoapWebServiceApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CgSoapWebServiceApp theApp;