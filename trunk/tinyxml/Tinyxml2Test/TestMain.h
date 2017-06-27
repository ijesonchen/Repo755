#include "..\TinyXml2\tinyxml2.h"

// show XML file strFileName content
void PrintXml(CString strFileName);

// show all content of pe, including child element
void PrintAllText(const tinyxml2::TiElement* pe);

// show name, attribute and text of pe, no child.
void PrintElement(const tinyxml2::TiElement* pe);

/*
// sub string search
// is strleng >= 1G, return -1;
int StringLength1G(const char* psz);
int StringLength1G(const wchar_t* pwsz);
int SubIndex(const char* T, const char* pSub, int tlength, int slength, int pos);
int KmpIndex(const char* T, const char* pSub, int tlength, int slength, int pos);

// return data write count.
// if input has a NULL terminator, a NULL terminator wrote in output and counted.
// if pBuffer is NULL, return required buffer length.
// weather NULL terminator included lies on if input.
int UnicodeToUtf8(const wchar_t* pUnicode, int nUnicodeBufLen, char* pUtf8, int nUtf8Len);
int UnicodeToUtf8(const wchar_t* pszUnicode, char* pUtf8, int nUtf8Len);

int Utf8ToUnicode(const char* pUtf8, int nUtf8Len,  wchar_t* pUnicode, int nUnicodeBufLen);
int Utf8ToUnicode(const char* pszUtf8,  wchar_t* pUnicode, int nUnicodeBufLen);
*/