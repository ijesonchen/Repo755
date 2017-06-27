#ifndef MongocHelp_h__
#define MongocHelp_h__

#include <string>
#include <sstream>
#include <vector>
#include <codecvt>
#include <bson.h>
#include <mongoc.h>


//////////////////////////////////////////////////////////////////////////
// help function

namespace MongoClib
{
	//////////////////////////////////////////////////////////////////////////
	// inline function declaration
	inline std::string EncodeU8(const std::wstring& strUnicode);
	inline std::wstring DecodeU8(const std::string& strUtf8);
	

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
	inline std::string EncodeU8(const std::wstring& strUnicode)
	{
		std::wstring_convert < std::codecvt_utf8<wchar_t>, wchar_t > convertor;
		return convertor.to_bytes(strUnicode);
	}
	inline std::wstring DecodeU8(const std::string& strUtf8)
	{
		std::wstring_convert < std::codecvt_utf8<wchar_t>, wchar_t > convertor;
		return convertor.from_bytes(strUtf8);
	}

	template<class IterStdString>
	inline bool BsonAddArray(bson_t* doc, const std::string& key, IterStdString first, IterStdString last)
	{
		if (!doc) return false;
		if (first == last) return false;
		unsigned idx = -1;
		AutoBson array;
		while (first != last)
		{
			std::stringstream ss;
			ss << ++idx;
			std::string idxkey(ss.str());
			BSON_APPEND_UTF8(array, idxkey.c_str(), first->c_str);
			++first;
		}
		return BSON_APPEND_ARRAY(doc, key.c_str(), array);
	}

	template<class IterStdWString>
	inline bool BsonAddArrayW(bson_t* doc, const std::string& key, IterStdWString first, IterStdWString last)
	{
		if (!doc) return false;
		if (first == last) return false;
		unsigned idx = -1;
		AutoBson array;
		while (first != last)
		{
			std::stringstream ss;
			ss << ++idx;
			std::string idxkey(ss.str());
			BSON_APPEND_UTF8(array, idxkey.c_str(), EncodeU8(*first)->c_str);
			++first;
		}
		return BSON_APPEND_ARRAY(doc, key.c_str(), array);
	}

	inline bool BsonAddArray(bson_t* doc, const std::string& key, std::vector<std::string>& val)
	{
		if (!doc) return false;
		if (!val.size()) return false;
		unsigned idx = -1;
		bson_t* array = bson_new();
		for (auto it : val)
		{
			std::stringstream ss;
			ss << ++idx;
			std::string idxkey(ss.str());
			BSON_APPEND_UTF8(array, idxkey.c_str(), it.c_str());
		}
		bool bRet = BSON_APPEND_ARRAY(doc, key.c_str(), array);
		bson_destroy(array);
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
		uint32_t len = 0;

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
		uint32_t len = 0;

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
					*result++ = DecodeUtf8(str).c_str();
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
