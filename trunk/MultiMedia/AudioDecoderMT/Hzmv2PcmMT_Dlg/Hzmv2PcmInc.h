#ifndef _HZMV2PCMINC_H_
#define _HZMV2PCMINC_H_

typedef int (*Hzmv2PcmFunc)(const wchar_t* pInput, const wchar_t* pOutput); 

#ifdef __cplusplus    // If used by C++ code, 
extern "C" {          // we need to export the C interface
#endif

	__declspec(dllexport) int __cdecl Hzmv2Pcm(const wchar_t* pInput, const wchar_t* pOutput);

#ifdef __cplusplus
}
#endif

#define AUDIO_FILE_DLL			(_T("Hzmv2PcmDll_Release_Win32.dll"))
#define AUDIO_FILE_DLL_DEBUG	(_T("Hzmv2PcmDll_Debug_Win32.dll"))

// #define WM_CONVERT_BASE	(WM_USER + 100)
// #define WM_START_CONV	(WM_CONVERT_BASE + 1)
// #define WM_STOP_CONV	(WM_CONVERT_BASE + 2)

#endif // _HZMV2PCMINC_H_