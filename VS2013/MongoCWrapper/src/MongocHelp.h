#ifndef MongocHelp_h__
#define MongocHelp_h__

#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <bson.h>
#include <mongoc.h>



//////////////////////////////////////////////////////////////////////////
// help function

namespace
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
}

namespace MongoClib
{
	// Return data write count.
	// If pBuffer is NULL, return required buffer length.
	// Whether NULL terminator included lies on input.

	// input must NULL terminated.
	int UnicodeToUtf8(const wchar_t* pszUnicode, char* pUtf8, int nUtf8Len);

	// function does not check NULL terminator.
	int UnicodeToUtf8(const wchar_t* pUnicode, int nUnicodeBufLen, char* pUtf8, int nUtf8Len);


	// input must NULL terminated.
	int Utf8ToUnicode(const char* pszUtf8, wchar_t* pUnicode, int nUnicodeBufLen);

	// function does not check NULL terminator.
	int Utf8ToUnicode(const char* pUtf8, int nUtf8Len, wchar_t* pUnicode, int nUnicodeBufLen);

	//////////////////////////////////////////////////////////////////////////
	// inline function declaration
	inline std::string EncUtf8(const std::wstring& strUnicode);
	inline std::wstring DecUtf8(const std::string& strUtf8);

	inline std::string Unicode2Ansi(const wchar_t* pUnicode, const std::string& strLoc = "chs");
	inline std::wstring Ansi2Unicode(const char* pAnsi, const std::string& strLoc = "chs");
	

	template<class IterStdString>
	inline bool BsonAddArray(bson_t* doc, const std::string& key, IterStdString first, IterStdString last);
	template<class IterStdWString>
	inline bool BsonAddArrayW(bson_t* doc, const std::string& key, IterStdWString first, IterStdWString last);
	inline bool BsonAddArray(bson_t* doc, const std::string& key, std::vector<std::string>& val);

	inline bool BsonKeyIter(const bson_t* doc, const std::string& key, bson_iter_t*& ival);
	inline bool BsonDoc(const bson_t* doc, const std::string& key, bson_t*& val);
	inline bool BsonOid(const bson_t* doc, const std::string& key, std::string& val);
	inline bool BsonJson(const bson_t* doc, const std::string& key, std::string& val);

	inline bool BsonValue(const bson_t* doc, const std::string& key, bool& val);
	inline bool BsonValue(const bson_t* doc, const std::string& key, int& val);
	inline bool BsonValue(const bson_t* doc, const std::string& key, long long& val);
	inline bool BsonValue(const bson_t* doc, const std::string& key, double& val);
	inline bool BsonValue(const bson_t* doc, const std::string& key, std::string& val);
	inline bool BsonValue(const bson_t* doc, const std::string& key, std::vector<std::string>& val);
	inline bool BsonValueBin(const bson_t* doc, const std::string& key, std::string& val);

	// if failed, return default value
	inline bool
		BsonValueBool(const bson_t* doc, const std::string& key);
	inline int
		BsonValueInt32(const bson_t* doc, const std::string& key);
	inline long long
		BsonValueInt64(const bson_t* doc, const std::string& key);
	inline double
		BsonValueDouble(const bson_t* doc, const std::string& key);
	inline std::string
		BsonValStr(const bson_t* doc, const std::string& key);
	inline std::string
		BsonValBin(const bson_t* doc, const std::string& key);

	
	//////////////////////////////////////////////////////////////////////////
	// inline functions' implementation

	// string (UTF-8) -> wstring (unicode)
	inline std::string EncUtf8(const std::wstring& strUnicode)
	{
		const wchar_t* pw = strUnicode.c_str();
		int nUnicode = strUnicode.length();
		int nUtf8 = UnicodeToUtf8(pw, nUnicode, NULL, 0);
		if (nUtf8 <= 0)
		{
			return "";
		}
		char* p = new char[nUtf8];
		std::auto_ptr<char> ap(p);
		memset(p, 0, nUtf8 * sizeof(char));
		int nRet = UnicodeToUtf8(pw, nUnicode, p, nUtf8);
		if (nRet <= 0)
		{
			return "";
		}
		std::string utf8;
		utf8.assign(p, nUtf8);
		return utf8;
	}
	inline std::wstring DecUtf8(const std::string& strUtf8)
	{
		const char* p = strUtf8.c_str();
		int nUtf8 = strUtf8.length();
		int nUnicode = Utf8ToUnicode(p, nUtf8, NULL, 0);
		if (nUnicode <= 0)
		{
			return L"";
		}
		wchar_t* pw = new wchar_t[nUnicode];
		std::auto_ptr<wchar_t> ap(pw);
		memset(pw, 0, nUnicode * sizeof(wchar_t));
		int nRet = Utf8ToUnicode(p, nUtf8, pw, nUnicode);
		if (nRet <= 0)
		{
			return L"";
		}
		std::wstring unicode;
		unicode.assign(pw, nUnicode);
		return unicode;
	}
	
#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4996)
#endif // _MSC_VER

	inline std::string Unicode2Ansi(const wchar_t* pUnicode, const std::string& strLoc /*= "chs"*/)
	{
		AutoLocale loc(strLoc);
		size_t len = wcstombs(nullptr, pUnicode, 0) + 1;
		if (len == (size_t)-1)
		{
			return "";
		}
		char* ap = new char[len];
		std::unique_ptr<char> aup(ap);
		ap[0] = 0;
		ap[len - 1] = 0;

		len = wcstombs(ap, pUnicode, len);

		if (len == (size_t)-1)
		{
			return "";
		}
		return ap;
	}

	inline std::wstring Ansi2Unicode(const char* pAnsi, const std::string& strLoc /*= "chs"*/)
	{
		AutoLocale loc(strLoc);
		size_t len = mbstowcs(nullptr, pAnsi, 0) + 1;
		if (len == (size_t)-1)
		{
			return L"";
		}
		wchar_t* wp = new wchar_t[len];
		std::unique_ptr<wchar_t> wup(wp);
		wp[0] = 0;
		wp[len - 1] = 0;

		len = mbstowcs(wp, pAnsi, len);
		if (len == (size_t)-1)
		{
			return L"";
		}
		return wp;
	}

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

	template<class IterStdString>
	inline bool BsonAddArray(bson_t* doc, const std::string& key, IterStdString first, IterStdString last)
	{
		if (!doc) return false;
		if (first == last) return false;
		unsigned idx = -1;
		bson_t* arr = bson_new();
		while (first != last)
		{
			std::stringstream ss;
			ss << ++idx;
			std::string idxkey(ss.str());
			BSON_APPEND_UTF8(arr, idxkey.c_str(), first->c_str);
			++first;
		}
		bool bRet = BSON_APPEND_ARRAY(doc, key.c_str(), arr);
		bson_destroy(arr);
		return bRet;
	}

	template<class IterStdWString>
	inline bool BsonAddArrayW(bson_t* doc, const std::string& key, IterStdWString first, IterStdWString last)
	{
		if (!doc) return false;
		if (first == last) return false;
		unsigned idx = -1;
		bson_t* arr = bson_new();
		while (first != last)
		{
			std::stringstream ss;
			ss << ++idx;
			std::string idxkey(ss.str());
			BSON_APPEND_UTF8(arr, idxkey.c_str(), EncUtf8(*first)->c_str);
			++first;
		}
		bool bRet = BSON_APPEND_ARRAY(doc, key.c_str(), arr);
		bson_destroy(arr);
		return bRet;
	}

	inline bool BsonAddArray(bson_t* doc, const std::string& key, std::vector<std::string>& val)
	{
		if (!doc) return false;
		if (!val.size()) return false;
		unsigned idx = (unsigned)-1;
		bson_t* arr = bson_new();
		for (auto it : val)
		{
			std::stringstream ss;
			ss << ++idx;
			std::string idxkey(ss.str());
			BSON_APPEND_UTF8(arr, idxkey.c_str(), it.c_str());
		}
		bool bRet = BSON_APPEND_ARRAY(doc, key.c_str(), arr);
		bson_destroy(arr);
		return bRet;
	}

	inline bool BsonKeyIter(const bson_t* doc, const std::string& key, bson_iter_t& ival)
	{
		if (!doc) return false;

		bson_iter_t iter;
		if (bson_iter_init(&iter, doc))
		{
			if (bson_iter_find_descendant(&iter, key.c_str(), &ival))
			{
				return true;
			}
		}
		return false;
	}

	inline bool BsonDoc(const bson_t* doc, const std::string& key, bson_t*& val)
	{
		if (!doc) return false;

		if (val)
		{
			bson_destroy(val);
		}

		bson_iter_t ival;

		if (BsonKeyIter(doc, key, ival) && BSON_ITER_HOLDS_DOCUMENT(&ival))
		{
			uint8_t* pdoc = nullptr;
			uint32_t ndoc = 0;
			bson_iter_document(&ival, &ndoc, (const uint8_t**)&pdoc);
			val = bson_new_from_data(pdoc, ndoc);
			return true;
		}
		return false;
	}

	inline bool BsonJson(const bson_t* doc, const std::string& key, std::string& val)
	{
		if (!doc) return false;

		bson_iter_t ival;

		uint8_t* pdoc = nullptr;
		if (BsonKeyIter(doc, key, ival) && BSON_ITER_HOLDS_DOCUMENT(&ival))
		{
			uint32_t ndoc = 0;
			bson_iter_document(&ival, &ndoc, (const uint8_t**)&pdoc);
			bson_t* subdoc = bson_new_from_data(pdoc, ndoc);
			if (subdoc)
			{
				char* str = bson_as_json(subdoc, nullptr);
				val = str;
				bson_free(str);
				bson_destroy(subdoc);
				return true;
			}
		}
		return false;
	}

	inline bool BsonOid(const bson_t* doc, const std::string& key, std::string& val)
	{
		if (!doc) return false;

		bson_iter_t ival;

		if (BsonKeyIter(doc, key, ival) && BSON_ITER_HOLDS_OID(&ival))
		{
			char str[25];
			bson_oid_to_string(bson_iter_oid(&ival), str);
			val = str;
			return true;
		}
		return false;
	}

	inline bool BsonValue(const bson_t* doc, const std::string& key, bool& val)
	{
		if (!doc) return false;

		bson_iter_t ival;

		if (BsonKeyIter(doc, key, ival) && BSON_ITER_HOLDS_BOOL(&ival))
		{
			val = bson_iter_bool(&ival);
			return true;
		}
		return false;
	}

	inline bool BsonValue(const bson_t* doc, const std::string& key, int& val)
	{
		if (!doc) return false;

		bson_iter_t ival;

		if (BsonKeyIter(doc, key, ival) && BSON_ITER_HOLDS_INT32(&ival))
		{
			val = bson_iter_int32(&ival);
			return true;
		}
		return false;
	}

	inline bool BsonValue(const bson_t* doc, const std::string& key, long long& val)
	{
		if (!doc) return false;

		bson_iter_t ival;

		if (BsonKeyIter(doc, key, ival) && BSON_ITER_HOLDS_INT64(&ival))
		{
			val = bson_iter_int64(&ival);
			return true;
		}
		return false;
	}

	inline bool BsonValue(const bson_t* doc, const std::string& key, double& val)
	{
		if (!doc) return false;

		bson_iter_t ival;

		if (BsonKeyIter(doc, key, ival) && BSON_ITER_HOLDS_DOUBLE(&ival))
		{
			val = bson_iter_double(&ival);
			return true;
		}
		return false;
	}

	inline bool BsonValue(const bson_t* doc, const std::string& key, std::string& val)
	{
		if (!doc) return false;

		bson_iter_t ival;
		uint32_t len = 0;

		if (BsonKeyIter(doc, key, ival) && BSON_ITER_HOLDS_UTF8(&ival))
		{
			val = bson_iter_utf8(&ival, &len);
			return true;
		}
		return false;
	}

	inline bool BsonValueBin(const bson_t* doc, const std::string& key, std::string& val)
	{
		if (!doc) return false;

		bson_iter_t ival;

		if (BsonKeyIter(doc, key, ival) && BSON_ITER_HOLDS_BINARY(&ival))
		{
			bson_subtype_t  subtype;
			uint32_t        len = 0;
			const uint8_t*	buf = nullptr;
			bson_iter_binary(&ival, &subtype, &len, &buf);
			if (buf && len)
			{
				val.assign((const char*)buf, len);
				return true;
			}
			return false;
		}
		return false;
	}

	template<class IterStdStringInsert>
	inline bool BsonValue(const bson_t* doc, const std::string& key, IterStdStringInsert result)
	{
		if (!doc) return false;

		bson_iter_t ival;
		uint32_t len = 0;
		unsigned cnt = 0;

		if (BsonKeyIter(doc, key, ival) && BSON_ITER_HOLDS_ARRAY(&ival))
		{
			bson_iter_t ichild;
			if (bson_iter_recurse(&ival, &ichild))
			{
				while (bson_iter_next(&ichild) && BSON_ITER_HOLDS_UTF8(&ichild))
				{
					*result++ = bson_iter_utf8(&ichild, &len);
					++cnt;
				}
				if (cnt)
				{
					return true;
				}
			}
		}
		return false;
	}

	template<class IterStdWStringInsert>
	inline bool BsonValueW(const bson_t* doc, const std::string& key, IterStdWStringInsert result)
	{
		if (!doc) return false;

		bson_iter_t ival;
		uint32_t len = 0;
		unsigned cnt = 0;

		if (BsonKeyIter(doc, key, ival) && BSON_ITER_HOLDS_ARRAY(&ival))
		{
			bson_iter_t ichild;
			if (bson_iter_recurse(&ival, &ichild))
			{
				while (bson_iter_next(&ichild) && BSON_ITER_HOLDS_UTF8(&ichild))
				{
					std::string str = bson_iter_utf8(&ichild, &len);
					*result++ = DecUtf8(str).c_str();
					++cnt;
				}
				if (cnt)
				{
					return true;
				}
			}
		}
		return false;
	}

	inline bool BsonValue(const bson_t* doc, const std::string& key, std::vector<std::string>& val)
	{
		if (!doc) return false;

		bson_iter_t ival;
		uint32_t len = 0;

		if (BsonKeyIter(doc, key, ival) && BSON_ITER_HOLDS_ARRAY(&ival))
		{
			bson_iter_t ichild;
			if (bson_iter_recurse(&ival, &ichild))
			{
				while (bson_iter_next(&ichild) && BSON_ITER_HOLDS_UTF8(&ichild))
				{
					val.push_back(bson_iter_utf8(&ichild, &len));
				}
				if (val.size())
				{
					return true;
				}
			}
		}
		return false;
	}

	inline bool BsonValueBool(const bson_t* doc, const std::string& key)
	{
		bool val = 0;
		BsonValue(doc, key, val);
		return val;
	}

	inline int BsonValueInt32(const bson_t* doc, const std::string& key)
	{
		int val = 0;
		BsonValue(doc, key, val);
		return val;
	}

	inline long long BsonValueInt64(const bson_t* doc, const std::string& key)
	{
		long long val = 0;
		BsonValue(doc, key, val);
		return val;
	}

	inline double BsonValueDouble(const bson_t* doc, const std::string& key)
	{
		double val = 0;
		BsonValue(doc, key, val);
		return val;
	}

	inline std::string BsonValStr(const bson_t* doc, const std::string& key)
	{
		std::string val;
		BsonValue(doc, key, val);
		return val;
	}

	inline std::string BsonValBin(const bson_t* doc, const std::string& key)
	{
		std::string val;
		BsonValueBin(doc, key, val);
		return val;
	}

} // MongoClib

#endif // MongocHelp_h__
