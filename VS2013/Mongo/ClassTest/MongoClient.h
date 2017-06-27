#ifndef MongoClient_h__
#define MongoClient_h__

#include <string>
#include <mongoc.h>
#include "CommonFunction.h"
#include "MongoAuto.h"

namespace MongoClib
{	
	// !! ONLY TryPopClient & PushClient are THREAD-SAFE

	// init mongo c driver. not thread safe.
	// unsigned toXXXMS is time-out milliseconds for xxx, <= 0 is for default
	// default: toConnectMS 10secs, toSelectMS 30secs, toSocketMS mins
	// remember to create index after init if necessary.
	int Init(const std::wstring& db, const std::wstring& srv, 
		int toConnectMS = 0, int toSelectMS = 0, int toSocketMS = 0,
		unsigned short port = 27017);
	void Cleanup(void);

	const char* GetDbName(void);
	
	// thread safe. DO NOT destroy client from PopClient, push-back when used.
	// return NULL when no client available.
	mongoc_client_t* TryPopClient(void);
	void PushClient(mongoc_client_t* client);

	
	//////////////////////////////////////////////////////////////////////////
	// create index
	bool CreateIndex(mongoc_collection_t *collection, const std::string& field, bool bAsc = true, bool bUnique = false, std::string* pError = nullptr);
	bool CreateIndex(mongoc_client_t* client, const std::string& coll, const std::string& field, bool bAsc = true, bool bUnique = false, std::string* pError = nullptr);

	inline bool CreateIndex(const std::string& coll, const std::string& field, bool bAsc = true, bool bUnique = false, std::string* pError = nullptr)
	{
		if (!coll.length() || !field.length())
		{
			return false;
		}
		mongoc_client_t* client = TryPopClient();
		if (!client)
		{
			return false;
		}

		return CreateIndex(client, coll, field, bAsc, bUnique, pError);
	}

} // MongoClib

#endif // MongoClient_h__
