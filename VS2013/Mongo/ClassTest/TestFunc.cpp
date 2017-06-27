#include "stdafx.h"
#include "TestFunc.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <codecvt>
#include <fstream>
#include <memory>

#include <mongoc.h>

#include "MongoAuto.h"
#include "MongocHelp.h"
#include "MongoClient.h"
#include "CommonFunction.h"

#include "StudentAccess.h"

using namespace std;
using namespace MongoClib;
using namespace CommonFunction;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const char* szSrv = "mongodb://192.168.1.120:27017/";
const char* szDb  = "mydb";
const char* szColl = "mycoll";

void Trap(void* p = nullptr)
{
	if (p)
	{
		return;
	}
	cout << "trapped!" << endl;
	exit(-1);
}

void TestMongo(void)
{
	TestAnsi();

	Cleanup();
	cout << endl;
	cout << "finished. press enter to exit." << endl;
	getchar();
}

void TestIndex(void)
{
	AutoClient client(szSrv);
	Trap(client);
	AutoColl coll(mongoc_client_get_collection(client, szDb, szColl));
	Trap(coll);

	string err;
	if (!CreateIndex(coll, "name", true, true,&err))
	{
		Trap(nullptr);
	}

	cout << "TestIndex passed." << endl;
}

void TestBuildDoc(void)
{
	bool bRet = false;
	AutoBson doc;
	bRet = BSON_APPEND_INT32(doc, "idx", 18);
	bRet = BSON_APPEND_UTF8(doc, "name", "strname");

	vector<string> vt;
	int nvt = 5;
	for (int ii = 0; ii < nvt; ++ii)
	{
		stringstream ss;
		ss << "path" << ii;
		vt.push_back(std::move(ss.str()));
	}
	bRet = BsonAddArray(doc, "da", vt);

	// add doc array
	AutoBson array;
	for (int ii = 0; ii < nvt; ++ii)
	{
		stringstream ss;
		ss << ii;
		string s = ss.str();
		AutoBson elem;
		bRet = BSON_APPEND_INT32(elem, "idx", ii);
		bRet = BSON_APPEND_UTF8(elem, "name", "childname");
		bRet = BSON_APPEND_DOCUMENT(array, s.c_str(), elem);
	}
	bRet = BSON_APPEND_ARRAY(doc, "docarray", array);

	AutoBson child;
	bRet = BSON_APPEND_INT32(child, "idx", 118);
	bRet = BSON_APPEND_UTF8(child, "name", "childname");
	bRet = BSON_APPEND_DOCUMENT(doc, "dc", child);

	AutoJson js(bson_as_json(doc, nullptr));

	AutoClient client( ("mongodb://192.168.1.120:27017/"));
	AutoColl collection(  mongoc_client_get_collection(client, "mydb", "mycoll"));

	bson_error_t error;
	if (!mongoc_collection_insert(collection, MONGOC_INSERT_NONE, doc, NULL, &error)) {
		fprintf(stderr, "%s\n", error.message);
	}

	AutoBson query;

	AutoCursor cursor (mongoc_collection_find(collection,
		MONGOC_QUERY_NONE,
		0,
		0,
		0,
		query,
		NULL,  /* Fields, NULL for all. */
		NULL)); /* Read Prefs, NULL for default */

	const bson_t * b = nullptr;

	while (mongoc_cursor_next(cursor.Cursor(), &b)) {

		bson_iter_t iter;

		// get all fileld name
		if (bson_iter_init(&iter, b))
		{
			while (bson_iter_next(&iter))
			{
				cout << bson_iter_key(&iter) << endl;
			}
		}

		string oid;
		int idx;
		string name;
		vector<string> vt;
		int cid;
		string cname;
		bRet = BsonOid(b, "_id", oid);
		bRet = BsonValue(b, "idx", idx);
		bRet = BsonValue(b, "name", name);
		bRet = BsonValue(b, "da", vt);

 		bson_t* ic = nullptr;
 		bRet = BsonDoc(b, "dc", ic);
		bRet = BsonValue(ic, "idx", cid);
		bRet = BsonValue(ic, "name", cname);
	}
}

void TestClient(void)
{
	int nRet = Init(L"mydb", L"192.168.1.128");
	vector<mongoc_client_t*> vtClient;
	mongoc_client_t* client = TryPopClient();

	if (!client)
	{
		Trap();
	}

	AutoColl collection(mongoc_client_get_collection(client, "mydb", "mycoll"));
	
	DWORD dw = GetTickCount();
	nRet = CreateIndex(client, "mycoll", "name");
	dw = GetTickCount() - dw;

	cout << "cost " << dw << endl;

	PushClient(client);
}


void TestConn(void)
{
	int nRet = Init(L"mydb", L"192.168.1.120", 2000, 2000, 2000);

	AutoPoolColl collection("mycoll");

	if (!collection)
	{
		return;
	}

	AutoBson query;
	AutoBson field;


	AutoCursor cursor = collection.Find(query, 0, field);
//	AutoCursor cursor(collection, query, 0, field);
	const bson_t* doc = nullptr;
	bson_error_t err;

	const bson_t* current = mongoc_cursor_current(cursor.Cursor());
	if (current)
	{
		AutoJson json(current);
		cout << json << endl;
	}
	unsigned n = mongoc_cursor_get_batch_size(cursor.Cursor());

	bool bRet = mongoc_cursor_more(cursor.Cursor());
	bRet = mongoc_cursor_next(cursor.Cursor(), &doc);
	
	bRet = mongoc_cursor_error(cursor.Cursor(), &err);

	while (cursor.Next())
	{
		AutoJson json(cursor);
		cout << json << endl;
	}
	
	long long n64 = mongoc_collection_count(collection, MONGOC_QUERY_NONE, query, 0, 0, nullptr, nullptr);
}


std::string toU8(const std::wstring& strUnicode)
{
	wstring_convert < codecvt_utf8<wchar_t>, wchar_t > convertor;
	return convertor.to_bytes(strUnicode);
}

std::wstring fromU8(const std::string& strUtf8)
{
	wstring_convert < codecvt_utf8<wchar_t>, wchar_t > convertor;
	return convertor.from_bytes(strUtf8);
}

template <class Facet>
struct deletable_facet : Facet
{
	template <class ...Args>
	deletable_facet(Args& ... args) : Facet(std::forward<Args>(args)...)
	{ }
	~deletable_facet() {}
};

void TestLoadBin(void)
{
	TestInsert();
	int nRet = Init(L"mydb", L"192.168.1.120", 2000, 2000, 2000);

	AutoPoolColl collection("mycoll");
	if (!collection)
	{
		Trap();
	}

	AutoCursor cursor = collection.Find(AutoBson());
	int ii = 0;
	while (cursor.Next())
	{
		AutoJson json(cursor);
		cout << "    ****    " <<  ii++ << endl;
		cout << json << endl;
		string str = cursor.Bin("bin");

		const char* p = str.c_str();
	}


}


void TestInsert(void)
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

		AutoJson json(doc);

		if (!mongoc_collection_insert(collection, MONGOC_INSERT_NONE, doc, NULL, &error)) {
			fprintf(stderr, "%s\n", error.message);
		}

		bson_destroy(doc);
	}
	mongoc_collection_destroy(collection);
	mongoc_client_destroy(client);
}

void TestU8(void)
{
// 	wstring wstr = L"测试aaa";
// 	wstring_convert < codecvt<wchar_t, char, mbstate_t>, wchar_t > convertor;
//
//	wstring_convert < deletable_facet<codecvt<wchar_t, char, mbstate_t>>, wchar_t > convertor;
// 	string str3 = convertor.to_bytes(wstr); 
// 	string str4 = Unicode2Ansi(wstr.c_str());
// 	if (str3 != str4)
// 	{
// 		Trap();
// 	}
//
// 	wstring wstr3 = convertor.from_bytes(str3);
// 	wstring wstr4 = Ansi2Unicode(str3.c_str());
// 	if (wstr3 != wstr4)
// 	{
// 		Trap();
// 	}
// 
// 
// 	string str1 = EncodeUtf8(wstr);
// 	string str2 = toU8(wstr);
// 	if (str1 != str2)
// 	{
// 		Trap();
// 	}
// 	wstring wstr1 = DecodeUtf8(str1);
// 	wstring wstr2 = fromU8(str1);
// 	if (wstr1 != wstr2)
// 	{
// 		Trap();
// 	}
}



void TestAnsi(void)
{
	char* p = "你好test最近";
	wchar_t* pw = L"你好test最近";

	string astr = Unicode2Ansi(pw);
	wstring wstr = Ansi2Unicode(p);

	if (astr != p)
	{
		Trap();
	}
	if (wstr != pw)
	{
		Trap();
	}
}

void TestStudent(void)
{	
	int nRet = Init(L"testdb", L"192.168.1.120", 2000, 2000, 2000);
	if (nRet)
	{
		Trap();
	}

	// drop db
	if (true)
	{
		AutoPoolClient client;
		if (client)
		{
			mongoc_database_t* db = mongoc_client_get_database(client, "testdb");
			bson_error_t err;
			bool bRet = mongoc_database_drop(db, &err);
			mongoc_database_destroy(db);
		}
	}

	deque<Student> vt0;
	int nCount = 100;
	for (int ii = 0; ii < nCount; ++ii)
	{
		Student stu;
		stu.strName.Format(L"Name_%04d", ii);
		stu.strMemo.Format(L"memo_%d", ii % 10);
		stu.bValue = ii & 1;
		stu.nValue = ii % 10;
		stu.fValue = (float)(ii % 10);
		stu.vtText.push_back(L"aaa");
		stu.vtText.push_back(L"bbb");
		stu.vtText.push_back(L"ccc");
		vt0.push_back(stu);
	}

	StudentAccess acc;
	if (!acc.CreateIndex())
	{
		Trap();
	}
	for (auto it : vt0)
	{
		if (!acc.Insert(it))
		{
			Trap();
		}
	}
	deque<Student> vt1;
	if (!acc.LoadAll(vt1))
	{
		Trap();
	}
	if (vt0 != vt1)
	{
		for (size_t ii = 0, ni = vt0.size();
			ii < ni; ++ii)
		{
			if (vt0[ii] == vt1[ii])
			{
				continue;
			}
			Trap();
		}
	}
	if (!acc.Delete(L"Name_0001"))
	{
		Trap();
	}
	if (!acc.Delete(L"Name_0001"))
	{
		Trap();
	}
	Student student = vt0.front();
	student.strMemo = L"changed.";
	student.bValue = true;
	student.nValue = 999;
	student.fValue = (float)123.45;
	if (!acc.Update(student))
	{
		Trap();
	}

	if (!acc.UpdateBool(L"Name_0001", true))
	{
		Trap();
	}

	if (acc.ModifyBool(L"Name_0001", true))
	{
		Trap();
	}

	if (!acc.UpdateBool(L"Name_0002", true))
	{
		Trap();
	}

	if (!acc.UpdateBool(L"Name_0002", false))
	{
		Trap();
	}

	if (!acc.LoadOne(L"Name_0002", student))
	{
		Trap();
	}

	vt1.clear();
	if (!acc.LoadCond(vt1, 5))
	{
		Trap();
	}

	int n = 0;
	n = acc.LoadState(L"Name_0002");
	n = acc.LoadState(L"Name_0003");
	n = acc.LoadState(L"Name_0013");
	n = acc.LoadState(L"Name_0033");
}