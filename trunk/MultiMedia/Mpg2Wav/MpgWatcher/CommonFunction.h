#pragma once

namespace CommonFunction
{
	CString GetFileNameByPath(const CString& strPath);//由文件全路径得到文件名称
	CString GetDirNameByPath(const CString& strDirPath);//由目录全路径得到当前的目录名称
	CString GetFileNameByPathNoExt(const CString& strPath);
	CString WINAPI ParentDir(const CString& strFilePath);
	bool WriteLineToTxt(const CString& strPath, const CString& strContent, bool bUnicode = false);
	bool WriteEmptyUnicodeText(const CString& strPath);
	bool WriteNumToTxt(const CString& strPath, const int nNum);
	bool IsFileExist(LPCTSTR lpszFilePath);
	bool IsDirExist(LPCTSTR lpszDirPath);
	CString GetModuleFolder();
	bool GetDirTotalMB(const CString& strDir, ULARGE_INTEGER& nMb);//得到目录总大小MB
	bool ReadLineFromTxt(CFile& file, CString& strLine,  bool bUnicode = false);
	bool ReadNumFromTxt(CFile& file, int& nNum);
	bool ReadNumFromTxt(CFile& file, float& fNum);
	bool DivisionString(const CString& strLine, CString& strLeft, CString& strRight, TCHAR tcSeparator = _T('='));
	bool DivisionString(const CString& strLine, std::vector<CString>& vec, TCHAR tcSeparator = _T('\''));//将字符串按照指定分隔符分割
	bool EnsureDir(const CString& strDir);//确保目录存在，如果原先不存在则创建，如果创建失败，返回false
	void RecurFindFiles(const CString& strSrcDir, std::deque<CString>& dqFiles, CString strExt);
	void FindFilesInDir(const CString& strSrcDir, std::deque<CString>& dqFiles, CString strExt);
	void FindSubDir(const CString& strSrcDir, std::deque<CString>& dqDirs);//查找源目录中的下一层子目录
	CString GetExceptionMessage(const CException *pe);
	BOOL SplitString(const CString& strLine, const TCHAR tchSeperator, 
		std::vector<CString>& vtStrSplits);
	bool DeleteFolder(const CString& strDir);//删除文件夹内容
	void CStringToCharArray(const CString& strSrc, char* szDst, int nDstBufferBytes);
};

