#include "stdafx.h"
#include "CommonFunction.h"
#include <string>
#include <cstdlib>
#include <memory>

#include <codecvt>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

namespace CommonFunction
{
	class AutoLocale
	{
	public:
		AutoLocale(const std::string& loc) {
			oldLocale = setlocale(LC_CTYPE, nullptr);
			setlocale(LC_CTYPE, loc.c_str());
		};
		~AutoLocale() { setlocale(LC_CTYPE, oldLocale.c_str()); };

	private:
		std::string oldLocale;
	};

	// wstring (unicode) -> string (utf-8)
	std::string EncodeUtf8MS(const CString& strUnicode)
	{
		return EncodeUtf8(strUnicode.GetString());
	}
	std::string EncodeUtf8(const std::wstring& strUnicode)
	{
		wstring_convert < codecvt_utf8<wchar_t>, wchar_t > convertor;
		return convertor.to_bytes(strUnicode);
	}
	// string (utf-8) -> wstring (unicode)
	CString DecodeUtf8MS(const std::string& strUtf8)
	{
		return DecodeUtf8(strUtf8).c_str();
	}
	std::wstring DecodeUtf8(const std::string& strUtf8)
	{
		wstring_convert < codecvt_utf8<wchar_t>, wchar_t > convertor;
		return convertor.from_bytes(strUtf8);
	}

	std::string Unicode2Ansi(const wchar_t* pUnicode, const std::string& strLoc /*= "chs"*/)
	{
		AutoLocale loc(strLoc);
		size_t len = wcstombs(nullptr, pUnicode, 0) + 1;
		if (len == -1)
		{
			return "";
		}
		char* ap = new char[len];
		unique_ptr<char> aup(ap);
 		ap[0] = 0;
 		ap[len - 1] = 0;

		len = wcstombs(ap, pUnicode, len );
		if (len == -1)
		{
			return "";
		}
		return ap;
	}

	std::wstring Ansi2Unicode(const char* pAnsi, const std::string& strLoc /*= "chs"*/)
	{
		AutoLocale loc(strLoc);
		size_t len = mbstowcs(nullptr, pAnsi, 0) + 1;
		if (len == -1)
		{
			return L"";
		}
		wchar_t* wp = new wchar_t[len];
		unique_ptr<wchar_t> wup(wp);
		wp[0] = 0;
		wp[len - 1] = 0;

		len = mbstowcs(wp, pAnsi, len);
		if (len == -1)
		{
			return L"";
		}
		return wp;

		CString str;
		str = pAnsi;
		return str.GetString();
	}

	CString Ansi2UnicodeMS(const char* pAnsi)
	{
		return Ansi2Unicode(pAnsi).c_str();
	}

	CString OperSysGuid(const wchar_t* pOperSysId, const wchar_t* pId)
	{
		CString guid;
		guid.Format(_T("%s_%s"), pOperSysId, pId);
		return guid;
	}

	CString OperSysItemId(const wchar_t* pGuid)
	{
		CString guid = pGuid;
		int pos = guid.Find(L'_');
		if (pos != -1)
		{
			return guid.Right(guid.GetLength() - pos - 1);
		}
		return guid;
	}
	CString OperSysSysId(const wchar_t* pGuid)
	{
		CString guid = pGuid;
		int pos = guid.Find(L'_');
		if (pos != -1)
		{
			return guid.Left(pos);
		}
		return guid;
	}
}