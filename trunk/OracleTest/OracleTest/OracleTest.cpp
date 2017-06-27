// OracleTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "OracleTest.h"
#include "stdlib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// The one and only application object

CWinApp theApp;

#include <vector>
#include <deque>
using namespace std;

#import "c:\program files\common files\system\ado\msado15.dll" no_namespace rename("EOF", "adoEOF")

void sh(char* pstr)
{
	printf("%s\n", pstr);
}

void se(char* pstr)
{
	printf("error: %s\n", pstr);
}

typedef struct _VOCFILEINFO 
{
	CString strDir;
	CString strName;
	CString strExt;
}VocFileInfo;

#include "CommonFunction.h"
using namespace CommonFunction;

CString GetFileExt(const CString strFileName)
{
	int nIdx;
	nIdx = strFileName.ReverseFind('.');
	int nLength = strFileName.GetLength();

	if (nIdx > -1)
	{
		return strFileName.Right(nLength - nIdx - 1);
	}
	else
	{
		return _T("");
	}
}

void RemoveTailBlanks(CString& str)
{
	int nIndex = 0;
	while (str.GetLength() - 1 == 
		(nIndex = str.ReverseFind(' ')))
	{
		str.Delete(nIndex);
	}
}

CString GetFileNameNoExt(CString& str)
{
	CString strFileName;
	int nPosEnd = str.ReverseFind('.');
	if (-1 != nPosEnd)
	{
		strFileName = str.Left(nPosEnd);
	}
	return strFileName;
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.

		// get file list
		CString strPath;
		strPath = _T("\\\\dellserver1\\e$\\特种产品部 语音备份-57Test");
		CString strFileName;
		strFileName = _T("\\\\dellserver1\\e$\特种产品部语音备份-57Test\\test1\\000_20110410000511.wav");

		CString strList;
		strList = _T("..\\debug\\复件 00listsub.txt");

		CString strTmp;
		CString strTmp1;

		CString strRoot;
		CString strDir;
		CString strFullDir;
		CString strExt;
		CString strFileNameNoDir;
		VocFileInfo vocFileInfo;

		int n = 0;
		CFile file;
		if (file.Open(strList, CFile::shareDenyWrite))
		{
			while (ReadLineFromTxt(file, strTmp))
			{
				vocFileInfo.strName = GetFileNameByPathNoExt(strTmp);
				RemoveTailBlanks(vocFileInfo.strName);
				vocFileInfo.strExt = GetFileExt(strTmp);
				strFullDir = ParentDir(strTmp);
				vocFileInfo.strDir = GetDirNameByPath(strFullDir);
				strRoot = ParentDir(strFullDir);
				n++;
			}
			file.Close();
		}
		printf("ok sub\n");

		CString strListNoSub;
		strListNoSub = _T("..\\debug\\复件 00listnosub.txt");
		if (file.Open(strListNoSub, CFile::shareDenyWrite))
		{
			while (ReadLineFromTxt(file, strTmp))
			{
				vocFileInfo.strName = GetFileNameNoExt(strTmp);
				RemoveTailBlanks(vocFileInfo.strName);
				vocFileInfo.strExt = GetFileExt(strTmp);
				n++;
			}
			file.Close();
		}
		printf("ok no-sub\n");


 		strTmp = GetFileNameByPath(strFileName);
		strTmp = GetDirNameByPath(strFileName);
		strTmp = GetFileNameByPathNoExt(strFileName);
		strTmp = ParentDir(strFileName);
		strTmp = GetFileExt(strFileName);

		strTmp1 = strTmp;
		strTmp = GetFileNameByPath(strTmp1);
		strTmp = GetDirNameByPath(strTmp1);
		strTmp = GetFileNameByPathNoExt(strTmp1);
		strTmp = ParentDir(strTmp1);
		 

		// write to db

		CoInitialize(NULL);
		_ConnectionPtr m_pConnection;
		_CommandPtr m_pCmd;
		_RecordsetPtr m_pRecord;
		_RecordsetPtr pRet;

		if (FAILED(m_pConnection.CreateInstance(__uuidof(Connection))))
		{
			se("m_pConnection");
		}
		if (FAILED(m_pCmd.CreateInstance(__uuidof(Command))))
		{
			se("m_pCmd");
		}		
		if (FAILED(m_pRecord.CreateInstance(__uuidof(Recordset))))
		{
			se("m_pRecord");
		}

//////////////////////////////////////////////////////////////////////////
// ConnectionString
/*
"Provider=OraOLEDB.Oracle.1;Persist Security Info=True;Data Source=\
(DESCRIPTION = \
	(ADDRESS = (PROTOCOL = TCP)(HOST = dellserver1)(PORT = 1521) )\
	(CONNECT_DATA = (SERVER = DEDICATED)(SERVICE_NAME = test1) )\
)"
*/
//////////////////////////////////////////////////////////////////////////

		CString strConn = _T("Provider=OraOLEDB.Oracle.1;Persist Security Info=True;\
							Data Source=\
								(DESCRIPTION = \
									(ADDRESS = (PROTOCOL = TCP)(HOST = dellserver1)(PORT = 1521) )\
									(CONNECT_DATA = (SERVER = DEDICATED)(SERVICE_NAME = test1) )\
								)");
		CString strUser = _T("scott");
		CString strPwd = _T("aA123456aA");
		CString strInsert; 
		CString strSelect = _T("select * from tblvoicetest");

		_variant_t vRet;

		try
		{
			m_pConnection->Open(strConn.AllocSysString(), strUser.AllocSysString(), strPwd.AllocSysString(), adModeUnknown);

			m_pCmd->ActiveConnection = m_pConnection;

/*
"SN" CHAR(32 BYTE) NOT NULL ENABLE, 
"FILESIZE" NUMBER(10,0) NOT NULL ENABLE, 
"FILEPATH" VARCHAR2(30 BYTE) NOT NULL ENABLE, 
"FILEEXT" VARCHAR2(10 BYTE) NOT NULL ENABLE, 
"LANGUAGE" CHAR(1 BYTE), 
"SEX" CHAR(1 BYTE), 
"NOIZE" CHAR(1 BYTE), 
"OUTVOCFILE" VARCHAR2(200 BYTE), 
"OUTVOCFILESIZE" NUMBER(10,0), 
*/

			int jj  = 0;
			for (int ii = 5048; ii<=5080; ii++)
			{
				strInsert.Format(_T("UPDATE tblvoicetest SET language = chr(%d) WHERE fileid = %d"), jj++, ii);
				m_pCmd->CommandText = strInsert.AllocSysString();
				pRet = m_pCmd->Execute(NULL, NULL, adCmdText);
			}

			printf("ok.");
		}
		catch (_com_error &e)
		{
			wprintf(_T("Error information are as follows\n"));
			wprintf(_T("ErrorNo: 0x%x\nError Message:%s\nError Source:%s\nError Description:%s\n"), 
				e.Error(), 
				e.ErrorMessage(), 
				(LPCTSTR)e.Source(), 
				(LPCTSTR)e.Description());
		}

		
		getchar();

	}

	return nRetCode;
}
