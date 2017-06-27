#include "stdafx.h"
#include "MongoClient.h"

#include <sstream>
#include <mongoc.h>

#include "MongoAuto.h"
#include "MongocHelp.h"

#include "CommonFunction.h"

using namespace std;
using namespace CommonFunction;

namespace MongoClib
{
	const unsigned MongoTimeoutMillisec = 2;

//////////////////////////////////////////////////////////////////////////
// class MongocDrv

class MongoClient
{
	friend int Init(const std::wstring& db, const std::wstring& srv,
		int toConnectMS /*= 0*/, int toSelectMS /*= 0*/, int toSocketMS /*= 0*/,
		unsigned short port /*= 27017*/);
public:
	MongoClient();
	~MongoClient();

	void Clear();

	mongoc_client_t* TryPop(void);
	void Push(mongoc_client_t* client);

	const char* db(void) const
	{
		return m_db.c_str();
	}

	const wchar_t* wdb(void) const
	{
		return m_wdb.c_str();
	}

	protected:
	std::string m_srv;
	unsigned short m_port = 0;
	std::string m_db;
	std::wstring m_wdb;
	mongoc_client_pool_t* m_pool = nullptr;
};

MongoClient::MongoClient()
{
	Clear();
}


MongoClient::~MongoClient()
{
}

void MongoClient::Clear()
{
	m_srv = "";
	m_port = 0;
	m_db = "";
	if (m_pool)
	{
		mongoc_client_pool_destroy(m_pool);
		m_pool = nullptr;
	}
}


mongoc_client_t* MongoClient::TryPop(void)
{
	if (!m_pool)
	{
		return nullptr;
	}
	return mongoc_client_pool_try_pop(m_pool);
}

void MongoClient::Push(mongoc_client_t* client)
{
	if (!client)
	{
		return;
	}
	if (!m_pool)
	{
		mongoc_client_destroy(client);
		return;
	}
	mongoc_client_pool_push(m_pool, client);
}
//////////////////////////////////////////////////////////////////////////
// class instance
MongoClient mongoClient;


//////////////////////////////////////////////////////////////////////////
// functions

// init mongo c driver. not thread safe. to
// unsigned toXXXMS is time-out milliseconds for xxx, <= 0 is for default
// default: toConnectMS 10secs, toSelectMS 30secs, toSocketMS mins
// remember to create index after init if necessary.
int Init(const std::wstring& db, const std::wstring& srv,
	int toConnectMS /*= 0*/, int toSelectMS /*= 0*/, int toSocketMS /*= 0*/,
	unsigned short port /*= 27017*/)
{
	string u8Srv = EncodeU8(srv);
	string u8db = EncodeU8(db);
	if (!u8Srv.length() || !port || !u8db.length())
	{
		return -1;
	}
	mongoc_init();

	stringstream ss;
	ss << "mongodb://" << u8Srv << ":" << port 
		<< "?connectTimeoutMS=" << toConnectMS 
		<< "&serverSelectionTimeoutMS=" << toSelectMS
		<< "&socketTimeoutMS=" << toSocketMS;
	string struri = ss.str();
	AutoUri uri(ss.str().c_str());
	if (!uri)
	{
		return -2;
	}
	mongoc_client_pool_t* pool = mongoc_client_pool_new(uri);
	if (!pool)
	{
		return -3;
	}
		
	mongoClient.Clear();
	mongoClient.m_srv = u8Srv;
	mongoClient.m_port = port;
	mongoClient.m_db = u8db;
	mongoClient.m_wdb = db;
	mongoClient.m_pool = pool;

	// test connection
	AutoPoolColl collection("fake");
	AutoBson query;
	int64_t count = mongoc_collection_count(collection, MONGOC_QUERY_NONE, query, 0, 1, nullptr, nullptr);
	if (count < 0)
	{
		mongoClient.Clear();
		return -4;
	}
	
	// remember to create index after init.

	return 0;
}

void Cleanup(void)
{
	mongoClient.Clear();
	mongoc_cleanup();
}

const char* GetDbName(void)
{
	return mongoClient.db();
}

mongoc_client_t* TryPopClient(void)
{
	return mongoClient.TryPop();
}

void PushClient(mongoc_client_t* client)
{
	mongoClient.Push(client);
}

bool CreateIndex(mongoc_collection_t *collection, const std::string& field, bool bAsc /*= true*/, bool bUnique /*= false*/, std::string* pError /*= nullptr*/)
{
	int nAsc = bAsc ? 1 : -1;
	AutoBson keys(BCON_NEW(field.c_str(), BCON_INT32(nAsc)));
	if (!keys)
	{
		return false;
	}

	bson_error_t error;
	mongoc_index_opt_t opt;
	mongoc_index_opt_init(&opt);
	opt.unique = bUnique;

	if (!mongoc_collection_create_index(collection, keys, &opt, &error))
	{
		if (pError)
		{
			*pError = error.message;
		}
		return false;
	}

	return true;
}


bool CreateIndex(mongoc_client_t* client, const std::string& coll, const std::string& field, bool bAsc/* = true*/, bool bUnique /*= false*/, std::string* pError /*= nullptr*/)
{
	AutoColl collection(mongoc_client_get_collection(client, mongoClient.db(), coll.c_str()));
	if (!collection)
	{
		return false;
	}
	return CreateIndex(collection, field, bAsc, bUnique, pError);
}

} // MongoClib
