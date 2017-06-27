#include "stdafx.h"
#include "MongoAuto.h"
#include "MongoClient.h"
#include "MongocHelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

namespace MongoClib
{
	//////////////////////////////////////////////////////////////////////////
	// AutoPoolColl

	AutoPoolColl::AutoPoolColl(mongoc_client_t* clientFromPool, mongoc_collection_t* coll)
		: m_client(clientFromPool)
		, m_coll(coll)
	{
	}

	AutoPoolColl::AutoPoolColl(const std::string& collname)
	{
		AutoPoolClient client;
		if (!client)
		{
			return;
		}
		AutoColl collection(mongoc_client_get_collection(client, GetDbName(), collname.c_str()));
		if (!collection)
		{
			return;
		}
		m_client = client.Release();
		m_coll = collection.Release();
	}

	void AutoPoolColl::Destroy(void)
	{
		if (m_coll)
		{
			mongoc_collection_destroy(m_coll);
			m_coll = nullptr;
		}
		if (m_client)
		{
			PushClient(m_client);
			m_client = nullptr;
		}
	}

	bool AutoPoolColl::Modify(bson_t* query, bson_t* update, bool bMulti /*= false*/)
	{
		if (!IsValid() || !query || !update)
		{
			return false;
		}
		StackBson reply;
		if (!mongoc_collection_find_and_modify(m_coll, query, nullptr, update, nullptr, false, false, false, reply, &m_err))
		{
			return false;
		}
		string val;
		if (!BsonJson(reply, "value", val))
		{
			SetErr("record not exist");
			return false;
		}
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	// AutoPoolClient

	AutoPoolClient::AutoPoolClient()
	{
		m_p = TryPopClient();
	}

	AutoPoolClient::~AutoPoolClient()
	{
		// push will check null client
		PushClient(m_p);
	}

} // MongoClib