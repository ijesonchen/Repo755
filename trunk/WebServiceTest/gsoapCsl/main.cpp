

#include <afx.h>


#include "soapH.h"  
#include "WeatherWebServiceSoap.nsmap" 
#include <iostream>
#include <xstring>
using namespace std;

#define new DEBUG_NEW

// 宽 字符转UTF8 
string EncodeUtf8(const wchar_t* p) 
{ 
	wstring in(p);
	string s(in.length()*3+1,' '); 
	size_t len = ::WideCharToMultiByte(CP_UTF8, 0, 
		in.c_str(), in.length(), 
		&s[0], s.length(), 
		NULL, NULL); 
	s.resize(len); 
	return s; 
}
string EncodeUtf8(wstring in) 
{ 
	string s(in.length()*3+1,' '); 
	size_t len = ::WideCharToMultiByte(CP_UTF8, 0, 
		in.c_str(), in.length(), 
		&s[0], s.length(), 
		NULL, NULL); 
	s.resize(len); 
	return s; 
}
// UTF8 转宽字符 
wstring DecodeUtf8(const char* p) 
{ 
	string in(p);
	wstring s(in.length(), L' '); 
	size_t len = ::MultiByteToWideChar(CP_UTF8, 0, 
		in.c_str(), in.length(), 
		&s[0], s.length()); 
	s.resize(len); 
	return s; 
}


void main(void)
{
	setlocale(LC_ALL, "chs");
	const char* server = "http://www.webxml.com.cn/WebServices/WeatherWebService.asmx";
	int nRet = 0;

	const int ncity = 5;
	wchar_t acity[ncity][20] = {
		L"上海", L"北京", L"广州", L"南京", L"西安"
	};

	for (int kk = 0; kk < ncity; ++kk)
	{

		struct soap *soap = soap_new1(SOAP_C_UTFSTRING); // alloc 'soap' engine context
		string strCity = EncodeUtf8(acity[kk]);
		char* p =  (char*)strCity.c_str();

		_ns1__getWeatherbyCityName* pcity = soap_new_set__ns1__getWeatherbyCityName(soap, p);

		_ns1__getWeatherbyCityNameResponse resp;
		nRet = soap_call___ns1__getWeatherbyCityName(soap, server, 0, pcity, resp);

		if (nRet == SOAP_OK)
		{
			ns1__ArrayOfString *aos = resp.getWeatherbyCityNameResult; 

			for (int ii = 0; ii < aos->__sizestring; ++ii)
			{
				wstring wstr = DecodeUtf8(aos->string[ii]);
				wcout << wstr << endl;

			}

			cout << endl << "******************************" << endl << endl;
		}

		soap_destroy(soap); // dealloc serialization data
		soap_end(soap);     // dealloc temp data
		soap_free(soap);    // dealloc 'soap' engine context
	}

}