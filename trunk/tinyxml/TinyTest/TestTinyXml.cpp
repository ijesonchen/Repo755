// TinyTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TinyTest.h"
#include "..\TinyXml\tinyxml.h"

#include <vector>

/************************************************************************/
/*                            common functions                          */
/************************************************************************/

int UnicodeToUTF8(CStringW strUnicode, BYTE *lpUtf8Buf, int nUTF8BufLen)
{
	int nRetLen = ::WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, NULL, 0, NULL, NULL);  //��ȡת����UTF8���������Ҫ���ַ��ռ䳤�� 
	if(NULL == lpUtf8Buf)  //���������Ϊ���򷵻�ת������Ҫ�Ŀռ��С 
	{ 
		return nRetLen; 
	}
	if(nUTF8BufLen < nRetLen)  //���������������Ȳ������˳� 
	{ 
		return 0; 
	} 
	nRetLen = ::WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, (char *)lpUtf8Buf, nUTF8BufLen, NULL, NULL);  //ת����UTF8���� 

	return nRetLen; 
}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Ψһ��Ӧ�ó������

// CWinApp theApp;

using namespace std;

//////////////////////////////////////////////////////////////////////////
// XML STRING NAME DEFINATION

// RD for result nodes
#define XML_VERSION					("1.0")
#define XML_ENCODING				("utf-8")
#define XML_STRING_ROOT				("RecResult")
#define XML_STRING_VocPath			("VocPath")
#define XML_STRING_RD_LangResult	("LangResult")
#define XML_STRING_LangName			("LangName")
#define XML_STRING_LangOutput		("LangOutput")
#define XML_STRING_RD_GenderResult	("GenderResult")
#define XML_STRING_GenderName		("Gender")
#define XML_STRING_GenderOutput		("GenderOutput")
#define XML_STRING_RD_KeywordResult	("KeywordResult")
#define XML_STRING_KeywordCount		("KeywordNum")
#define XML_STRING_RD_KwdItem		("KwdItems")
#define XML_STRING_KwdItem			("KwdItem")
#define XML_STRING_KwdText			("KwdText")
#define XML_STRING_KwdResultCount	("KwdResNum")
#define XML_STRING_RD_KwdResultItem	("KwdResItems")
#define XML_STRING_KwdResultItem	("KwdResItem")
#define XML_STRING_StartPos			("StartPos")
#define XML_STRING_EndPos			("EndPos")
#define XML_STRING_KwdOutput		("KwdOutput")
#define XML_STRING_RD_VprResult		("VprResult")
#define XML_STRING_VprResCount		("VprResNum")
#define XML_STRING_RD_VprResItem	("VprResItems")
#define XML_STRING_VprResItem		("VprResItem")
#define XML_STRING_VprName			("VprName")
#define XML_STRING_VprOutput		("VprOutput")

typedef unsigned int      u_32;
typedef unsigned short    u_16;
typedef unsigned char     u_8;

#define HTONL(x) (x = htonl(x))
#define HTONS(x) (x = htons(x))
#define NTOHL(x) (x = ntohl(x))
#define NTOHS(x) (x = ntohs(x))


#define FILENAME_SIZE    256
#define MSGID_SIZE       32
#define TEL_LENGTH       256
#define VOICEID_SIZE     37
#define V_VERSION		2
#define MSG_MAGIC	(0xFFFF)

#define MSG_VOICE_INFO_RESULT	 0x0014     // ������Ϣ������

typedef struct _PackHead
{
	u_16 w_nFlag;  //ͬ����־     
	u_16 w_nVer;	//Э��汾
	u_32 w_nMsgType;//��Ϣ����
	char w_szMsgID[MSGID_SIZE] ;   //��Ϣ���к� 
	u_32 w_nMsgLen;//��Ϣ��ĳ���
	void hton(void)
	{
		HTONS(w_nFlag);   
		HTONS(w_nVer);	
		HTONL(w_nMsgType); 
		HTONL(w_nMsgLen); 
	}
	void ntoh(void)
	{
		NTOHS(w_nFlag);   
		NTOHS(w_nVer);	
		NTOHL(w_nMsgType); 
		NTOHL(w_nMsgLen); 
	}
	bool IsValid();
}PackHead;

typedef struct _VoiceInfResult 
{
	PackHead	w_Head;
	u_32	w_nCallerType;					//Ŀ������(��������)	Int(32)	M	
	char	w_szCallerTelNumber[TEL_LENGTH];	//�����ʶ(�������) Char(32) M
	char	w_szCalledTelNumber[TEL_LENGTH];	//�Զ˺���	Char(32)	C
	char    w_szFileName[FILENAME_SIZE];	//�����ļ���	Char(128)	C
	u_32	w_nStartTime;                //ͨ����ʼʱ��	Int(32)	C	
	u_32	w_nEndTime;                  //ͨ������ʱ��	Int(32)	C	
	char	w_szRecogResult[32];				//ʶ������	Char(32)	M	��������ʶ����Ľ�������ʶ��ɹ���ʶ��ʧ��
	char	w_szVoiceID[VOICEID_SIZE];			//������ʾ	Char(128)	M
	u_32	w_nComposeType;					//�ϳɱ�ʶ	Int(32)	M	����·ȡ0������ȡ1��2��
	u_32	w_nXmlLength;						// XMLʶ��������
	void hton(void)
	{
		HTONL(w_nCallerType);
		HTONL(w_nStartTime);
		HTONL(w_nEndTime);
		HTONL(w_nComposeType);
		HTONL(w_nXmlLength);
	}
} VoiceInfResult;

struct SRecogResult 
{
	CStringA strLang;
	float fLangOutput;
	CStringA strGender;
	float fGenderOutput;
	CStringA strVocFullName;
	SRecogResult()
	{
		fLangOutput = 0;
		fGenderOutput = 0;
	}
};

struct KwdResultItem
{
	u_32	w_nRecogOutput;					//ʶ���������λ�������֮һ��
	u_32	w_nStartTime;					//��ʼλ��	Int32	M	ʱ�䵥λ����
	u_32	w_nEndTime;						//����λ��	Int32	M	ʱ�䵥λ����
	KwdResultItem()
	{
		w_nRecogOutput = 0;
		w_nStartTime = 0; 
		w_nEndTime = 0;
	}
	KwdResultItem(u_32 start, u_32 end, u_32 out)
	{
		w_nRecogOutput = out;
		w_nStartTime = start; 
		w_nEndTime = end;
	}
};

struct KwdResult
{
	CStringA	w_szKeyword;		//�ؼ���	Char[64]	M
	std::vector<KwdResultItem> w_vtResultItems;
};

struct VprResultItem 
{
	CStringA	w_szObjID;    // �������	Char[128]	M
	float	w_nRecogOutput;              // ʶ���������λ�������֮һ�� 
	u_32 	w_nIsHit;                 // �Ƿ��б꣬0��ʾ�б꣬������ʾ���бꡣ
	VprResultItem()
	{
		w_nRecogOutput = 0;
		w_nIsHit = 0;
	}
	VprResultItem(char* szObjID, float out)
	{
		w_szObjID = szObjID;
		w_nRecogOutput = out;
		w_nIsHit = 0;
	}
};

void XmlAddKwdResults( TiXmlElement* pRoot, vector<KwdResult> &vtKwdResults ) 
{
	TiXmlElement* pKeywordResult = pRoot->AddNewElem(XML_STRING_RD_KeywordResult);
	pKeywordResult->AddNewChild(XML_STRING_KeywordCount, vtKwdResults.size());
	TiXmlElement* pKeywordItems = pKeywordResult->AddNewElem(XML_STRING_RD_KwdItem);

	for (unsigned ii = 0, iLoop = vtKwdResults.size(); ii < iLoop; ii++)
	{
		TiXmlElement* pKeywordItem = pKeywordItems->AddNewElem(XML_STRING_KwdItem);
		pKeywordItem->AddNewChild(XML_STRING_KwdText, vtKwdResults[ii].w_szKeyword);
		pKeywordItem->AddNewChild(XML_STRING_KwdResultCount, vtKwdResults[ii].w_vtResultItems.size());
		TiXmlElement* pKwdResultItems = pKeywordItem->AddNewElem(XML_STRING_RD_KwdResultItem);
		for (unsigned jj = 0, jLoop = vtKwdResults[ii].w_vtResultItems.size();
			jj < jLoop; jj++)
		{
			TiXmlElement* pKwdResultItem = pKwdResultItems->AddNewElem(XML_STRING_KwdResultItem);
			pKwdResultItem->AddNewChild(XML_STRING_StartPos, vtKwdResults[ii].w_vtResultItems[jj].w_nStartTime);
			pKwdResultItem->AddNewChild(XML_STRING_EndPos, vtKwdResults[ii].w_vtResultItems[jj].w_nEndTime);
			pKwdResultItem->AddNewChild(XML_STRING_KwdOutput, vtKwdResults[ii].w_vtResultItems[jj].w_nRecogOutput);
		}			
	}
}

void XmlAddVprResults( TiXmlElement* pRoot, vector<VprResultItem> &vtVprResults ) 
{

	TiXmlElement* pVprResult = pRoot->AddNewElem(XML_STRING_RD_VprResult);
	pVprResult->AddNewChild(XML_STRING_VprResCount, vtVprResults.size());
	TiXmlElement* pVprResultItems = pVprResult->AddNewElem(XML_STRING_RD_VprResItem);
	for (unsigned ii = 0, iLoop = vtVprResults.size();
		ii < iLoop; ii++)
	{
		TiXmlElement* pVprResultItem = pVprResultItems->AddNewElem(XML_STRING_VprResItem);
		pVprResultItem->AddNewChild(XML_STRING_VprName, vtVprResults[ii].w_szObjID);
		pVprResultItem->AddNewChild(XML_STRING_VprOutput, vtVprResults[ii].w_nRecogOutput);
	}
}

unsigned char* GetMsgVoiceInfResult(VoiceInfResult& infResult, 
														 SRecogResult& recogResult, 
														 vector<KwdResult>& vtKwdResults, 
														 vector<VprResultItem>& vtVprResults)
{
	// generate XML string
	TiXmlDocument xmlDoc;
	xmlDoc.AddNewDeclaration(XML_VERSION, XML_ENCODING);
	TiXmlElement* pRoot = xmlDoc.AddRoot(XML_STRING_ROOT);

	pRoot->AddNewChild(XML_STRING_VocPath, recogResult.strVocFullName);

	TiXmlElement* pLangResult = pRoot->AddNewElem(XML_STRING_RD_LangResult);
	pLangResult->AddNewChild(XML_STRING_LangName, recogResult.strLang);
	pLangResult->AddNewChild(XML_STRING_LangOutput, recogResult.fLangOutput);

	TiXmlElement* pGenderResult = pRoot->AddNewElem(XML_STRING_RD_GenderResult);
	pGenderResult->AddNewChild(XML_STRING_GenderName, recogResult.strGender);
	pGenderResult->AddNewChild(XML_STRING_GenderOutput, recogResult.fGenderOutput);

	XmlAddKwdResults(pRoot, vtKwdResults);

	XmlAddVprResults(pRoot, vtVprResults);


	// fixme vpr not finish

	TiXmlPrinter xmlPrinter;
	xmlPrinter.SetIndent( "\t" );
	xmlDoc.Accept(&xmlPrinter);

	CStringW strXml;
	strXml = xmlPrinter.CStr();
	int nXmlLen = UnicodeToUTF8(strXml, NULL, NULL);
	int nTotalLen = sizeof(VoiceInfResult) + nXmlLen;

	unsigned char* pMsg = new unsigned char[nTotalLen];
	memset(pMsg, 0, nTotalLen);

	// !!! copy body first then set header
	// VoiceInfResult body
	memcpy(pMsg, &infResult, sizeof(infResult));

	// VoiceInfResult header
	VoiceInfResult* pInf = (VoiceInfResult*)pMsg;
	pInf->w_Head.w_nFlag = MSG_MAGIC;
	pInf->w_Head.w_nMsgLen = nTotalLen - sizeof(PackHead);
	pInf->w_Head.w_nMsgType = MSG_VOICE_INFO_RESULT;
	pInf->w_Head.w_nVer = V_VERSION;
	// pInf->w_Head.w_szMsgID is "\0"

	// VoiceInfResult XML part
	pInf->w_nXmlLength = nXmlLen;
	UnicodeToUTF8(strXml, pMsg + sizeof(VoiceInfResult), nXmlLen);

	return pMsg;
}

static void SplitString(const CString& strLine, const TCHAR tchSeperator, std::vector<CString>& vtStrSplits)
{
	vtStrSplits.clear();
	int nNum = -1;	
	int nStrLen = strLine.GetLength();

	while (nNum < nStrLen - 1)
	{
		int nStartPos = nNum + 1;
		nNum = strLine.Find(tchSeperator, nStartPos);
		if (nNum == nStartPos)
		{
			vtStrSplits.push_back(_T(""));
			continue;
		}
		if (nNum == -1)
		{
			nNum = nStrLen;
		}
		CString strTmp = strLine.Mid(nStartPos, nNum - nStartPos);
		vtStrSplits.push_back(strTmp);
	}
}

int mainTinyXml(void)
{
	int nRetCode = 0;

	CString x;
	x = "a;b;;;;;d;";
	vector<CString> vtstr;
	SplitString(x,L';', vtstr);

	VoiceInfResult infResult;
	memset(&infResult, 0, sizeof(VoiceInfResult));
	SRecogResult recogResult;
	vector<KwdResult> vtKwdResults;
	vector<VprResultItem> vtVprResults;

	infResult.w_nStartTime = 11;
	infResult.w_szFileName[0] = 'a';
	//////////////////////////////////////////////////////////////////////////

	recogResult.fLangOutput = 12.5;
	recogResult.strGender = "gender";
	recogResult.strLang = "lang";
	recogResult.strVocFullName = "fullname";

	KwdResult kwd1;
	kwd1.w_szKeyword = "kwd1";
	kwd1.w_vtResultItems.push_back(KwdResultItem(1, 2, 3));
	kwd1.w_vtResultItems.push_back(KwdResultItem(4, 5, 6));

	KwdResult kwd2;
	kwd2.w_szKeyword = "kwd2";
	kwd2.w_vtResultItems.push_back(KwdResultItem(11, 12, 13));
	kwd2.w_vtResultItems.push_back(KwdResultItem(14, 15, 16));

	vtKwdResults.push_back(kwd1);
	vtKwdResults.push_back(kwd2);

	vtVprResults.push_back(VprResultItem("01", 101.5));
	vtVprResults.push_back(VprResultItem("02", 201.5));
	vtVprResults.push_back(VprResultItem("03", 301.5));

	unsigned char* pXmlTest = GetMsgVoiceInfResult(infResult, recogResult, vtKwdResults, vtVprResults);



	TiXmlDocument* pDoc = new TiXmlDocument();
	TiXmlDeclaration* pXmlDecl = new TiXmlDeclaration("1.0", "utf-8", "");
	pDoc->LinkEndChild(pXmlDecl);

	TiXmlElement* pRoot = new TiXmlElement("Root");
	pRoot->SetAttribute("name", "drvoice");
	pDoc->LinkEndChild(pRoot);

	TiXmlElement* pLv1 = new TiXmlElement("Lv1");
	TiXmlText* pText = new TiXmlText("TEXT");
	pLv1->LinkEndChild(pText);
	pRoot->LinkEndChild(pLv1);

	printf("lv1 value: %s\n", pLv1->Value());
	printf("TEXT value: %s\n", pText->Value());

	pRoot->AddNewChild("Lv2", "Lv2-text");
	pRoot->AddNewChild("���Ľڵ�", "����\r\n����");

	TiXmlElement* pLv3 = new TiXmlElement("Lv3");
	pLv3->AddNewChild("lv3-1", "lv3-1-text");
	pRoot->LinkEndChild(pLv3);

	pDoc->Print();

	TiXmlPrinter xmlPrinter;
	xmlPrinter.SetIndent( "\t" );
	pDoc->Accept(&xmlPrinter);
	const char* pXml = xmlPrinter.CStr();
	int nLen = strlen(pXml);
	int nLen2 = xmlPrinter.Size();

	CString strXml;
	strXml = "���Ľڵ�";
	const char* p1 = "���Ľڵ�";
	wchar_t p2 = '��';
	wchar_t p3 = L'��';
	wchar_t p4 = *(wchar_t*)p1;
	strXml = p1;


		
// 	int xxxxx;
// 	xxxxx ++;
// 	xxxxx ++;
// 	xxxxx ++;
//	getchar();	

	//////////////////////////////////////////////////////////////////////////
	// ���ڱ��룺
	/*
	1. �������ݣ�
	 ��
	 UNICODE wchar_t: 0x4e2d
	 ANSI wchar_t: 0xd6d0
	 ANSI char*: 0xd6d0
	 UTF-8 char*: 0x E4 B8 AD
	 ע�⣺ANSI wchar_t��ANSI char*��Ȼ������ͬ������С���ڴ����ݲ�ͬ
	2. XML DOCָ���ı����ʵ�ʳ����еı����Ӧ��ϵ���ֶ�����ת����
	һ��XML������ANSI��Ȼ��CStr()��ȡXML string,��ת���xmlͷencoding

	ANSI ->��Unicode -> UTF-8
	     MBTOWB -> WBTOMB
	һ�����ת����Ҫ��UNICODE��Ϊ�м���
	*/


	delete pDoc;

	return nRetCode;
}
