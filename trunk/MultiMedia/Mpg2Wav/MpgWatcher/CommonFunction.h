#pragma once

namespace CommonFunction
{
	CString GetFileNameByPath(const CString& strPath);//���ļ�ȫ·���õ��ļ�����
	CString GetDirNameByPath(const CString& strDirPath);//��Ŀ¼ȫ·���õ���ǰ��Ŀ¼����
	CString GetFileNameByPathNoExt(const CString& strPath);
	CString WINAPI ParentDir(const CString& strFilePath);
	bool WriteLineToTxt(const CString& strPath, const CString& strContent, bool bUnicode = false);
	bool WriteEmptyUnicodeText(const CString& strPath);
	bool WriteNumToTxt(const CString& strPath, const int nNum);
	bool IsFileExist(LPCTSTR lpszFilePath);
	bool IsDirExist(LPCTSTR lpszDirPath);
	CString GetModuleFolder();
	bool GetDirTotalMB(const CString& strDir, ULARGE_INTEGER& nMb);//�õ�Ŀ¼�ܴ�СMB
	bool ReadLineFromTxt(CFile& file, CString& strLine,  bool bUnicode = false);
	bool ReadNumFromTxt(CFile& file, int& nNum);
	bool ReadNumFromTxt(CFile& file, float& fNum);
	bool DivisionString(const CString& strLine, CString& strLeft, CString& strRight, TCHAR tcSeparator = _T('='));
	bool DivisionString(const CString& strLine, std::vector<CString>& vec, TCHAR tcSeparator = _T('\''));//���ַ�������ָ���ָ����ָ�
	bool EnsureDir(const CString& strDir);//ȷ��Ŀ¼���ڣ����ԭ�Ȳ������򴴽����������ʧ�ܣ�����false
	void RecurFindFiles(const CString& strSrcDir, std::deque<CString>& dqFiles, CString strExt);
	void FindFilesInDir(const CString& strSrcDir, std::deque<CString>& dqFiles, CString strExt);
	void FindSubDir(const CString& strSrcDir, std::deque<CString>& dqDirs);//����ԴĿ¼�е���һ����Ŀ¼
	CString GetExceptionMessage(const CException *pe);
	BOOL SplitString(const CString& strLine, const TCHAR tchSeperator, 
		std::vector<CString>& vtStrSplits);
	bool DeleteFolder(const CString& strDir);//ɾ���ļ�������
	void CStringToCharArray(const CString& strSrc, char* szDst, int nDstBufferBytes);
};

