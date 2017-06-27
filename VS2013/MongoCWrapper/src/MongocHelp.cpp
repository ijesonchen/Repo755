#include "mfcafx.h"
#include "MongocHelp.h"

#include <mongoc.h>
#include <iostream>
#include <sstream>

#include "MongoAuto.h"

#include "mfcnew.h"

using namespace std;

namespace MongoClib
{
	// Return length of string. Must be NULL terminated.
	// If length >= 1G, return -1; terminate NULL not counted
	template <typename T>
	int StringLength1G(const T* psz)
	{
		int nLength = 0;
		while (psz[nLength++])
		{
			if (nLength >= 0x40000000)
			{
				return -1;
			}
		}
		return --nLength;
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
		*pUtf8 = 0;
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
	int Utf8ToUnicode(const char* pszUtf8, wchar_t* pUnicode, int nUnicodeBufLen)
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

	int Unf8ToWchar(const char* pUtf8, wchar_t& wch)
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
	int Utf8ToUnicode(const char* pUtf8, int nUtf8Len, wchar_t* pUnicode, int nUnicodeBufLen)
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
		*pUnicode = 0;
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


} // MongoClib