#pragma once
namespace CommonFunction
{
	// wstring (unicode) -> string (utf-8)
	std::string EncodeUtf8MS(const CString& strUnicode);
	std::string EncodeUtf8(const std::wstring& strUnicode);
	// string (utf-8) -> wstring (unicode)
	CString DecodeUtf8MS(const std::string& strUtf8);
	std::wstring DecodeUtf8(const std::string& strUtf8);
	std::string Unicode2Ansi(const wchar_t* pUnicode, const std::string& strLoc = "chs");
	std::wstring Ansi2Unicode(const char* pAnsi, const std::string& strLoc = "chs");
// 	CString Ansi2UnicodeMS(const char* pAnsi);

	// GUI = OPERSYSID_ID (call, task)

// 	CString OperSysGuid(const wchar_t* pOperSysId, const wchar_t* pId);
// 	CString OperSysItemId(const wchar_t* pGuid);
// 	CString OperSysSysId(const wchar_t* pGuid);
};
