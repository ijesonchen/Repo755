#include "PublicFunc.h"
#include <memory>

#include <afxmt.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


namespace PublicFunc
{
	using namespace std;
	//////////////////////////////////////////////////////////////////////////
	// 	find pSub in pSrc (brute-force, very low efficient, KMP suggested)
	// 	if found, return index of pSub in pSrc, or -1 if not found.

	// string must be NULL terminated.
	int SubIndexBF(const char* pSrc, const char* pSub, int nStartPos)
	{
		int nSrcLength = StringLength1G(pSrc);
		int nSubLength = StringLength1G(pSub);

		if (nSrcLength <= 0 || nSubLength <= 0)
		{
			return -1;
		}
		return SubIndexBF(pSrc, nSrcLength, pSub, nSubLength, nStartPos);
	}

	// string can be not NULL terminated.
	int SubIndexBF(const char* pSrc, int nSrcLength, const char* pSub, int nSubLength, int nStartPos)
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



	/************************************************************************/
	/*                            RaiiPtr                                   */
	/************************************************************************/
	// used for RAII, store pointer and free mem when class destructed.

	RaiiPtr::RaiiPtr()
		: pBuffer(0)
		, nCount(0)
		, nCapacity(0)
	{
	}

	RaiiPtr::~RaiiPtr()
	{
		for (int ii = 0; ii < nCount; ++ii)
		{
			delete pBuffer[ii];
		}
		delete []pBuffer;
	}

	void RaiiPtr::Add(void* p)
	{
		if (nCount > nCapacity)
		{
			// should not get there.
			return;
		}

		if (nCapacity == nCount)
		{
			// realloc
			if (!nCapacity)
			{
				nCapacity = 16;
				pBuffer = new void*[nCapacity];
				memset(pBuffer, 0, sizeof(void*) * nCapacity);
			}
			else
			{
				int nNewCapacity = nCapacity * 2;
				void** pNewBuffer = new void*[nNewCapacity];
				memset(pNewBuffer, 0, sizeof(void*) * nNewCapacity);
				memcpy(pNewBuffer, pBuffer, sizeof(void*) * nCapacity);
				delete pBuffer;
				pBuffer = pNewBuffer;
				nCapacity = nNewCapacity;
			}
		}
		pBuffer[nCount] = p;
		nCount++;
	}


	/*************************************************************************

	UNICODE <-> UTF-8
	UNICODE hex			bit	UTF-8 bin presentation
	00000000 0000007F	 7	0xxxxxxx
	00000080 000007FF	11  110xxxxx 10xxxxxx
	00000800 0000FFFF	16	1110xxxx 10xxxxxx 10xxxxxx

	UCS-4
	00010000 001FFFFF	21	11110xxx 10xxxxxx 10xxxxxx 10xxxxxx 
	00200000 03FFFFFF	26	111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
	04000000 7FFFFFFF	31	1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 

	*************************************************************************/


	// wstring (unicode) -> string (utf-8)
	std::string MSEncodeUtf8(const std::wstring& strUnicode)
	{
		std::string utf8;
		const wchar_t* pw = strUnicode.c_str();
		int nUnicode = strUnicode.length();
		int nUtf8 = ::WideCharToMultiByte(CP_UTF8, 0, 
			pw, nUnicode, NULL, 0, NULL, NULL);
		if (nUtf8 <= 0)
		{
			return utf8;
		}
		char* p = new char[nUtf8];
		std::auto_ptr<char> ap(p);
		memset(p, 0, nUtf8 * sizeof(char));
		int nRet = ::WideCharToMultiByte(CP_UTF8, 0, 
			pw, nUnicode, p, nUtf8, NULL, NULL);
		if (nRet <= 0)
		{
			return utf8;
		}
		utf8.assign(p, nUtf8);
		return utf8;
	}
	// string (utf-8) -> wstring (unicode)
	std::wstring MSDecodeUtf8(const std::string& strUtf8)
	{
		std::wstring unicode;
		const char* p = strUtf8.c_str();
		int nUtf8 = strUtf8.length();
		int nUnicode = MultiByteToWideChar(CP_UTF8, 0, p, nUtf8, NULL, 0);
		if (nUnicode <= 0)
		{
			return unicode;
		}
		wchar_t* pw = new wchar_t[nUnicode];
		std::auto_ptr<wchar_t> ap(pw);
		memset(pw, 0, nUnicode * sizeof(wchar_t));
		int nRet = MultiByteToWideChar(CP_UTF8, 0, p, nUtf8, pw, nUnicode);
		if (nRet <= 0)
		{
			return unicode;
		}
		unicode.assign(pw, nUnicode);

		return unicode;
	}

	// wstring (unicode) -> string (utf-8)
	std::string EncodeUtf8(const std::wstring& strUnicode)
	{
		const wchar_t* pw = strUnicode.c_str();
		int nUnicode = strUnicode.length();
		int nUtf8 = UnicodeToUtf8(pw, nUnicode, NULL, 0);
		if (nUtf8 <= 0)
		{
			return "";
		}
		char* p = new char[nUtf8];
		std::auto_ptr<char> ap(p);
		memset(p, 0, nUtf8 * sizeof(char));
		int nRet = UnicodeToUtf8(pw, nUnicode, p, nUtf8);
		if (nRet <= 0)
		{
			return "";
		}
		string utf8;
		utf8.assign(p, nUtf8);
		return utf8;
	}
	// string (utf-8) -> wstring (unicode)
	std::wstring DecodeUtf8(const std::string& strUtf8)
	{
		const char* p = strUtf8.c_str();
		int nUtf8 = strUtf8.length();
		int nUnicode = Utf8ToUnicode(p, nUtf8, NULL, 0);
		if (nUnicode <= 0)
		{
			return L"";
		}
		wchar_t* pw = new wchar_t[nUnicode];
		std::auto_ptr<wchar_t> ap(pw);
		memset(pw, 0, nUnicode * sizeof(wchar_t));
		int nRet = Utf8ToUnicode(p, nUtf8, pw, nUnicode);
		if (nRet <= 0)
		{
			return L"";
		}
		wstring unicode;
		unicode.assign(pw, nUnicode);
		return unicode;
	}

	// input must NULL terminated.
	int UnicodeToUtf8(const wchar_t* pszUnicode, char* pUtf8, int nUtf8Len)
	{
		int nInputLen = StringLength1G(pszUnicode) + 1;
		if (nInputLen <= 0)
		{
			return -1;
		}
		return UnicodeToUtf8(pszUnicode, nInputLen, pUtf8, nUtf8Len);
	}

	// function does not check NULL terminator.
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
				if (nwUtf8 > nUtf8Len - 1)
				{
					return -3;
				}
				// 0xxxxxxx
				pUtf8[nwUtf8++] = (char)wch; 
			}
			else if (wch <= 0x07FF)
			{
				if (nwUtf8 > nUtf8Len - 2)
				{
					return -3;
				}
				// 110xxxxx 10xxxxxx
				pUtf8[nwUtf8++] = (char)((wch >> 6) | 0xC0); 
				pUtf8[nwUtf8++] = (char)((wch & 0x3F) | 0x80); 
			}
			else if (wch <= 0xFFFF)
			{
				if (nwUtf8 > nUtf8Len - 3)
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

	// input must NULL terminated.
	int Utf8ToUnicode(const char* pszUtf8,  wchar_t* pUnicode, int nUnicodeBufLen)
	{
		int nInputLen = StringLength1G(pszUtf8) + 1;
		if (nInputLen <= 0)
		{
			return -1;
		}
		return Utf8ToUnicode(pszUtf8, nInputLen, pUnicode, nUnicodeBufLen);
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
			wch = (wchar_t)-1;
			nUtf8Count -= 100;
			break;
		}

		return nUtf8Count;
	}

	// function does not check NULL terminator.
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
};
