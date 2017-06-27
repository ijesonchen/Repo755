
// stdafx.cpp : source file that includes just the standard includes
// OracleDlg.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"



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

DWORD GetFileSize(CString strFile)
{
	WIN32_FILE_ATTRIBUTE_DATA fileAttributeData;
	memset(&fileAttributeData, 0, sizeof(WIN32_FILE_ATTRIBUTE_DATA));
	::GetFileAttributesEx(strFile, GetFileExInfoStandard, &fileAttributeData);
	return fileAttributeData.nFileSizeLow;
}