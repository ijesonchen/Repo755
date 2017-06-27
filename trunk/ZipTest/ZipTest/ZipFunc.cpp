#include "StdAfx.h"
#include "ZipFunc.h"

#include "zip.h"

namespace ZipFunc
{
	int ZipTestMain(int argc, TCHAR* argv[], TCHAR* envp[])
	{
		CString strZipName;
		strZipName = _T("D:\\Data\\tmp\\test.zip");
		HZIP hz = CreateZip(strZipName, NULL);
		if (!hz)
		{
			_tprintf(_T("zip create error: %s\n"), strZipName);
		}
		ZRESULT zRet = NULL;

		CString strFolder;
		strFolder = _T("D:\\Data\\������\\f01����\\01-98765432");
		zRet = ZipAddFolder(hz, strFolder);

		zRet = ZipAdd(hz, _T("TName1"), _T("D:\\Data\\������\\f01����\\02-19760703\\f-����-19760703-16k-01.wav"));
		zRet = ZipAdd(hz, _T("TFolder1\\f-����-19760703-16k-02.wav"), _T("D:\\Data\\������\\f01����\\02-19760703\\f-����-19760703-16k-02.wav"));
		zRet = ZipAdd(hz, _T("T����\\f-����-19760703-16k-02.wav"), _T("D:\\Data\\������\\f01����\\02-19760703\\f-����-19760703-16k-03.wav"));

		zRet =CloseZip(hz);

		return 0;
	}
}