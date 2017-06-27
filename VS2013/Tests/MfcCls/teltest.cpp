#include "stdafx.h"
#include "PhoneSegParse.h"
#include "TelCityParser.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

//////////////////////////////////////////////////////////////////////////
// map vs unordered_map
/*
	31W 7位数字字串，查询时间unordered_map比map略快(不足10%)，但是都是在150ms左右。
*/

vector<string> LoadTels(void)
{
	vector<string> cntr;
	auto filename = ".\\sysconfig\\PhoneSegCity.cfg";
	fstream fs(filename, ios::in);
	if (!fs)
	{
		return cntr;
	}
	string line;
	while (fs >> line)
	{

		static const auto chSrc = '|';
		static const auto chDst = ' ';
		for (auto& it : line)
		{
			if (it == chSrc)
			{
				it = chDst;
			}
		}
		istringstream iss(line);
		string str1;
		string str2;
		iss >> str1;
		iss >> str2;
		if (!str1.empty() && !str2.empty())
		{
			cntr.push_back(str1);
		}
	}
	return cntr;
}

void TelTestOld(void)
{
//	telParser.InitAll(".");
	CPhoneSegParse telParser;
	telParser.InitAll(".");

//	bool FindCityName(char* PhoneNo,char* CityId,char* CityName, char* ProvinceName)
	char id[1024];
	char name[1024];
	char prov[1024];
	auto ret = telParser.FindCityName("15000292011", id, name, prov);
	auto r = telParser.FindCityCode("15000292011");
	r = telParser.FindCityCode("+8615000292011");
	ret = telParser.IsCityTel("08615000292011", "021");
	cout << r << endl;
}

void TelTestNew(void)
{
	TelCityParser telParser;
	telParser.Init();
	int loop = 1;
	for (int ii = 0; ii < loop; ++ii)
	{
		cout << ii << endl;
		string tel("13855128214");
		string pre("0086");
		string tel2 = pre + tel;
		auto r = telParser.CityCodeFromTel(tel);
		cout << r << endl;
		auto b = telParser.IsMatchTelCity(tel, "021");
		cout << b << endl;
		cout << telParser.IsMatchTelProv(tel, "安徽省") << endl;
		string code;
		string city;
		string prov;
		std::tie(code, city, prov) = telParser.GetInfoFromTel(tel2);
		cout << code << city << prov << endl;
	}

	// load tel
	vector<string> v;
	auto tels = LoadTels();

	auto dw = GetTickCount();
	for (auto& tel : tels)
	{
		auto code = telParser.CityCodeFromTel(tel);
		if (!code.empty())
		{
			v.push_back(code);
		}
	}
	dw = GetTickCount() - dw;
	cout << "size " << v.size() << " cost " << dw << endl;



	string code;
	string name;
	std::tie(code, name) = telParser.GetCountryFromTel("+86123456");
	cout << code << name << endl;
	std::tie(code, name) = telParser.GetCountryFromTel("00123456");
	cout << code << name << endl;
}

void ReadTelInfoOld(void)
{
	cout << "ReadTelInfoOld" << endl;
	CPhoneSegParse telParser;
	telParser.InitAll(".");

//	bool FindCityName(char* PhoneNo,char* CityId,char* CityName, char* ProvinceName)
	char id[1024];
	id[0] = 0;
	char name[1024];
	name[0] = 0;
	char prov[1024];
	prov[0] = 0;
	auto ret = telParser.FindCityName("862169917078", id, name, prov);
	cout << id << name << prov << endl;
// 	auto r = telParser.FindCityCode("15000292011");
// 	r = telParser.FindCityCode("+8615000292011");
// 	ret = telParser.IsCityTel("08615000292011", "021");
//	cout << r << endl;
}


void PrintTelInfo(const TelCityParser& telParser, const std::string& tel)
{
	string code;
	string city;
	string prov;
	std::tie(code, city, prov) = telParser.GetInfoFromTel(tel);
	cout << tel << " " << code << " " << city << " " << prov << " " << endl;
}

void ReadTelInfo(void)
{
	cout << "ReadTelInfo" << endl;
	TelCityParser telParser;
	auto b = telParser.Init();
	cout << "init " << b << endl;
	PrintTelInfo(telParser, "862169917078");
	PrintTelInfo(telParser, "00862169917078");
	PrintTelInfo(telParser, "02169917078");
}


void TelTest(void)
{
	ReadTelInfo();
}