#ifndef MongoAuto_h__
#define MongoAuto_h__

#include <string>
#include <sstream>
#include <vector>
#include <bson.h>
#include <mongoc.h>
#include "MongocHelp.h"


//////////////////////////////////////////////////////////////////////////
// help class

namespace MongoClib
{
	// wrapper for mongoc_uri_t*
	class AutoUri
	{
		AutoUri(AutoUri& rhs) = delete;
		AutoUri(AutoUri&& rhs) = delete;
		AutoUri& operator = (AutoUri& rhs) = delete;
		AutoUri& operator = (AutoUri&& rhs) = delete;
	public:
		AutoUri(mongoc_uri_t* p) : m_p(p) { };
		AutoUri(const std::string uri) { m_p = mongoc_uri_new(uri.c_str()); };

		~AutoUri() { if (m_p) mongoc_uri_destroy(m_p); };
		
		operator mongoc_uri_t* () { return m_p; }

	private:
		mongoc_uri_t* m_p = nullptr;
	};

	// wrapper for mongoc_client_t*
	class AutoClient
	{
		AutoClient(AutoClient& rhs) = delete;
		AutoClient(AutoClient&& rhs) = delete;
		AutoClient& operator = (AutoClient& rhs) = delete;
		AutoClient& operator = (AutoClient&& rhs) = delete;
	public:
		AutoClient(mongoc_client_t* p) : m_p(p) { };
		explicit AutoClient(const std::string& srv) 
			{ m_p = mongoc_client_new(srv.c_str()); };

		~AutoClient() { if (m_p) mongoc_client_destroy(m_p); };
		
		operator mongoc_client_t* (){ return m_p; }

	private:
		mongoc_client_t* m_p = nullptr;
	};

	// wrapper for mongoc_collection_t*
	class AutoColl
	{
		AutoColl(AutoColl& rhs) = delete;
		AutoColl(AutoColl&& rhs) = delete;
		AutoColl& operator = (AutoColl& rhs) = delete;
		AutoColl& operator = (AutoColl&& rhs) = delete;
	public:
		AutoColl(mongoc_collection_t* p) : m_p(p) { };

		AutoColl(mongoc_client_t* client, std::string& db, std::string& coll) 
			{ m_p = mongoc_client_get_collection(client, db.c_str(), coll.c_str()); };

		~AutoColl() { if (m_p) mongoc_collection_destroy(m_p); };
		
		operator mongoc_collection_t* () { return m_p; }

		mongoc_collection_t* Release(void)
		{
			mongoc_collection_t* coll = m_p;
			m_p = nullptr;
			return coll;
		}

	private:
		mongoc_collection_t* m_p = nullptr;
	};

	// wrapper for bson_t*
	class AutoBson
	{
		// no copy constructor & copy assign
		AutoBson(AutoBson& rhs) = delete;
		AutoBson& operator=(AutoBson& rhs) = delete;
	public:
		AutoBson() : m_p(bson_new()) { };
		AutoBson(bson_t* p) : m_p(p) { };

		// need move to BuildBson
		AutoBson(AutoBson&& rhs) { m_p = rhs.Release(); }
		AutoBson& operator=(AutoBson&& rhs) { m_p = rhs.Release(); return *this; }

		~AutoBson() { if (m_p) bson_destroy(m_p); };

		operator bson_t* () { return m_p; };
		
		bson_t* Release(void)
		{
			bson_t* p = m_p;
			m_p = nullptr;
			return p;
		}

		// add key value
		bool Add(const std::string& key, const bool val)
			{ return BSON_APPEND_BOOL(m_p, key.c_str(), val); }
		bool Add(const std::string& key, const int val)
			{ return BSON_APPEND_INT32(m_p, key.c_str(), val); }
		bool Add(const std::string& key, const long long val)
			{ return BSON_APPEND_INT64(m_p, key.c_str(), val); }
		bool Add(const std::string& key, const double val)
			{ return BSON_APPEND_DOUBLE(m_p, key.c_str(), val); }
		bool Add(const std::string& key, const std::string& val)
			{ return BSON_APPEND_UTF8(m_p, key.c_str(), val.c_str()); }
		bool Add(const std::string& key, const std::wstring& val)
			{ return BSON_APPEND_UTF8(m_p, key.c_str(), EncUtf8(val).c_str()); }
		bool AddBin(const std::string& key, const std::string& val)
			{ return BSON_APPEND_BINARY(m_p, key.c_str(), BSON_SUBTYPE_BINARY, (const uint8_t*)val.c_str(), val.length()); }

		template<class IterStdString>
		bool Add(const std::string& key, IterStdString first, IterStdString last)
			{ return BsonAddArray(m_p, first, last); }
		template<class IterStdWString>
		bool AddW(const std::string& key, IterStdWString first, IterStdWString last)
			{ return BsonAddArrayW(m_p, first, last); }
		bool AddVec(const std::string& key, std::vector<std::string>& val)
			{ return BsonAddArray(m_p, key.c_str(), val); }

	private:
		bson_t* m_p = nullptr;
	};

	// wrapper for bson_t
	class StackBson
	{
		StackBson(StackBson& rhs) = delete;
		StackBson(StackBson&& rhs) = delete;
		StackBson& operator = (StackBson& rhs) = delete;
		StackBson& operator = (StackBson&& rhs) = delete;
	public:
		StackBson()  { bson_init(&doc); };

		~StackBson() { bson_destroy(&doc); };

		operator bson_t* () { return &doc; };
	private:
		bson_t doc;
	};

	// wrapper for char* from bson_as_json
	class AutoJson
	{
		AutoJson(AutoJson& rhs) = delete;
		AutoJson(AutoJson&& rhs) = delete;
		AutoJson& operator = (AutoJson& rhs) = delete;
		AutoJson& operator = (AutoJson&& rhs) = delete;
	public:
		explicit AutoJson(char* p) : m_p(p) { };

		AutoJson(const bson_t* p) { if (p) m_p = bson_as_json(p, nullptr); };

		~AutoJson() { if (m_p) bson_free(m_p); }; 

		operator char* () { return m_p; }

	private:
		char* m_p = nullptr;
		const char* const m_def = " NO CONTENT ";
	};

	// wrapper for mongoc_cursor_t* and it's result (bson_t*) 
	class AutoCursor
	{
		AutoCursor(AutoCursor& rhs) = delete;
		AutoCursor& operator=(AutoCursor& rhs) = delete;
		AutoCursor& operator=(AutoCursor&& rhs) = delete;
	public:
		AutoCursor(mongoc_cursor_t* p) : m_cursor(p) { };

		AutoCursor(AutoCursor&& rhs)
		{
			m_cursor = rhs.m_cursor;
			m_doc = rhs.m_doc;
			rhs.m_cursor = nullptr;
			rhs.m_doc = nullptr;
		};

		~AutoCursor() { if (m_cursor) mongoc_cursor_destroy(m_cursor); };

		bool IsValid() { return m_cursor ? true : false; }
		bool operator!() { return !IsValid(); }

		mongoc_cursor_t* Cursor() { return m_cursor; }
		
		bool Next()
		{
			if (!m_cursor) { return false; }
			return mongoc_cursor_next(m_cursor, &m_doc);
		}

		// return result bson by Next()
		const bson_t* ResultBson() { return m_doc; }
		operator const bson_t* () { return ResultBson(); }

		// get result key value
		bool Bool(const std::string& key)
		{
			return BsonValueBool(m_doc, key);
		}
		int Int32(const std::string& key)
		{
			return BsonValueInt32(m_doc, key);
		}
		long long Int64(const std::string& key)
		{
			return BsonValueInt64(m_doc, key);
		}
		double Double(const std::string& key)
		{
			return BsonValueDouble(m_doc, key);
		}
		std::string Str(const std::string& key)
		{
			return BsonValStr(m_doc, key);
		}
		std::string Bin(const std::string& key)
		{
			return BsonValBin(m_doc, key);
		}

		template<class IterStdStringInsert>
		bool BsonStr(const std::string& key, IterStdStringInsert result)
		{
			return BsonValue(m_doc, key, result);
		}
		template<class IterStdWStringInsert>
		bool BsonWStr(const std::string& key, IterStdWStringInsert result)
		{
			return BsonValueW(m_doc, key, result);
		}

	private:
		mongoc_cursor_t* m_cursor = nullptr; 
		const bson_t* m_doc = nullptr;
	};


	//////////////////////////////////////////////////////////////////////////
	// used with mongoc_client_pool_t

	// wrapper for mongoc_client_t* from client's pool
	class AutoPoolClient
	{
		AutoPoolClient(AutoPoolClient& rhs) = delete;
		AutoPoolClient(AutoPoolClient&& rhs) = delete;
		AutoPoolClient& operator = (AutoPoolClient& rhs) = delete;
		AutoPoolClient& operator = (AutoPoolClient&& rhs) = delete;
	public:
		AutoPoolClient();

		~AutoPoolClient();

		operator mongoc_client_t* () { return m_p; }

		mongoc_client_t* Release(void)
		{
			mongoc_client_t* client = m_p;
			m_p = nullptr;
			return client;
		}

	private:
		mongoc_client_t* m_p = nullptr;
	};

	// wrapper for client's pool
	// auto push & pop client from mongoc_client_pool_t and CRUD on collection
	// use Error() for detail when failed.
	class AutoPoolColl
	{
		AutoPoolColl(AutoPoolColl& rhs) = delete;
		AutoPoolColl(AutoPoolColl&& rhs) = delete;
		AutoPoolColl& operator = (AutoPoolColl& rhs) = delete;
		AutoPoolColl& operator = (AutoPoolColl&& rhs) = delete;
	public:
		AutoPoolColl() { };

		AutoPoolColl(mongoc_client_t* clientFromPool, mongoc_collection_t* coll);

		explicit AutoPoolColl(const std::string& collname);
		
		~AutoPoolColl() { Destroy(); };

		bool IsValid(void)
		{
			if (m_client && m_coll) { return true; }

			SetErr("Null collection");
			return false;
		}

		bool operator!() { return !IsValid(); }

		operator mongoc_collection_t* () { return m_coll; }

		bool Insert(bson_t* doc, mongoc_insert_flags_t flags = MONGOC_INSERT_NONE)
		{
			if (!IsValid() || !doc) { return false; }
			return mongoc_collection_insert(m_coll, flags, doc, nullptr, &m_err);
		};

		bool Delete(bson_t* query, mongoc_remove_flags_t flags = MONGOC_REMOVE_NONE)
		{
			if (!IsValid() || !query) { return false; }
			return mongoc_collection_remove(m_coll, flags, query, NULL, &m_err);
		}

		// return true if record not exist but no DB operation failure
		bool Update(bson_t* query, bson_t* update, mongoc_update_flags_t flags = MONGOC_UPDATE_NONE)
		{
			if (!IsValid() || !query || !update) { return false; }
			return mongoc_collection_update(m_coll, flags, query, update, nullptr, &m_err);
		}

		// return false if record not exist. only one record could be modified.
		bool Modify(bson_t* query, bson_t* update);

		mongoc_cursor_t* Find(bson_t* query, int maxResult = 0, bson_t* field = nullptr,
			int skipResult = 0, mongoc_query_flags_t flags = MONGOC_QUERY_NONE, const mongoc_read_prefs_t* read_prefs = nullptr)
			{ return mongoc_collection_find(m_coll, flags, skipResult, maxResult, 0, query, field, read_prefs); }

		const char* Error(void) const { return m_err.message; }

	private:
		mongoc_client_t* m_client = nullptr;
		mongoc_collection_t* m_coll = nullptr;
		bson_error_t m_err;

		void Destroy(void);


		void SetErr(const std::string& str)
		{
			size_t len = sizeof(m_err.message);

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4996)
#endif // _MSC_VER

			memccpy(m_err.message, str.c_str(), 0, len);

#ifdef _MSC_VER
	#pragma warning(pop)
#endif // _MSC_VER

			m_err.message[len - 1] = 0;
		}
	};
	
} // MongoClib

#endif // MongoAuto_h__
