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
	CPhoneSegParse(char* strPath);//��ʼ������
	CPhoneSegParse();//�ṹ
	void InitAll(char* strPath);//��ʼˢ��������
	bool FindCountryName(char* PhoneNo,char* CountryId,char* CountryName);       //���Ҹ��ֻ��������ĸ����һ����
	bool FindCityName(char* PhoneNo, char* CityId, char* CityName, char* ProvinceName);       //���Ҹ��ֻ��������ĸ�����---ʡ��
	~CPhoneSegParse();    //CPhoneSeg ����

private:

	void InitCity();       //��������ID-�������ƶ�Ӧ��
	void InitProvince();    //���б�Ŷ�Ӧʡ������
	void InitCountryPhoneMap();     //�����-���ұ�Ŷ�Ӧ��
	void InitCityPhoneMap();        //�����-���б�Ŷ�Ӧ��
	void InitPhoneIpMap();      //�����-IP��Ӧ��
	void InitPhonePrefix();      //�����ǰ׺
	void DelPrefix(char* PrePhoneNo,char* LstPhone);     //ȥ��ǰ׺(86,+86,086)

	bool FindIpPhoneMap(char* CallNo,char* CallArea);       //�����-IP��ַ��Ӧ��
	bool FindLocalCity(char* PhoneNo,char* CityId,char* CityName);    //���ҵ��б�ż�����
	bool FindCountry(char* PhoneNo,char* CountryId,char* CountryName);     //���ҹ��ұ�ż�����
	bool DownLoad(char *str,char * seg1,char * seg2);    //�����ַ����б�
		

private:
	std::map<std::string, std::string> g_CountryPhoneMap;    //�绰����-�������Ŷ��ձ�
	std::map<std::string, std::string> g_CityPhoneMap;    //�绰����-�������Ŷ��ձ�
	std::map<std::string, std::string> g_CountryMap;    //���ҵ��б�
	std::map<std::string, std::string> g_CityMap;    //���е��б�
	std::map<std::string, std::string> g_ProvinceMap;    //���б�Ŷ�Ӧʡ���б�
	std::map<std::string, std::string> g_IpMap;     //�����-IP��ַ��
	std::list<std::string> g_PreSeg;      //��Ҫ���˵�ǰ׺�����
	char strExePath[1024];    //��ǰӦ�ó�������Ŀ¼
	CRITICAL_SECTION m_Sec;    //map��

	//////////////////////////////////////////////////////////////////////////
	// refactor
public:
	bool IsCityTel(const std::string& telNumber, const std::string& cityCode);       //��֤telNumber�Ƿ�����cityCode
	std::string FindCityCode(const std::string& telNumber);       //��ȡ������д���
private:
	bool IsIpPhone(const std::string& telNumber);
	std::string RemovePrefix(const std::string& telNumber);     //ȥ��ǰ׺(86,+86,086)

	std::mutex g_mtx;
};