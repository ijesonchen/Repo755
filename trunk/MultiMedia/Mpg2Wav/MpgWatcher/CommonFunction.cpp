#include "stdafx.h"
#include <Shlwapi.h>
#include "CommonFunction.h"

 
namespace CommonFunction
{
	bool WriteEmptyUnicodeText(const CString& strPath)
	{
		try
		{
			CFile file;
			if (file.Open(strPath,CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite))
			{
				USHORT usHead = 0xfeff;
				file.Write(&usHead, UINT(sizeof(usHead)));
			}
			else
			{
				return false;
			}
		}
		catch(CFileException* pe)
		{
			pe->Delete();
			return false;
		}
		return true;
	}
	bool WriteLineToTxt(const CString& strPath, const CString& strContent, bool bUnicode)
	{
		bool bExist = false;
		if (IsFileExist(strPath))
		{
			bExist = true;
		}
		try
		{
			CFile file;
			if (file.Open(strPath,CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate | CFile::shareDenyWrite))
			{
				file.SeekToEnd();
				if (!bUnicode)
				{
					CStringA strTemp(strContent);
					strTemp += "\r\n";
					file.Write(strTemp.GetBuffer(),strTemp.GetLength());
				}
				else
				{
					if (!bExist)
					{
						USHORT usHead = 0xfeff;
						file.Write(&usHead, UINT(sizeof(usHead)));
					}
					CStringW strTemp(strContent);
					strTemp += L"\r\n";
					file.Write(strTemp.GetBuffer(),strTemp.GetLength() * sizeof(WCHAR));
				}
			}
			else
			{
				return false;
			}
		}
		catch(CFileException* pe)
		{
			pe->Delete();
			return false;
		}
		return true;
	}

	bool WriteNumToTxt(const CString& strPath, const int nNum)
	{
		CString strNum;
		strNum.Format(_T("%d"), nNum);
		return WriteLineToTxt(strPath,strNum);
	}

	bool IsDirExist(LPCTSTR lpszDirPath)
	{
		return ::PathFileExists(lpszDirPath) && 
			(GetFileAttributes(lpszDirPath) & FILE_ATTRIBUTE_DIRECTORY);
	}

	bool IsFileExist(LPCTSTR lpszFilePath)
	{
		DWORD dwAttr = GetFileAttributes(lpszFilePath);
		return dwAttr != INVALID_FILE_ATTRIBUTES
			&& !(dwAttr & FILE_ATTRIBUTE_DIRECTORY);
	}

#pragma warning(disable: 4996)

	CString GetModuleFolder()
	{
		CString strPath = _T("");
		TCHAR lpszFilePath [_MAX_PATH];
		if (::GetModuleFileName (NULL, lpszFilePath, _MAX_PATH) > 0)
		{
			TCHAR path_buffer[_MAX_PATH];
			TCHAR drive[_MAX_DRIVE];
			TCHAR dir[_MAX_DIR];

			_tsplitpath (lpszFilePath, drive, dir, NULL, NULL);		
			_tmakepath_s (path_buffer, drive, dir, NULL, NULL);
			strPath = path_buffer;
		}

		return strPath;
	}

	CString GetFileNameByPath(const CString& strPath)
	{
		CString strFileName;

		int nPos = strPath.ReverseFind('\\');
		if (-1 != nPos)
		{
			strFileName = strPath.Mid(nPos + 1, strPath.GetLength() - nPos);
		}

		return strFileName;
	}

	CString GetDirNameByPath(const CString& strDirPath)//由目录全路径得到当前的目录名称
	{
		return CommonFunction::GetFileNameByPath(strDirPath);
	}

	CString GetFileNameByPathNoExt(const CString& strPath)
	{
		CString strFileName;
		int nPosStart = strPath.ReverseFind('\\');
		int nPosEnd = strPath.ReverseFind('.');
		if (-1 != nPosStart)
		{
			if (nPosEnd == -1)
			{
				strFileName = strPath.Mid(nPosStart + 1, strPath.GetLength() - nPosStart);
			}
			else if (nPosEnd > nPosStart + 1)
			{
				strFileName = strPath.Mid(nPosStart + 1, nPosEnd - nPosStart - 1);
			}
		}
		return strFileName;
	}

	bool GetDirTotalMB(const CString& strDir, ULARGE_INTEGER& ulByte)
	{
		ulByte.QuadPart = 0;
		ULARGE_INTEGER ulTotalSize;
		ulTotalSize.QuadPart = 0;
		CFileFind finder;
		CString strWildcard = strDir;
		if (strWildcard.Right(1) == _T("\\"))
		{
			strWildcard += _T("*.*");
		}
		else
		{
			strWildcard += _T("\\*.*");
		}

		BOOL bWorking = finder.FindFile(strWildcard);
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			if (finder.IsDots())
				continue;
			if (finder.IsDirectory())
			{
				ULARGE_INTEGER ulSubByte;
				ulSubByte.QuadPart = 0;
				if (!GetDirTotalMB (finder.GetFilePath(), ulSubByte))
				{
					continue;
				}
				ulByte.QuadPart += ulSubByte.QuadPart;
				continue;
			}

			CString strCurFile = finder.GetFilePath();

			WIN32_FILE_ATTRIBUTE_DATA attrData;
			ULARGE_INTEGER ulIntFileSize;
			if (::GetFileAttributesEx(strCurFile, GetFileExInfoStandard, &attrData) == FALSE)
			{
				return false;
			}
			ulIntFileSize.HighPart = attrData.nFileSizeHigh;
			ulIntFileSize.LowPart = attrData.nFileSizeLow;
			ulTotalSize.QuadPart += ulIntFileSize.QuadPart;
		}
		finder.Close();
		ulByte.QuadPart += ulTotalSize.QuadPart ;
		return true;
	}

	bool ReadLineFromTxt(CFile& file, CString& strLine, bool bUnicode)
	{
		strLine.Empty();
		int  ret;
		if (bUnicode)
		{
			CStringW strTemp;
			wchar_t ch;
			try
			{
				while (true)
				{
					ret = file.Read(&ch, sizeof(ch));
					if (ret == sizeof(ch))
					{
						if (ch == L'\n')
						{
							break;
						}
						strTemp += ch;
					}
					else
					{
						return false;
					}
				}
			}
			catch (CFileException* pe)
			{	
				TCHAR   szCause[255];
				CString strError;
				pe->GetErrorMessage(szCause, 255);
				strError = _T("ReadString error : ");
				strError += szCause;
				TRACE(_T("ReadString error   %s\n"), strError);
				pe->Delete();
				return false;
			}
			strTemp.Trim();
			strLine = strTemp;
		}
		else
		{
			CStringA strTempAnsi;
			char ch;
			try
			{
				while (true)
				{
					ret = file.Read(&ch, sizeof(ch));
					if (ret == sizeof(ch))
					{
						if (ch == '\n')
						{
							break;
						}
						strTempAnsi += ch;
					}
					else
					{
						return false;
					}
				}
			}
			catch (CFileException* pe)
			{	
				TCHAR   szCause[255];
				CString strError;
				pe->GetErrorMessage(szCause, 255);
				strError = _T("ReadString error : ");
				strError += szCause;
				TRACE(_T("ReadString error   %s\n"), strError);
				pe->Delete();
				return false;
			}
			strTempAnsi.Trim();
			strLine = strTempAnsi;
		}
		return true;
	}
	bool ReadNumFromTxt(CFile& file, int& nNum)
	{
		CString strLine;
		ReadLineFromTxt(file, strLine);
		if(_stscanf(strLine,_T("%d"),&nNum) != 1)
		{
			return false;
		}
		return true;
	}
	bool ReadNumFromTxt(CFile& file, float& fNum)
	{
		CString strLine;
		ReadLineFromTxt(file, strLine);
		if(_stscanf(strLine,_T("%f"),&fNum) != 1)
		{
			return false;
		}
		return true;
	}

#pragma warning(default: 4996)

	bool DivisionString(const CString& strLine, CString& strLeft, CString& strRight, TCHAR tcSeparator)
	{
		int nPos = strLine.Find(tcSeparator);
		if (nPos == -1)
		{
			return false;
		}
		if (nPos <= 0 || nPos >= (strLine.GetLength() - 1))
		{
			return  false;
		}
		strLeft = strLine.Mid(0, nPos);
		strRight = strLine.Mid(nPos + 1, strLine.GetLength() - nPos);
		strLeft.Trim();
		strRight.Trim();
		return true;
	}
	bool DivisionString(const CString& strLine, std::vector<CString>& vec, TCHAR tcSeparator)
	{
		vec.clear();
		int nLen = strLine.GetLength();
		int nStartPos = 0;
		CString strTemp;
		for (int nPos = 0 ;nPos < nLen; nPos++)
		{
			if (strLine[nPos] == tcSeparator && nPos > nStartPos)
			{
				strTemp = strLine.Mid(nStartPos, nPos - nStartPos);
				nStartPos = nPos + 1;
				vec.push_back(strTemp);
			}
		}
		if (nLen > nStartPos)
		{
			strTemp = strLine.Mid(nStartPos, nLen - nStartPos);
			vec.push_back(strTemp);
		}
		return true;
	}

	CString WINAPI ParentDir(const CString& strFilePath)
	{
		int nIdx;
		nIdx = strFilePath.ReverseFind('\\');

		if (nIdx > -1)
		{
			return strFilePath.Left(nIdx);
		}
		else
		{
			return _T("");
		}
	}

	bool EnsureDir(const CString& strDir)//确保目录存在，如果原先不存在则创建，如果创建失败，返回false
	{
		DWORD dwRet = ::GetFileAttributes(strDir);
		if ( dwRet == INVALID_FILE_ATTRIBUTES || (dwRet & FILE_ATTRIBUTE_DIRECTORY) == 0) 
		{
			if (ERROR_SUCCESS != ::SHCreateDirectoryEx(NULL, strDir, NULL))
			{
				return false;
			}
		}
		return true;
	}

	static bool IsExtMatch(const CString strFile, CString strExt)
	{
		int nFind = strFile.ReverseFind(_T('.'));
		int nFilePathLen = strFile.GetLength();
		if (nFind < 0 || nFind == nFilePathLen - 1)//没找到或者是最后一个字符
		{
			return false;
		}
		CString strFileExt = strFile.Right(nFilePathLen - nFind- 1);
		return (0 == strFileExt.CompareNoCase(strExt));
	}

	void RecurFindFiles(const CString& strSrcDir, std::deque<CString>& dqFiles, CString strExt)
	{
		CFileFind finder;
		CString strWildcard(strSrcDir);
		strWildcard += _T("\\*.*");

		BOOL bWorking = finder.FindFile(strWildcard);
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			if (finder.IsDots())
			{
				continue;
			}
			CString strPath = finder.GetFilePath();
			if (finder.IsDirectory())
			{
				RecurFindFiles(strPath, dqFiles, strExt);
				continue;
			}
			if (IsExtMatch(strPath, strExt))
			{
				dqFiles.push_back(strPath);
			}
		}
		finder.Close();
	}

	void FindFilesInDir(const CString& strSrcDir, std::deque<CString>& dqFiles, CString strExt)
	{
		CFileFind finder;
		CString strWildcard(strSrcDir);
		strWildcard += _T("\\*.*");

		BOOL bWorking = finder.FindFile(strWildcard);
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			if (finder.IsDots())
			{
				continue;
			}

			if (finder.IsDirectory())
			{
				continue;
			}

			CString strPath = finder.GetFilePath();
			if (IsExtMatch(strPath, strExt))
			{
				dqFiles.push_back(strPath);
			}
		}
		finder.Close();
	}

	void FindSubDir(const CString& strSrcDir, std::deque<CString>& dqDirs)
	{
		CFileFind finder;
		CString strWildcard(strSrcDir);
		strWildcard += _T("\\*.*");

		BOOL bWorking = finder.FindFile(strWildcard);
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			if (finder.IsDots())
			{
				continue;
			}

			if (finder.IsDirectory())
			{
				CString strPath = finder.GetFilePath();
				dqDirs.push_back(strPath);
			}			
		}
		finder.Close();
	}

	CString GetExceptionMessage(const CException *pe)
	{
		TCHAR   szCause[255];
		pe->GetErrorMessage(szCause, 255);
		CString strFormatted(szCause);
		return strFormatted;
	}
	BOOL SplitString(const CString& strLine, const TCHAR tchSeperator, 
		std::vector<CString>& vtStrSplits)
	{
		vtStrSplits.clear();
		int nNum = -1;	
		int nStrLen = strLine.GetLength();

		while (nNum < nStrLen - 1)
		{
			int nStartPos = nNum + 1;
			nNum = strLine.Find(tchSeperator, nStartPos);
			if (nNum == nStartPos)
			{
				vtStrSplits.push_back(_T(""));
				continue;
			}
			if (nNum == -1)
			{
				nNum = nStrLen;
			}
			CString strTmp = strLine.Mid(nStartPos, nNum - nStartPos);
			vtStrSplits.push_back(strTmp);
		}

		return vtStrSplits.size() > 1;
	}

	bool DeleteFolder(const CString& strDir)//删除文件夹内容
	{
		CFileFind ff;
		CString strPath = strDir;
		if (strPath.Right(1) != "\\")
		{
			strPath += "\\";
		}
		strPath += _T("*.*");
		BOOL bRes = ff.FindFile(strPath);
		while(bRes)
		{
			bRes = ff.FindNextFile();
			if (ff.IsDots())
			{
				continue;
			}
			else if (ff.IsDirectory())
			{
				strPath = ff.GetFilePath();
				if (!DeleteFolder(strPath))
				{
					return false;
				}
			}
			else
			{
				if (FALSE == DeleteFile(ff.GetFilePath()))
				{
					return false;
				}				
			}
		}

		ff.Close();

		if(!RemoveDirectory(strDir))
		{
			return false;
		}
		return true;
	}

	void CStringToCharArray(const CString& strSrc, char* szDst, int nDstBufferBytes)
	{
		CStringA straSrc = (CStringA)strSrc;
		memset(szDst, 0, nDstBufferBytes);
		memcpy(szDst, straSrc, straSrc.GetLength() * sizeof(char));
	}
};
