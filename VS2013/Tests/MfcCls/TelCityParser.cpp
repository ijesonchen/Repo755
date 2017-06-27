#include "stdafx.h"
#include "TelCityParser.h"
#include <fstream>
#include <sstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


TelCityParser::TelCityParser()
{
}


TelCityParser::~TelCityParser()
{
}


bool TelCityParser::Init(const std::string& dataPath /*= "."*/)
{
	mapCountryCode2Name = ReadMap(dataPath + CountryCode2NamePath);
	if (mapCountryCode2Name.empty())
		return false;
	
	// citycode 长度 [3,4], 05518|巢湖市|已合并到合肥
	mapCityCode2Name = ReadMap(dataPath + CityCode2NamePath);
	if (mapCityCode2Name.empty())
		return false;

	mapCityCode2Prov = ReadMap(dataPath + CityCode2ProvincePath);
	if (mapCityCode2Prov.empty())
		return false;

	mapTel2CityCode = ReadMap(dataPath + Tel2CityCodePath);
	if (mapTel2CityCode.empty())
		return false;
	
	setIpPrefix = ReadSet(dataPath + IpPhonePrefixPath);
	if (setIpPrefix.empty())
		return false;

	setTelPrefix = ReadSet(dataPath + TelPrefixPath);
	if (setTelPrefix.empty())
		return false;
	
	return true;
}

std::string TelCityParser::CityCodeFromTel(const std::string& tel) const
{
	if (IsIpPhone(tel))
	{
		return "";
	}
	auto localTel = RemovePrefix(tel);
	auto code = ValueFromMapByKey(mapTel2CityCode, localTel.substr(0, knMobileTelSegLen));
	if (code.empty())
	{
		// 固话
		if (localTel.substr(0, 1) != "0")
		{
			// 首位补0
			localTel.insert(0, "0");
		}
		for (int ii = knCityCodeLenMin; ii <= knCityCodeLenMax; ++ii)
		{
			auto citycode = localTel.substr(0, ii);
			auto cityname = ValueFromMapByKey(mapCityCode2Name, citycode);
			if (!cityname.empty())
			{
				code = citycode;
				break;
			}
		}
	}
	return code;
}

std::string TelCityParser::ValueFromMapByKey(const std::unordered_map <std::string, std::string>& m, const std::string key) const
{
	if (!key.empty())
	{
		auto p = m.find(key);
		if (p != m.end())
		{
			return p->second;
		}
	}
	return "";
}

bool TelCityParser::IsMatchTelCity(const std::string& tel, const std::string& citycode)
{
	return citycode == CityCodeFromTel(tel);
}

bool TelCityParser::IsMatchTelProv(const std::string& tel, const std::string& provname)
{
	auto citycode = CityCodeFromTel(tel);
	auto srcprov = ValueFromMapByKey(mapCityCode2Prov, citycode);
	return provname == srcprov;
}

std::tuple<std::string, std::string, std::string> 
	TelCityParser::GetInfoFromTel(const std::string& tel) const
{
	string citycode;
	string cityname;
	string provname;
	citycode = CityCodeFromTel(tel);
	cityname = ValueFromMapByKey(mapCityCode2Name, citycode);
	provname = ValueFromMapByKey(mapCityCode2Prov, citycode);
	return make_tuple(citycode, cityname, provname);
}

// country code, country name
std::tuple<std::string, std::string>
	TelCityParser::GetCountryFromTel(const std::string& tel)
{
	string code;
	string name;
	if (!IsIpPhone(tel) && !tel.empty())
	{
		string ftel = tel;
		if (ftel.at(0) == '+')
		{
			ftel.replace(0, 1, 2, '0');
		}
		for (int ii = 3; ii <= 6; ++ii)
		{
			auto telseg = ftel.substr(0, ii);
			name = ValueFromMapByKey(mapCountryCode2Name, telseg);
			if (!name.empty())
			{
				code = telseg;
				break;
			}
		}
	}
	return make_tuple(code, name);
}

bool TelCityParser::IsIpPhone(const std::string& tel) const
{
	for (const auto& it : setIpPrefix)
	{
		if (it == tel.substr(0, it.length()))
		{
			return true;
		}
	}
	return false;
}

std::string TelCityParser::RemovePrefix(const std::string& tel) const
{
	for (const auto& it : setTelPrefix)
	{
		if (it == tel.substr(0, it.length()))
		{
			// prefix found
			return tel.substr(it.length());
		}
	}
	return tel;
}

std::unordered_map <std::string, std::string> 
	TelCityParser::ReadMap(const std::string& filename)
{
	std::unordered_map <std::string, std::string> cntr;
	fstream fs(filename, ios::in);
	if (!fs)
	{
		return cntr;
	}
	string line;
	while (fs >> line)
	{
		auto pr = ParseLine2Pair(line);
		if (!pr.first.empty() && !pr.second.empty())
		{
			cntr.insert(ParseLine2Pair(line));
		}
	}
	return std::move(cntr);
}

std::unordered_set <std::string>
	TelCityParser::ReadSet(const std::string& filename)
{
	std::unordered_set <std::string> cntr;
	fstream fs(filename, ios::in);
	if (!fs)
	{
		return cntr;
	}
	string line;
	while (fs >> line)
	{
		if (!line.empty())
		{
			cntr.insert(line);
		}
	}
	return std::move(cntr);
}

std::pair<std::string, std::string> TelCityParser::ParseLine2Pair(const std::string& line)
{
	static const auto chSrc = '|';
	static const auto chDst = ' ';
	auto str(line);
	for (auto& it : str)
	{
		if (it == chSrc)
		{
			it = chDst;
		}
	}
	istringstream iss(str);
	string str1;
	string str2;
	iss >> str1;
	iss >> str2;
	return make_pair(str1, str2);
}