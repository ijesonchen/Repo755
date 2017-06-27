#include "stdafx.h"
#include "PhoneSegParse.h"
#include <sstream>
#include <deque>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;


CPhoneSegParse::~CPhoneSegParse()//析构函数
{
	DeleteCriticalSection(&m_Sec);
}
CPhoneSegParse::CPhoneSegParse()
{
	InitializeCriticalSection(&m_Sec);
	InitCity();
	InitCityPhoneMap();
	InitCountryPhoneMap();
	InitPhoneIpMap();
	InitPhonePrefix();
	InitProvince();
}
CPhoneSegParse::CPhoneSegParse(char * strPath)//初始化函数
{
	InitializeCriticalSection(&m_Sec);
	strcpy(strExePath,strPath);
	InitCity();
	InitProvince();
	InitCityPhoneMap();
	InitCountryPhoneMap();
	InitPhoneIpMap();
	InitPhonePrefix();
}

void CPhoneSegParse::InitAll(char* strPath)
{
	InitializeCriticalSection(&m_Sec);
	strcpy(strExePath,strPath);
	InitCity();
	InitProvince();
	InitCityPhoneMap();
	InitCountryPhoneMap();
	InitPhoneIpMap();
	InitPhonePrefix();
}

void CPhoneSegParse::DelPrefix(char* PrePhoneNo,char* LstPhone)
{
	int len = -1;CStringA stTmpPhone;
	stTmpPhone = PrePhoneNo;
	for (list<string>::iterator itor = g_PreSeg.begin();itor != g_PreSeg.end();itor++)
	{
		string stPreNo = *itor;
		len = stPreNo.length();
		int TotalLen = stTmpPhone.GetLength();
		if (strncmp(stPreNo.c_str(),PrePhoneNo,len) == 0)//如果找到该前缀
		{
			stTmpPhone = PrePhoneNo;
			stTmpPhone=stTmpPhone.Right(TotalLen-len);
			break;
		}
	}
	strcpy(LstPhone,stTmpPhone.GetBuffer(0));
	
}
bool CPhoneSegParse::FindIpPhoneMap(char* PhoneNo,char* CityName)
{
	bool bret = false;string stCityName,stPhoneNo;

	CStringA nTmpCallNo,CallNumber;
	CallNumber = PhoneNo;
	for(int i =7;(i >= 3) && (bret != true);i--)
	{
		nTmpCallNo  = CallNumber.Left(i);
		if (g_IpMap.find(nTmpCallNo.GetBuffer(0)) != g_IpMap.end())
		{
			bret = true;
			strcpy(CityName,g_IpMap[nTmpCallNo.GetBuffer(0)].c_str());
		}
	}
	return bret;
}
bool CPhoneSegParse::FindCountry(char* PhoneNo,char* CountryId,char* CoutnryName)
{
	bool bRet = false;    //寻找国家编号，编码
	CStringA nTmpCallNo,CallNumber;
	CallNumber = PhoneNo;
	for(int i =7;(i >= 3) && (bRet != true);i--)
	{
		nTmpCallNo  = CallNumber.Left(i);
		if (g_CountryPhoneMap.find(nTmpCallNo.GetBuffer(0)) != g_CountryPhoneMap.end())
		{
			bRet = true;
			strcpy(CountryId,nTmpCallNo.GetBuffer(0));
			strcpy(CoutnryName,g_CountryPhoneMap[nTmpCallNo.GetBuffer(0)].c_str());
		}
	}

	return bRet;
}
bool CPhoneSegParse::FindLocalCity(char* PhoneNo,char* CityId,char* CityName)
{
	bool bRet = false;

	CStringA nTmpCallNo,CallNumber;
	CallNumber = PhoneNo;
	for(int i =8;(i >= 6) && (bRet != true);i--)
	{
		nTmpCallNo  = CallNumber.Left(i);
		if (g_CityPhoneMap.find(nTmpCallNo.GetBuffer(0)) != g_CityPhoneMap.end())
		{
			bRet = true;
			strcpy(CityId,g_CityPhoneMap[nTmpCallNo.GetBuffer(0)].c_str());
			if(g_CityMap.find(CityId) != g_CityMap.end())
			{
				strcpy(CityName,g_CityMap[CityId].c_str());
			}
		}
	}

	if (bRet == false)
	{
		for(int i =7;(i >= 2) && (bRet != true);i--)
		{
			nTmpCallNo  = CallNumber.Left(i);
			if (g_CityMap.find(nTmpCallNo.GetBuffer(0)) != g_CityMap.end())
			{
				bRet = true;
				strcpy(CityName,g_CityMap[nTmpCallNo.GetBuffer(0)].c_str());

				strcpy(CityId,nTmpCallNo.GetBuffer(0));
			}
		}
	}
	return bRet;
}
void CPhoneSegParse::InitPhonePrefix()
{
	CStringA strIpPhonePath,stLine;
	strIpPhonePath.Format("%s%s",strExePath,PhonePrefixPath);
	strIpPhonePath.Replace("/","\\");

	FILE *fp = fopen(strIpPhonePath,"rt");
	char szLine[MAX_PATH] = "\0"; char PhoneSeg[30] = "\0"; char CityName[30] = "\0";
	EnterCriticalSection(&m_Sec);
	if(fp != NULL)
	{
		while (true)
		{
			if (fgets(szLine,sizeof(szLine)-1,fp) == NULL)
			{
				fclose(fp);
				fp = NULL;
				break;
			}
			stLine.Format("%s",szLine);
			stLine.TrimRight('\n');
			stLine.TrimRight('\r');
			int len = strlen(stLine.GetBuffer(0));
			g_PreSeg.push_back(stLine.GetBuffer(0));
		}
	}
	LeaveCriticalSection(&m_Sec);
}
void CPhoneSegParse::InitPhoneIpMap()
{
	CStringA strIpPhonePath;
	strIpPhonePath.Format("%s%s",strExePath,IpPhoneSegPath);
	strIpPhonePath.Replace("/","\\");

	FILE *fp = fopen(strIpPhonePath,"rt");
	char szLine[MAX_PATH] = "\0"; char PhoneSeg[30] = "\0"; char CityName[30] = "\0";
	EnterCriticalSection(&m_Sec);
	if(fp != NULL)
	{
		while (true)
		{
			if (fgets(szLine,sizeof(szLine)-1,fp) == NULL)
			{
				fclose(fp);
				fp = NULL;
				break;
			}
			if (szLine[strlen(szLine)-1] == '\n') szLine[strlen(szLine)-1] = '\0';
			{
				DownLoad(szLine,PhoneSeg,CityName);   //读取所有的城市ID-城市名字入map表
			}
			g_IpMap.insert(map<string,string>::value_type(PhoneSeg,CityName));
		}
	}
	LeaveCriticalSection(&m_Sec);
}
bool CPhoneSegParse::FindCountryName(char* PhoneNo,char* CountryId,char* CountryName) //查找该号码属于国家还是地市
{
	char stTrimCall[MAX_PATH] = "\0";bool bRet = false;bool IsIpPhone,bRes;
 //首先判断是否IP电话
	IsIpPhone = FindIpPhoneMap(PhoneNo,CountryName);
	if (IsIpPhone != FALSE)//如果该电话为Ip电话
	{
		bRet = true;
		return bRet;
	}
	DelPrefix(PhoneNo,stTrimCall);//首先去除前缀例如86，+86,086

	EnterCriticalSection(&m_Sec);

	bRes = FindCountry(stTrimCall,CountryId,CountryName);   //查找所属的国家
	
	LeaveCriticalSection(&m_Sec);
	return bRes;

}

bool CPhoneSegParse::FindCityName(char* PhoneNo,char* CityId,char* CityName, char* ProvinceName) //查找该号码属于国家还是地市
{
	char stTrimCall[MAX_PATH] = "\0";bool bRet = false;bool IsIpPhone,bRes;
	//首先判断是否IP电话
	IsIpPhone = FindIpPhoneMap(PhoneNo,CityName);
	if (IsIpPhone != FALSE)//如果该电话为Ip电话
	{
		bRet = true;
		return bRet;
	}
	DelPrefix(PhoneNo,stTrimCall);//首先去除前缀例如86，+86,086

	EnterCriticalSection(&m_Sec);

	bRes = FindLocalCity(stTrimCall,CityId,CityName);     //查找所属的地市

	if (bRes == true) //如果城市编号查找成功，则查找对应省市
	{
		if (g_ProvinceMap.find(CityId) != g_ProvinceMap.end())
		{
			strcpy(ProvinceName,g_ProvinceMap[CityId].c_str());
		}
	}
	LeaveCriticalSection(&m_Sec);
	return bRes;

}


void CPhoneSegParse::InitCountryPhoneMap()
{
	CStringA strPhoneSegCountryPath;
	strPhoneSegCountryPath.Format("%s%s",strExePath,PhoneSegCountryPath);
	strPhoneSegCountryPath.Replace("/","\\");

	FILE *fp = fopen(strPhoneSegCountryPath,"rt");
	char szLine[MAX_PATH] = "\0"; char PhoneSeg[30] = "\0"; char CountryId[30] = "\0";
	EnterCriticalSection(&m_Sec);
	if(fp != NULL)
	{
		while (true)
		{
			if (fgets(szLine,sizeof(szLine)-1,fp) == NULL)
			{
				fclose(fp);
				fp = NULL;
				break;
			}
			if (szLine[strlen(szLine)-1] == '\n') szLine[strlen(szLine)-1] = '\0';
			{
				DownLoad(szLine,PhoneSeg,CountryId);   //读取所有的城市ID-城市名字入map表
			}
			g_CountryPhoneMap.insert(map<string,string>::value_type(PhoneSeg,CountryId));
		}
	}
	LeaveCriticalSection(&m_Sec);
}

void CPhoneSegParse::InitCityPhoneMap()
{
	CStringA strPhoneSegCityPath;
	strPhoneSegCityPath.Format("%s%s",strExePath,PhoneSegCityPath);
	strPhoneSegCityPath.Replace("/","\\");

	FILE *fp = fopen(strPhoneSegCityPath,"rt");
	char szLine[MAX_PATH] = "\0"; char PhoneSeg[30] = "\0"; char CityId[30] = "\0";
	EnterCriticalSection(&m_Sec);
	if(fp != NULL)
	{
		while (true)
		{
			if (fgets(szLine,sizeof(szLine)-1,fp) == NULL)
			{
				fclose(fp);
				fp = NULL;
				break;
			}
			if (szLine[strlen(szLine)-1] == '\n') szLine[strlen(szLine)-1] = '\0';
			{
				DownLoad(szLine,PhoneSeg,CityId);   //读取所有的城市ID-城市名字入map表
			}
			g_CityPhoneMap.insert(map<string,string>::value_type(PhoneSeg,CityId));
		}
	}
	LeaveCriticalSection(&m_Sec);
}

void CPhoneSegParse::InitProvince()
{
	CStringA strProvincePath;
	strProvincePath.Format("%s%s",strExePath,ProvincePath);
	strProvincePath.Replace("/","\\");
	FILE *fp=fopen(strProvincePath,"rt");//城市名称ID-城市名称对应表
	char szLine[1024] = "\0";char CityId[MAX_PATH] = "\0";char ProvinceName[MAX_PATH] = "\0";

	EnterCriticalSection(&m_Sec);
	if(fp != NULL)
	{
		while (true)
		{
			memset(szLine,0,1024);
			if (fgets(szLine,sizeof(szLine)-1,fp) == NULL)
			{
				fclose(fp);
				fp = NULL;
				break;
			}
			if (szLine[strlen(szLine)-1] == '\n') szLine[strlen(szLine)-1] = '\0';
			{
				DownLoad(szLine,CityId,ProvinceName);   //读取所有的城市ID-城市名字入map表
			}
			g_ProvinceMap.insert(map<string,string>::value_type(CityId,ProvinceName));
		}
	}
	LeaveCriticalSection(&m_Sec);
}

void CPhoneSegParse::InitCity()
{
	CStringA strCityPath;
	strCityPath.Format("%s%s",strExePath,CityPath);
	strCityPath.Replace("/","\\");
	FILE *fp=fopen(strCityPath,"rt");//城市名称ID-城市名称对应表
	char szLine[1024] = "\0";char CityId[MAX_PATH] = "\0";char CityName[MAX_PATH] = "\0";
	
	EnterCriticalSection(&m_Sec);
	if(fp != NULL)
	{
		while (true)
		{
			memset(szLine,0,1024);
			if (fgets(szLine,sizeof(szLine)-1,fp) == NULL)
			{
					fclose(fp);
					fp = NULL;
					break;
			}
			if (szLine[strlen(szLine)-1] == '\n') szLine[strlen(szLine)-1] = '\0';
			{
				DownLoad(szLine,CityId,CityName);   //读取所有的城市ID-城市名字入map表
			}
			g_CityMap.insert(map<string,string>::value_type(CityId,CityName));
		}
	}
	LeaveCriticalSection(&m_Sec);
}
bool CPhoneSegParse::DownLoad(char *str,char * seg1,char * seg2)
{
	*seg1 = 0;
	*seg2 = 0;
	string input(str);
	for (auto& ch : input)
	{
		if (ch == '|')
		{
			ch = ' ';
		}
	}
	istringstream iss(input);
	deque<string> dq;
	string tmp;
	while (iss >> tmp)
	{
		dq.push_back(tmp);
	}
	if (dq.size() == 2)
	{
		// may over-flow
		strcpy(seg1, dq[0].c_str());
		strcpy(seg2, dq[1].c_str());
		return true;
	}
	return false;

	int nTokenNo = 0;
	char *szOriTokenBuf = (char*)malloc(strlen(str) + 256);
	if(szOriTokenBuf == NULL) return false;
	strcpy(szOriTokenBuf, str);

	char *szTokenBuf = szOriTokenBuf;

	if(szTokenBuf == NULL) return false;
	bool ret = false;
	while(1)
	{
		char *p = strstr(szTokenBuf, "|");
		if(p != NULL)
		{
			*p = '\0';
			p++;
			switch(nTokenNo)
			{
			case 0:
				strcpy(seg1,szTokenBuf);
				break;
			case 1:
				strcpy(seg2,szTokenBuf);
				ret = true;
				break;
			}
			nTokenNo++;
		}
		else
		{
			break;
		}
		szTokenBuf = p;
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////
// refactor
std::string CPhoneSegParse::RemovePrefix(const std::string& telNumber)     //去除前缀(86,+86,086)
{
	for (auto& strPre : g_PreSeg)
	{
		auto len = strPre.length();
		if (strncmp(strPre.c_str(), telNumber.c_str(), len) == 0)//如果找到该前缀
		{
			return telNumber.substr(len);
		}
	}
	return telNumber;
}

bool CPhoneSegParse::IsIpPhone(const std::string& telNumber)
{
	bool bret = false;
	for (int i = 7; (i >= 3) && (bret != true); i--)
	{
		auto tmpTel = telNumber.substr(0, i);
		if (g_IpMap.find(tmpTel) != g_IpMap.end())
		{
			return true;
		}
	}
	return false;
}

std::string CPhoneSegParse::FindCityCode(const std::string& telNumber)       //获取号码城市代码
{
	string nullCode;
	if (IsIpPhone(telNumber))
	{
		return nullCode;
	}
	auto trimTel = RemovePrefix(telNumber);//首先去除前缀例如86，+86,086

	std::unique_lock < std::mutex > lock(g_mtx);

	bool bRet = false;
	for (int i = 8; (i >= 6) && (bRet != true); i--)
	{
		auto tmpTel = trimTel.substr(0, i);
		auto citycode = g_CityPhoneMap.find(tmpTel);
		if (citycode != g_CityPhoneMap.end())
		{
			return citycode->second;
		}
	}
	return nullCode;
}

//验证telNumber是否属于cityCode
bool CPhoneSegParse::IsCityTel(const std::string& telNumber, const std::string& cityCode)
{
	return cityCode == FindCityCode(telNumber);
}