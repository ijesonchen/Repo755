// for windows platform, MSVC, use afx.h & DEBUG_NEW to detect memory leak.
// mfcnew.h MUST be last .h file included ONLY in .cpp

#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0502
#endif

#if defined( _WIN32 ) || defined (WIN32)  
	#if defined(_MSC_VER) && defined(_DEBUG) && defined( _AFXDLL ) && defined(_DLL)
		#define __AFX_DEBUG_NEW__
	#endif
	#if defined(_MSC_VER) && defined(_DEBUG) && !defined( _AFXDLL ) && !defined(_DLL)
		#define __AFX_DEBUG_NEW__
	#endif
#endif

#ifdef __AFX_DEBUG_NEW__
	#include <afx.h>
#endif // __AFX_DEBUG_NEW__