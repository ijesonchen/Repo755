#include "stdafx.h"
#include "TestMain.h"

using namespace tinyxml2;

#include <iostream>
using namespace std;

void TestTransUtf(void)
{
	char* p = "源文件";
	wchar_t* pw = L"源文件";
}

void CopyXmlNode(void);

// 用代码创建xml
void CreateXml(void);


int Tinyxml2TestMain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	CreateXml();
	return 0;
	TestTransUtf();
	//////////////////////////////////////////////////////////////////////////
	// parameter
 	CString strVcprojFile(_T("utf8test.xml"));

	PrintXml(strVcprojFile);

	//////////////////////////////////////////////////////////////////////////
	// create element

	return 0;
}
/*
enum TiEncoding
{
	TiEncUnknown = 0,
	TiEncUTF8,
	TiEncGB2312,
};

bool IsEncoding(const char* pEncString, const char* pEncStd)
{
	if (-1 != SubIndex(pEncString, pEncStd, 
						StringLength1G(pEncString), StringLength1G(pEncStd), 
						0))
	{
		return true;
	}

	return false;
}

TiEncoding ParseEncoding(const char* pEncString)
{
	if (IsEncoding(pEncString, "utf-8") ||
		IsEncoding(pEncString, "UTF-8"))
	{
		return TiEncUTF8;
	}
	else if (IsEncoding(pEncString, "gb2312") ||
			IsEncoding(pEncString, "GB2312"))
	{
		return TiEncGB2312;
	}
	return TiEncUnknown;
}

TiEncoding ParseDeclare(const char* pDecl)
{
	TiEncoding code = TiEncUnknown;
	int nDeclLen = StringLength1G(pDecl);
	if (nDeclLen <= 0)
	{
		return code;
	}
	// xml version="1.0" encoding="UTF-8"
	// check header
	int nPos = 0;
	nPos = SubIndex(pDecl, "xml ", nDeclLen, 4, 0);
	if (nPos)
	{
		return code;
	}
	char* pszEncDecl = "encoding=\"";
	int nEncDecl = StringLength1G(pszEncDecl);
	nPos = SubIndex(pDecl, pszEncDecl, nDeclLen, nEncDecl, 0);

	return ParseEncoding(pDecl + nPos + nEncDecl);
}
*/
void PrintXml(CString strFileName)
{
	TiDocument xmlDoc;
	TiError tiRet = xmlDoc.LoadFile(strFileName.GetString());
	if (tiRet)
	{
		return;
	}

	TiEncoding code = GetDocEncoding(xmlDoc);
	


	TiPrinter prnt;
	xmlDoc.Print(&prnt);
	const char* psz = prnt.CStr();
	int nbyte = prnt.CStrSize();

	bool bBom = xmlDoc.HasBOM();

	TiElement* pRoot = xmlDoc.RootElement();
	PrintAllText(pRoot);
//
	return;
	TiElement* peCfg;
	peCfg = pRoot->FirstChildElement("Configurations")->FirstChildElement("Configuration");
	
	while (peCfg)
	{
		PrintElement(peCfg);
		const TiElement* peLinker = NULL;
		peLinker = peCfg->FirstChildElement("Tool");
		while (peLinker)
		{
			const TiAttribute* peAttr = peLinker->FindAttribute("Name");
			if (!strcmp(peAttr->Value(), "VCLinkerTool"))
			{
				continue;
			}
			PrintElement(peLinker);
			peLinker = peLinker->NextSiblingElement("Tool");
		}

		peCfg = peCfg->NextSiblingElement();
	}
}


void PrintAllText(const tinyxml2::TiElement* pe)
{
	if (!pe)
	{
		return;
	}
	PrintElement(pe);
	const TiElement* peChild = pe->FirstChildElement();
	while (peChild)
	{
		PrintAllText(peChild);
		peChild = peChild->NextSiblingElement();
	}
}


void PrintElement(const tinyxml2::TiElement* pe)
{
	if (!pe)
	{
		return;
	}
	const char* psz = NULL;
	psz = pe->Name();
	if (psz)
	{
		cout<<pe->Name();
	}
	{
		const TiAttribute* pa = pe->FirstAttribute();
		bool bAttr = false;
		if (pa)
		{
			cout<<endl;
		}
		while (pa)
		{
			cout<<"\t";
			psz = pa->Name();
			if (psz)
			{
				cout<<psz;
			}
			cout<<" = ";
			psz = pa->Value();
			if (psz)
			{
				cout<<psz;
			}
			cout<<endl;
			pa = pa->Next();
		}
	}

	psz = pe->GetText();
	if (psz)
	{
		cout<<": "<<psz;
	}
	else
	{
		cout<<"/";
	}
	cout<<endl;
}

char* GetText(TiDocument* pe)
{
	tinyxml2::TiPrinter* pprinter = new tinyxml2::TiPrinter();;
	pe->Accept(pprinter);
	const char* szText = pprinter->CStr();


	char* psz = new char[strlen(szText) + 1];
	memcpy(psz, szText, strlen(szText) + 1);

	delete pprinter;
	return psz;
}

char* GetText(TiElement* pe)
{
	tinyxml2::TiPrinter* pprinter = new tinyxml2::TiPrinter();;
	pe->Accept(pprinter);
	const char* szText = pprinter->CStr();

	char* psz = new char[strlen(szText) + 1];
	memcpy(psz, szText, strlen(szText) + 1);

	delete pprinter;
	return psz;
}


void CopyXmlNode(void)
{	
	tinyxml2::RaiiPtr gc;
	char* szText = NULL;

	char* szXmlName = "D:\\Tinyxml2Test.vcproj";
	tinyxml2::TiDocument doc;
	TiError tiRet = doc.LoadFile(szXmlName);

	char* szXmlPath = "/VisualStudioProject/Configurations/Configuration[Name=Debug|Win32]/";

	TiElement* pe = doc.ElementByPath(szXmlPath);
//	gc.Add(szText = GetText(pe));


	TiElement* pe2 = doc.ElementByPath("/VisualStudioProject/Configurations/");
	TiElement* pe3 = pe2->CopyAndAppendNode(pe);

	gc.Add(szText = GetText(pe2));

	gc.Add(szText = GetText(pe3));
	
	/*
	1. 获取doc1的节点pd1的p1text
	2. 使用p1text初始化doc2
	3. 获取doc2的根节点peroot
	4. 插入peroot到doc1中：错误：无法插入非本文档节点

	//////////////////////////////////////////////////////////////////////////
	// 代码


	char* szXmlName = "D:\\Tinyxml2Test.vcproj";
	tinyxml2::TiDocument doc;
	TiError tiRet = doc.LoadFile(szXmlName);

	char* szXmlPath = "/VisualStudioProject/Configurations/Configuration[Name=Debug|Win32]/";

	TiElement* pe = doc.ElementByPath(szXmlPath);
	PrintAllText(pe);


	const char* szText = NULL;
	
	szText = GetText(pe);;

	TiDocument doc2;
	tiRet = doc2.Parse(szText);
	TiElement* pe2 = doc2.RootElement();

	szText = GetText(pe2);

	TiElement* pe3 = doc.ElementByPath("/VisualStudioProject/Configurations");
	pe3->InsertAfterChild(pe, pe2);

	szText = GetText(pe3);


	*/

	/*
	const char* szText = NULL;

	char* szXmlName = "D:\\Tinyxml2Test.vcproj";
	tinyxml2::TiDocument doc;
	TiError tiRet = doc.LoadFile(szXmlName);

	char* szXmlPath = "/VisualStudioProject/Configurations/Configuration[Name=Debug|Win32]/";

	TiElement* pe = doc.ElementByPath(szXmlPath);
	szText = GetText(pe);

	char* psz = new char[strlen(szText) + 1];
	memcpy(psz, szText, strlen(szText) + 1);


	TiElement* pe4 = doc.ParseElement(psz);


// 	TiElement* pe4 = (TiElement*)ele->FirstChild();
	szText = GetText(pe4);
	szText = GetText(&doc);




// 	TiDocument doc3;
// 	doc3.Parse(psz);
// 
// 	doc3.ParseDeep(psz, 0);
// 
// 	szText = GetText(&doc3);
// 
// 	pe = doc3.RootElement();
// 
// 	szText = GetText(pe);
//
// 
// 	szText = GetText(ele);

	TiElement* pe3 = doc.ElementByPath("/VisualStudioProject/Configurations");
	TiElement* pe5 = doc.ElementByPath("/VisualStudioProject/Configurations/Configuration[Name=Debug|Win32]/");
	pe3->InsertAfterChild(pe5, pe4);
	szText = GetText(pe3);

*/


}


void CreateXml(void)
{
	
	/*
	<root>
		<ele1/>
		<ele3/>
	</root>
	*/
	TiDocument doc;
	TiElement* pe = doc.NewElement("root");
	doc.InsertEndChild(pe);

	TiElement* pe11 =  doc.NewElement("ele1");
	pe->InsertEndChild(pe11);

	TiElement* pe12 =  doc.NewElement("ele2");
	pe->InsertEndChild(pe12);

	TiElement* pe13 =  doc.NewElement("ele3");
	pe->InsertEndChild(pe13);

	pe->DeleteChild(pe12);
}


/************************************************************************/
/*                         common functions                             */
/************************************************************************/


/************************************************************************/
/* 
主串T的pos开始，是否有和字串S匹配。
如果有，返回匹配开始位置
如果没有，返回-1
*/
/************************************************************************/
/*
int SubIndex(const char* pSrc, const char* pSub, int nSrcLength, int nSubLength, int nStartPos)
{
	int i = nStartPos;
	int j = 0;
	while (i < nSrcLength && j < nSubLength)
	{
		if (pSrc[i] == pSub[j])
		{
			++i;
			++j;
		}
		else
		{
			i = i - j + 1;
			j = 0;
		}
	}
	return j == nSubLength ? 
				i - nSubLength :
				-1;
}

void KmpNext(const char* s, int *next, int slength)
{
	int i = -1, j=-2;
	next[0]=-1;
	while (i < slength-1)
	{
		if (j==-1 || s[i] == s[j])
		{
			++i;
			++j;
			next[i]=j;
		}
		else
		{
			j=next[j];
		}
	}
}

int KmpIndex(const char* T, const char* pSub, int tlength, int slength, int pos)
{
	int* next = new int[slength+1];
	KmpNext(pSub, next, slength);

	int j=-1, i=pos-1;
	while (i<tlength && j<slength)
	{
		if (j==-1 || T[i]==pSub[j])
		{
			++i;
			++j;
		}
		else
		{
			j=next[j];
		}
	}

	delete next;
	return j==slength?i-slength:-1;
}

const int MAX_STRING_LENGTH = 0x40000000;
int StringLength1G(const char* psz)
{
	int nLength = 0;
	while (psz[nLength++])
	{
		if (nLength >= MAX_STRING_LENGTH)
		{
			return -1;
		}
	}
	return --nLength;
}

int StringLength1G(const wchar_t* psz)
{
	int nLength = 0;
	while (psz[nLength++])
	{
		if (nLength >= MAX_STRING_LENGTH)
		{
			return -1;
		}
	}
	return --nLength;
}
*/
/************************************************************************/
/* 
UNICODE <-> UTF-8
UNICODE hex			bit	UTF-8 bin presentation
00000000 0000007F	 7	0xxxxxxx
00000080 000007FF	11  110xxxxx 10xxxxxx
00000800 0000FFFF	16	1110xxxx 10xxxxxx 10xxxxxx

UCS-4
00010000 001FFFFF	21	11110xxx 10xxxxxx 10xxxxxx 10xxxxxx 
00200000 03FFFFFF	26	111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
04000000 7FFFFFFF	31	1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
*/
/************************************************************************/

/*
int UnicodeToUtf8(const wchar_t* pUnicode, int nUnicodeBufLen, char* pUtf8, int nUtf8Len)
{
	if (!pUnicode)
	{
		return -1;
	}

	int nTotal = 0;
	for (int ii = 0; ii < nUnicodeBufLen; ++ii)
	{
		wchar_t wch = pUnicode[ii];
		if (wch <= 0x007F)
		{
			nTotal += 1;
		}
		else if (wch <= 0x07FF)
		{
			nTotal += 2;
		}
		else if (wch <= 0xFFFF)
		{
			nTotal += 3;
		}
	}

	if (!pUtf8)
	{
		return nTotal;
	}
	*pUtf8 = NULL;
	if (nUtf8Len < nTotal)
	{
		return -2;
	}

	// utf8 write index
	int nwUtf8 = 0;
	for (int ii = 0; ii < nUnicodeBufLen; ++ii)
	{
		wchar_t wch = pUnicode[ii];
		if (wch <= 0x007F)
		{
			if (nwUtf8 >= nUtf8Len - 1)
			{
				return -3;
			}
			// 0xxxxxxx
			pUtf8[nwUtf8++] = (char)wch; 
		}
		else if (wch <= 0x07FF)
		{
			if (nwUtf8 >= nUtf8Len - 2)
			{
				return -3;
			}
			// 110xxxxx 10xxxxxx
			pUtf8[nwUtf8++] = (char)((wch >> 6) | 0xC0); 
			pUtf8[nwUtf8++] = (char)((wch & 0x3F) | 0x80); 
		}
		else if (wch <= 0xFFFF)
		{
			if (nwUtf8 >= nUtf8Len - 3)
			{
				return -3;
			}
			// 1110xxxx 10xxxxxx 10xxxxxx
			pUtf8[nwUtf8++] = (char)((wch >> 12) | 0xE0); 
			pUtf8[nwUtf8++] = (char)(((wch >> 6) & 0x3F) | 0x80); 
			pUtf8[nwUtf8++] = (char)((wch & 0x3F) | 0x80); 
		}
	}

	return nwUtf8;
}


int UnicodeToUtf8(const wchar_t* pszUnicode, char* pUtf8, int nUtf8Len)
{
	int nInputLen = StringLength1G(pszUnicode) + 1;
	if (nInputLen <= 0)
	{
		return -1;
	}
	return UnicodeToUtf8(pszUnicode, nInputLen, pUtf8, nUtf8Len);
}

int Utf8CharCount(const char* pUtf8)
{
	if (!pUtf8)
	{
		return -1;
	}
	int nCharCount = 0;
	char ch = *pUtf8;
	while (ch & 0x80)
	{
		++nCharCount;
		ch <<= 1;
	}

	if (nCharCount == 0)
	{
		// <= 0x007F
		return 1;
	}
	else if (nCharCount > 3)
	{
		// not UCS2
		return -2;
	}

	for (int ii = 0; ii < nCharCount - 1; ++ii)
	{
		char CH = pUtf8[1 + ii];
		if ((CH & 0xC0) ^ 0x80)
		{
			// followed not 10xxxxxx
			return -3;
		}
	}

	return nCharCount;
}

int Unf8ToWchar(const char* pUtf8 , wchar_t& wch)
{
	int nUtf8Count = Utf8CharCount(pUtf8);
	wch = 0;
	switch (nUtf8Count)
	{
	case 1:
		wch = *pUtf8;
		break;
	case 2:
		wch |= ((pUtf8[0] & 0x1F) << 6);
		wch |= (pUtf8[1] & 0x3F);
		break;
	case 3:
		wch |= ((pUtf8[0] & 0x0F) << 12);
		wch |= ((pUtf8[1] & 0x3F) << 6);
		wch |= (pUtf8[2] & 0x3F);
		break;
	default:
		wch = -1;
		nUtf8Count -= 100;
		ASSERT(false);
		break;
	}

	return nUtf8Count;
}

int Utf8ToUnicode(const char* pUtf8, int nUtf8Len,  wchar_t* pUnicode, int nUnicodeBufLen)
{
	if (!pUtf8)
	{
		// parameter failed.
		return -1;
	}

	int nTotalUtf = 0;
	int nTotalUni = 0;
	for (nTotalUtf = 0; nTotalUtf < nUtf8Len;)
	{
		int nTemp = Utf8CharCount(pUtf8 + nTotalUtf);
		if (nTemp < 0)
		{
			// utf8 check failed.
			return -2;
		}
		nTotalUtf += nTemp;
		++nTotalUni;
	}
	if (!pUnicode)
	{
		return nTotalUni;
	}
	*pUnicode = NULL;
	if (nUnicodeBufLen < nTotalUni)
	{
		return -1;
	}

	int nIdxUtf = 0;
	for (int ii = 0; ii < nTotalUni; ++ii)
	{
		if (nIdxUtf >= nUtf8Len)
		{
			return -2;
		}
		int nUtfCount = Unf8ToWchar(pUtf8 + nIdxUtf, pUnicode[ii]);
		if (nUtfCount <= 0)
		{
			return -3;
		}
		nIdxUtf += nUtfCount;
	}
	return nTotalUni;
}


int Utf8ToUnicode(const char* pszUtf8,  wchar_t* pUnicode, int nUnicodeBufLen)
{
	int nInputLen = StringLength1G(pszUtf8) + 1;
	if (nInputLen <= 0)
	{
		return -1;
	}
	return Utf8ToUnicode(pszUtf8, nInputLen, pUnicode, nUnicodeBufLen);
}
*/