#include "stdafx.h"
#include "MongoFunc.h"

#include <mongoc.h>
#include <iostream>
#include <sstream>
#include <memory>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const string g_strSrvIP = "192.168.1.120";
unsigned short g_uPort = 27017;
const string g_strDataBase = "libcTest";

void PrintBson(const bson_t* pbson)
{
	char* str = bson_as_json(pbson, nullptr);
	cout << str << endl;
	bson_free(str);
}

void TestMongo(void)
{
	Insert();
	Load();
// 	bson_t* query = nullptr;
// 	query = BCON_NEW("$query", "{", "foo", BCON_INT32(1), "cond2", BCON_UTF8("string2"), "}", "$orderby","{", "foo", BCON_INT32(-1), "}");
// 	
// 	char* str = bson_as_json(query, nullptr);
// 	bson_free(str);
// 
// 	bson_destroy(query);
//	

	cout << endl;
	cout << "finished. press enter to exit." << endl;
	getchar();
}

void Insert(void)
{
	mongoc_client_t *client;
	mongoc_collection_t *collection;
	bson_error_t error;
	bson_t *doc;
	
	client = mongoc_client_new("mongodb://192.168.1.120:27017/");
	collection = mongoc_client_get_collection(client, "mydb", "mycoll");

	const int nBinLen = 100;
	char* pBin = new char[nBinLen];
	for (int ii = 0; ii < nBinLen; ++ii)
	{
		pBin[ii] = ii;
	}
	unique_ptr<char> upBin(pBin);
	
	for (int ii = 0; ii < 1000; ++ii)
	{
		doc = bson_new();
		BSON_APPEND_INT32(doc, "idx", ii);
		stringstream ss;
		ss << "object_" << ii;
		BSON_APPEND_UTF8(doc, "name", ss.str().c_str());
		ss.str("");
		
		BSON_APPEND_INT32(doc, "group", ii % 10);
		BSON_APPEND_INT32(doc, "tag", ii / 10);

		BSON_APPEND_BINARY(doc, "bin", BSON_SUBTYPE_BINARY, (uint8_t*)pBin, nBinLen);

		char* str = bson_as_json(doc, NULL);
		printf("%s\n", str);
		bson_free(str);

		if (!mongoc_collection_insert(collection, MONGOC_INSERT_NONE, doc, NULL, &error)) {
			fprintf(stderr, "%s\n", error.message);
		}

		bson_destroy(doc);
	}
	mongoc_collection_destroy(collection);
	mongoc_client_destroy(client);
}

void Delete(void)
{
	mongoc_client_t *client;
	mongoc_collection_t *collection;
	bson_error_t error;
	bson_t *doc;

	client = mongoc_client_new("mongodb://192.168.1.120:27017/");
	collection = mongoc_client_get_collection(client, "mydb", "mycoll");

	doc = BCON_NEW("name", BCON_UTF8("object_20"));
	
	if (!mongoc_collection_remove(collection, MONGOC_REMOVE_NONE, doc, NULL, &error)) {
		printf("Delete failed: %s\n", error.message);
	}

	bson_destroy(doc);
	mongoc_collection_destroy(collection);
	mongoc_client_destroy(client);
}

void Load(void)
{
	mongoc_client_t *client;
	mongoc_collection_t *collection;
	mongoc_cursor_t *cursor;
	const bson_t *doc;
	bson_t *query;
	char *str;

	client = mongoc_client_new("mongodb://192.168.1.120:27017/");
	collection = mongoc_client_get_collection(client, "mydb", "mycoll");

// 	bson_t* cntr;
// 	cntr = BCON_NEW("name", BCON_INT32(1));  // only name field
// 	PrintBson(cntr);
// 
	// all
// 	query = bson_new();
	// skip 30, max 20
// 	cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, 30, 20, 0, query, cntr, NULL);
// 
// 	while (mongoc_cursor_next(cursor, &doc)) {
// 		str = bson_as_json(doc, NULL);
// 		printf("%s\n", str);
// 		bson_free(str);
// 	}
//
// 	bson_destroy(query);
// 	bson_destroy(cntr);
// 	mongoc_cursor_destroy(cursor);

// 	query = bson_new();
// 	query = BCON_NEW("$query", "{", "name", "{", "$gte", "object_100", "$lt", "object_200", "}",
// 		"group", BCON_INT32(3), "}",
// 		"$orderby", "{", "tag", BCON_INT32(-1), "}");
// 	query = BCON_NEW("$query", "{", "$or", "[",
// 		"{", "tag", "{", "$gte", "90", "}", "}",
// 		"{", "tag", "{", "$lt", "10", "}", "}",
// 		"{", "group", BCON_INT32(3), "}",
// 		"]", "}");
// 	query = BCON_NEW("tag", "{", "$gte", BCON_INT32(2), "}", "tag", "{", "$lte", BCON_INT32(4), "}");
	char* pJson = "{ \"group\" : { \"$gte\" : 2, \"$lt\"  : 4}, \"tag\" : 10 }";
	query = bson_new_from_json((uint8_t*)pJson, -1, nullptr);

	PrintBson(query);
	cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, 0, 20, 0, query, nullptr, nullptr);

	while (mongoc_cursor_next(cursor, &doc)) {
		str = bson_as_json(doc, NULL);
		printf("%s\n", str);
		bson_free(str);
	}

	bson_destroy(query);

	mongoc_cursor_destroy(cursor);
	mongoc_collection_destroy(collection);
	mongoc_client_destroy(client);
}

void Update(void)
{
	mongoc_collection_t *collection;
	mongoc_client_t *client;
	bson_error_t error;
	bson_t *doc = NULL;
	bson_t *update = NULL;
	bson_t *query = NULL;
	
	client = mongoc_client_new("mongodb://192.168.1.120:27017/");
	collection = mongoc_client_get_collection(client, "mydb", "mycoll");
	
	query = BCON_NEW("group", BCON_INT32(1));
	update = BCON_NEW("$set", "{", "group", BCON_INT32(11), "}");

	if (!mongoc_collection_update(collection, MONGOC_UPDATE_MULTI_UPDATE, query, update, NULL, &error)) {
		printf("%s\n", error.message);
	}
	else
	{
		cout << "update ok" << endl;
	}

	if (doc)
		bson_destroy(0);
	if (query)
		bson_destroy(query);
	if (update)
		bson_destroy(update);

	mongoc_collection_destroy(collection);
	mongoc_client_destroy(client);
}

void Count(void)
{
	mongoc_client_t *client;
	mongoc_collection_t *collection;
	bson_error_t error;
	bson_t *doc;
	int64_t count;


	client = mongoc_client_new("mongodb://192.168.1.120:27017/");
	collection = mongoc_client_get_collection(client, "mydb", "mycoll");

	doc = BCON_NEW("tag", BCON_INT32(10));

	count = mongoc_collection_count(collection, MONGOC_QUERY_NONE, doc, 0, 0, NULL, &error);

	if (count < 0) {
//		str = bson_as_json(&error, NULL);
		fprintf(stderr, "%s\n", error.message);
//		bson_free(str);
	}
	else {
		fprintf(stdout, "%" PRIu64 "\n", count);
	}

	bson_destroy(doc);
	mongoc_collection_destroy(collection);
	mongoc_client_destroy(client);
}

void CreateIndex(void)
{
	mongoc_client_t *client;
	mongoc_collection_t *collection;
	bson_t* keys;
	bson_error_t error;

	string strIndexName = "tag";

	mongoc_index_opt_t opt;
	mongoc_index_opt_init(&opt);
	opt.unique = true;

	keys = BCON_NEW("tag", BCON_INT32(1));
	
	client = mongoc_client_new("mongodb://192.168.1.1:27017/");
	collection = mongoc_client_get_collection(client, "mydb", "mycoll");


	if (mongoc_collection_create_index(collection, keys, &opt, &error)) {
		cout << "succeed index on " << strIndexName << endl;
	}
	else
	{
		cout << "failed: " << error.message << endl;
	}

	bson_destroy(keys);
	mongoc_collection_destroy(collection);
	mongoc_client_destroy(client);
}

void FindAndModify(void)
{
	mongoc_collection_t *collection;
	mongoc_client_t *client;
	bson_error_t error;
	bson_t *query;
	bson_t *update;
	bson_t reply;
	char *str;

	client = mongoc_client_new("mongodb://192.168.1.120:27017/");
	collection = mongoc_client_get_collection(client, "mydb", "mycoll");

	/*
	* Build our query, {"cmpxchg": 1}
	*/
	query = BCON_NEW("group", BCON_INT32(3));

	/*
	* Build our update. {"$set": {"cmpxchg": 2}}
	*/
	update = BCON_NEW("$set", "{", "group", BCON_INT32(33), "}");

	/*
	* Submit the findAndModify.
	*/
	if (!mongoc_collection_find_and_modify(collection, query, NULL, update, NULL, false, false, false, &reply, &error)) {
		fprintf(stderr, "find_and_modify() failure: %s\n", error.message);		
	}
	else
	{
		/*
		* Print the result as JSON.
		*/
		str = bson_as_json(&reply, NULL);
		printf("%s\n", str);
		bson_free(str);
	}


	/*
	* Cleanup.
	*/
	bson_destroy(query);
	bson_destroy(update);
	bson_destroy(&reply);
	mongoc_collection_destroy(collection);
	mongoc_client_destroy(client);
}
void FindAndModifyDel(void)
{
	mongoc_collection_t *collection;
	mongoc_client_t *client;
	bson_error_t error;
	bson_t *query;
	bson_t reply;

	client = mongoc_client_new("mongodb://192.168.1.120:27017/");
	collection = mongoc_client_get_collection(client, "mydb", "mycoll");

	query = BCON_NEW("name", BCON_UTF8("object_19"));

	/*
	* Submit the findAndModify.
	*/
while (true)
{
	if (!mongoc_collection_find_and_modify(collection, query, NULL, NULL, NULL, true, false, false, &reply, &error)) {
		fprintf(stderr, "find_and_modify() failure: %s\n", error.message);		
	}
	else
	{
		/*
		* Print the result as JSON.
		*/
		string val;
		if (!BsonJson(&reply, "value", val))
		{
			bson_destroy(&reply);
			cout << "no record found " << endl;
			break;
		}
		else
		{
			cout << "deleted: " << endl << val << endl;
		}
		bson_destroy(&reply);
	}
}
	bson_destroy(query);
	mongoc_collection_destroy(collection);
	mongoc_client_destroy(client);
}

void FindAndModifyUpdate(void)
{
	mongoc_collection_t *collection;
	mongoc_client_t *client;
	bson_error_t error;
	bson_t *query;
	bson_t *update;
	bson_t reply;
//	bson_t* reply = bson_new();
//	bson_init(reply);
	char *str;

	client = mongoc_client_new("mongodb://192.168.1.120:27017/");
	collection = mongoc_client_get_collection(client, "testdb", "Student");

	/*
	* Build our query, {"cmpxchg": 1}
	*/
	query = BCON_NEW("strName", BCON_UTF8("Name_0001"));

	/*
	* Build our update. {"$set": {"cmpxchg": 2}}
	*/
	update = BCON_NEW("$set", "{", "nValue", BCON_INT32(11), "}");

	/*
	* Submit the findAndModify.
	*/
	if (!mongoc_collection_find_and_modify(collection, query, NULL, update, NULL, false, false, false, &reply, &error)) {
		fprintf(stderr, "find_and_modify() failure: %s\n", error.message);		
	}
	else
	{

		string val;
		if (!BsonJson(&reply, "value", val))
		{
			cout << "no record found " << endl;
		}
		else
		{
			cout << "deleted: " << endl << val << endl;
		}
		/*
		* Print the result as JSON.
		*/
		str = bson_as_json(&reply, NULL);
		printf("%s\n", str);
		bson_free(str);
	}


	/*
	* Cleanup.
	*/
	bson_destroy(query);
	bson_destroy(update);
	bson_destroy(&reply);
	mongoc_collection_destroy(collection);
	mongoc_client_destroy(client);
}
bool BsonIterValue(const bson_t* doc, const std::string key, bson_iter_t& ival)
{
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

bool BsonJson(const bson_t* doc, const std::string key, std::string& val)
{
	bson_iter_t ival;
	uint32_t len = 0;

		uint8_t* pdoc = nullptr;
	if (BsonIterValue(doc, key, ival) && BSON_ITER_HOLDS_DOCUMENT(&ival))
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

bool BsonValue(const bson_t* doc, const std::string key, bool& val)
{
	bson_iter_t ival;

	if (BsonIterValue(doc, key, ival) && BSON_ITER_HOLDS_BOOL(&ival))
	{
		val = bson_iter_bool(&ival);
		return true;
	}
	return false;
}
bool BsonValue(const bson_t* doc, const std::string key, int& val)
{
	bson_iter_t ival;

	if (BsonIterValue(doc, key, ival) && BSON_ITER_HOLDS_INT32(&ival))
	{
		val = bson_iter_int32(&ival);
		return true;
	}
	return false;
}
bool BsonValue(const bson_t* doc, const std::string key, double& val)
{
	bson_iter_t ival;

	if (BsonIterValue(doc, key, ival) && BSON_ITER_HOLDS_DOUBLE(&ival))
	{
		val = bson_iter_double(&ival);
		return true;
	}
	return false;
}
bool BsonValue(const bson_t* doc, const std::string key, std::string& val)
{
	bson_iter_t ival;
	uint32_t len = 0;

	if (BsonIterValue(doc, key, ival) && BSON_ITER_HOLDS_UTF8(&ival))
	{
		val = bson_iter_utf8(&ival, &len);
		return true;
	}
	return false;
}

bool BsonValue(const bson_t* doc, const std::string key, std::vector<std::string>& val)
{
	bson_iter_t ival;
	uint32_t len = 0;

	if (BsonIterValue(doc, key, ival) && BSON_ITER_HOLDS_ARRAY(&ival))
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


void BsonTest(void)
{

	// no leak
// 	bson_t * a = bson_new();
// 	bson_reinit(a);
// 	bson_destroy(a);
// 	return;

	// no leak
// 	bson_t doc;
// 	bson_init(&doc);
// 	return;
		
	// memory leak
// 	bson_t * a = bson_new();
// 	bson_init(a);
// 	bson_destroy(a);
// 	return;

	// memory leak
// 	bson_t* src = bson_new();
// 	bson_t* dst = bson_new();
// 	bson_copy_to(src, dst);
// 
// 	bson_destroy(src);
// 	bson_destroy(dst);
// 	return;

	// no leak
// 	bson_t* src = bson_new();
// 	bson_t cpy;
// 	bson_t* dst = &cpy;
// 	bson_init(dst);
// 	bson_copy_to(src, dst);
// 
// 	bson_destroy(src);
// 	return;

	// no leak
	// if src is INLINE, dst in stack & no destroy, no mem-leak
// 	char* p = "{ \"value\" : null, \"ok\" : 1 }";
// 	bson_t* src = bson_new_from_json((uint8_t*)p, -1, nullptr);
// 	bson_t cpy;
// 	bson_t* dst = &cpy;
// 	bson_copy_to(src, dst);
// 
// 	bson_destroy(src);
// 	return;
	
	// no leak
	// src is no INLINE (too big for bson_t buffer), dst must destroy or will cause mem-leak
// 	char* p = "{ \"value\" : \"assssssssssssssssssssssssssssssssssssssssssssssssssssssssss\
// ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss\
// ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss\
// ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss\", \"ok\" : 1 }";
// 	bson_t* src = bson_new_from_json((uint8_t*)p, -1, nullptr);
// 	bson_t cpy;
// 	bson_t* dst = &cpy;
// 	bson_copy_to(src, dst);
// 
// 	bson_destroy(src);
// 	bson_destroy(dst);
// 	return;

	// memory leak
	// src is no INLINE (too big for bson_t buffer), dst must destroy or will cause mem-leak
// 	char* p = "{ \"value\" : \"assssssssssssssssssssssssssssssssssssssssssssssssssssssssss\
// ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss\
// ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss\
// ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss\", \"ok\" : 1 }";
// 	bson_t* src = bson_new_from_json((uint8_t*)p, -1, nullptr);
// 	bson_t cpy;
// 	bson_t* dst = &cpy;
// 	bson_copy_to(src, dst);
// 
// 	bson_destroy(src);
// 	return;



}