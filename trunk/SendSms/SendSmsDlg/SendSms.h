
// SendSms.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSendSmsApp:
// �йش����ʵ�֣������ SendSms.cpp
//

class CSendSmsApp : public CWinAppEx
{
public:
	CSendSmsApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSendSmsApp theApp;