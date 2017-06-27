// MongoC64.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "MongoC64.h"

#include <mongoc.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "bson-1.0.lib")
#pragma comment(lib, "mongoc-1.0.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: MFC 初始化失败\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 在此处为应用程序的行为编写代码。
	}

	mongoc_client_t *client;
	mongoc_collection_t *collection;
	mongoc_cursor_t *cursor;
	bson_error_t error;
	const bson_t *doc;
	const char *uristr = "mongodb://192.168.1.120/";
	const char *collection_name = "TRecord";
	bson_t query;
	char *str;

	mongoc_init();


	client = mongoc_client_new(uristr);

	if (!client) {
		fprintf(stderr, "Failed to parse URI.\n");
		return EXIT_FAILURE;
	}

	bson_init(&query);
	bool bRet = 0;
	// append condition, empty for all
// 	bRet = bson_append_int32(&query, "w_nProcessModeBits", -1, 127);
// 	bRet = bson_append_int32(&query, "w_nAudioEncodingTag", -1, 1);

#if 0
	bson_append_utf8(&query, "hello", -1, "world", -1);
#endif

	collection = mongoc_client_get_collection(client, "DrVoice", "TVprObject");
	cursor = mongoc_collection_find(collection,
		MONGOC_QUERY_NONE,
		0,
		0,
		0,
		&query,
		NULL,  /* Fields, NULL for all. */
		NULL); /* Read Prefs, NULL for default */

	while (mongoc_cursor_next(cursor, &doc)) {

		const bson_t* b = doc;
		bson_iter_t iter;

		// get all fileld name
		if (bson_iter_init(&iter, b))
		{
			while (bson_iter_next(&iter))
			{
				cout << bson_iter_key(&iter) << endl;
			}
		}

		bson_iter_t val;
		bson_iter_t child;
		const char* name;
		uint32_t u8len = 0;

		if (bson_iter_init(&iter, b))
		{
			name = "_id";
			if (bson_iter_find_descendant(&iter, name, &val) &&
				BSON_ITER_HOLDS_UTF8(&val))
			{
				cout << name << " : " << bson_iter_utf8(&val, &u8len) << endl;
			}

			name = "w_strVprId";
			if (bson_iter_find_descendant(&iter, name, &val) &&
				BSON_ITER_HOLDS_UTF8(&val))
			{
				cout << name << " : " << bson_iter_utf8(&val, &u8len) << endl;
			}

			name = "w_bIsHighTh";
			if (bson_iter_find_descendant(&iter, name, &val) &&
				BSON_ITER_HOLDS_BOOL(&val))
			{
				cout << name << " : " << bson_iter_bool(&val) << endl;
			}

			name = "w_fThreshold";
			if (bson_iter_find_descendant(&iter, name, &val) &&
				BSON_ITER_HOLDS_DOUBLE(&val))
			{
				cout << name << " : " << bson_iter_double(&val) << endl;
			}

			name = "w_nVprState";
			if (bson_iter_find_descendant(&iter, name, &val) &&
				BSON_ITER_HOLDS_INT32(&val))
			{
				cout << name << " : " << bson_iter_int32(&val) << endl;
			}


// 			name = "subdoc";
// 			if (bson_iter_find_descendant(&iter, name, &val) &&
// 				BSON_ITER_HOLDS_DOCUMENT(&val) &&
// 				bson_iter_recurse(&val, &child))
// 			{
// 				cout << name << " : " << endl;
// 				while (bson_iter_next(&child))
// 				{
// 					// PROC DOC
// 				}
// 				cout << endl;
// 			}

			name = "w_vtPosFileNames";
			if (bson_iter_find_descendant(&iter, name, &val) &&
				BSON_ITER_HOLDS_ARRAY(&val) &&
				bson_iter_recurse(&val, &child))
			{
				cout << name << " : " << endl;
				while (bson_iter_next(&child))
				{
					if (BSON_ITER_HOLDS_UTF8(&child))
					{
						cout << bson_iter_utf8(&child, &u8len) << endl;
					}
				}
				cout << endl;
			}
		}




		str = bson_as_json(doc, NULL);
		fprintf(stdout, "%s\n", str);
		bson_free(str);
	}

	if (mongoc_cursor_error(cursor, &error)) {
		fprintf(stderr, "Cursor Failure: %s\n", error.message);
		return EXIT_FAILURE;
	}

	bson_destroy(&query);
	mongoc_cursor_destroy(cursor);
	mongoc_collection_destroy(collection);
	mongoc_client_destroy(client);

	mongoc_cleanup();

	return EXIT_SUCCESS;
}
