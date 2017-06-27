#include "stdafx.h"
#include <hash_map>
#include <string>
#include <list>
#include <map>
#include <mutex>

//using namespace std;


#define PhoneSegCountryPath "/sysconfig/PhoneSegCountry.cfg"
#define CityPath "/sysconfig/City.cfg"
#define ProvincePath "/sysconfig/Province.cfg"
#define PhoneSegCityPath "/sysconfig/PhoneSegCity.cfg"
#define IpPhoneSegPath "/sysconfig/PhoneIp.cfg"
#define PhonePrefixPath "/sysconfig/phonePrefix.cfg"

class CPhoneSegParse
{
public:
	CPhoneSegParse(char* strPath);//初始化该类
	CPhoneSegParse();//结构
	void InitAll(char* strPath);//初始刷所有数据
	bool FindCountryName(char* PhoneNo,char* CountryId,char* CountryName);       //查找该手机号属于哪个国家或地市
	bool FindCityName(char* PhoneNo, char* CityId, char* CityName, char* ProvinceName);       //查找该手机号属于哪个地市---省份
	~CPhoneSegParse();    //CPhoneSeg 析构

private:

	void InitCity();       //城市名称ID-城市名称对应表
	void InitProvince();    //城市编号对应省份名称
	void InitCountryPhoneMap();     //号码段-国家编号对应表
	void InitCityPhoneMap();        //号码段-城市编号对应表
	void InitPhoneIpMap();      //号码段-IP对应表
	void InitPhonePrefix();      //号码段前缀
	void DelPrefix(char* PrePhoneNo,char* LstPhone);     //去除前缀(86,+86,086)

	bool FindIpPhoneMap(char* CallNo,char* CallArea);       //号码段-IP地址对应表
	bool FindLocalCity(char* PhoneNo,char* CityId,char* CityName);    //查找地市编号及编码
	bool FindCountry(char* PhoneNo,char* CountryId,char* CountryName);     //查找国家编号及编码
	bool DownLoad(char *str,char * seg1,char * seg2);    //解析字符串列表
		

private:
	std::map<std::string, std::string> g_CountryPhoneMap;    //电话号码-国家区号对照表
	std::map<std::string, std::string> g_CityPhoneMap;    //电话号码-国内区号对照表
	std::map<std::string, std::string> g_CountryMap;    //国家地市表
	std::map<std::string, std::string> g_CityMap;    //城市地市表
	std::map<std::string, std::string> g_ProvinceMap;    //城市编号对应省份列表
	std::map<std::string, std::string> g_IpMap;     //号码段-IP地址表
	std::list<std::string> g_PreSeg;      //需要过滤的前缀号码段
	char strExePath[1024];    //当前应用程序所在目录
	CRITICAL_SECTION m_Sec;    //map锁

	//////////////////////////////////////////////////////////////////////////
	// refactor
public:
	bool IsCityTel(const std::string& telNumber, const std::string& cityCode);       //验证telNumber是否属于cityCode
	std::string FindCityCode(const std::string& telNumber);       //获取号码城市代码
private:
	bool IsIpPhone(const std::string& telNumber);
	std::string RemovePrefix(const std::string& telNumber);     //去除前缀(86,+86,086)

	std::mutex g_mtx;
};