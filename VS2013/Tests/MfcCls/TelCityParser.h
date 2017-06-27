#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

class TelCityParser
{
public:
	TelCityParser();
	~TelCityParser();
	bool Init(const std::string& dataPath = ".");
	std::string CityCodeFromTel(const std::string& tel) const;


	bool IsMatchTelCity(const std::string& tel, const std::string& citycode);
	bool IsMatchTelProv(const std::string& tel, const std::string& provname);

	// city code, city name, prov name
	std::tuple<std::string, std::string, std::string>
		GetInfoFromTel(const std::string& tel) const;

	// country code, country name
	std::tuple<std::string, std::string>
		GetCountryFromTel(const std::string& tel);
private:
	bool IsIpPhone(const std::string& tel) const;
	std::string RemovePrefix(const std::string& tel) const;
	std::string ValueFromMapByKey(const std::unordered_map <std::string, std::string>& m, 
									const std::string key) 
									const;

	const int knMobileTelSegLen = 7;
	const int knCityCodeLenMin = 3;
	const int knCityCodeLenMax = 4;
	// config path
	const std::string CountryCode2NamePath = "\\sysconfig\\PhoneSegCountry.cfg";
	const std::string CityCode2NamePath = "\\sysconfig\\City.cfg";
	const std::string CityCode2ProvincePath = "\\sysconfig\\Province.cfg";
	const std::string Tel2CityCodePath = "\\sysconfig\\PhoneSegCity.cfg";
	const std::string IpPhonePrefixPath = "\\sysconfig\\PhoneIp.cfg";
	const std::string TelPrefixPath = "\\sysconfig\\phonePrefix.cfg";

	// data
	std::unordered_map <std::string, std::string> mapCountryCode2Name;
	std::unordered_map <std::string, std::string> mapCityCode2Name;
	std::unordered_map <std::string, std::string> mapCityCode2Prov;
	std::unordered_map <std::string, std::string> mapTel2CityCode;
	std::unordered_set <std::string> setIpPrefix;
	std::unordered_set <std::string> setTelPrefix;

	// read data function
	std::unordered_map <std::string, std::string> 
		ReadMap(const std::string& filename);
	std::unordered_set <std::string>
		ReadSet(const std::string& filename);
	std::pair<std::string, std::string> ParseLine2Pair(const std::string& line);
};
