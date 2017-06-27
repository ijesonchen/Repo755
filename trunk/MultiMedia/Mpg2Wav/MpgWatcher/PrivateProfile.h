#pragma once

class CPrivateProfile
{
public:
	CPrivateProfile(const CString strIniFile) : w_strIniFile(strIniFile)
	{
	}
	~CPrivateProfile(){}

	bool GetString(LPCTSTR lpAppName, LPCTSTR lpKeyName, CString& strReturn);
	bool GetUInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, unsigned int& nReturn);
	bool GetFloat(LPCTSTR lpAppName, LPCTSTR lpKeyName, float& fReturn);
	BOOL WriteString(LPCTSTR lpAppName, LPCTSTR lpKeyName, const CString strWrite);
private:
	CString w_strIniFile;
};
