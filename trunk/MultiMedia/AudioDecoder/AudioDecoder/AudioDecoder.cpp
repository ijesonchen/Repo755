// AudioDecoder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "AudioDecoder.h"
#include "..\..\DShowComm\DShowComm.h"
#include <conio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

#define RELEASE_PTR_RESOURCE(x) if (x)\
{\
	x->Release();\
	x = NULL;\
}

int Audio2Pcm(wchar_t* pInput, wchar_t* pOutput)
{
	WAVEFORMATEX WavFmt;
	AM_MEDIA_TYPE AmType;
	WAVEFORMATEX* pWavFmt = &WavFmt;
	pWavFmt->wFormatTag = 1;
	pWavFmt->nChannels = 1;
	pWavFmt->nSamplesPerSec = 8000;
	pWavFmt->nAvgBytesPerSec = 16000;
	pWavFmt->nBlockAlign = 2;
	pWavFmt->wBitsPerSample = 16;
	pWavFmt->cbSize = 0;
	AM_MEDIA_TYPE* pAmType = &AmType;
	pAmType->majortype = MEDIATYPE_Audio;
	pAmType->subtype = MEDIASUBTYPE_PCM;
	pAmType->bFixedSizeSamples = 1;
	pAmType->bTemporalCompression = 0;
	pAmType->lSampleSize = 2;
	pAmType->formattype = FORMAT_WaveFormatEx;
	pAmType->pUnk = NULL;
	pAmType->cbFormat = sizeof(WAVEFORMATEX);
	pAmType->pbFormat = (BYTE*)pWavFmt;

	//	bool bComInited = false;

	GUID CLSID_AmrSplitter;	
	UuidFromStringA((unsigned char*)"24FA7933-FE18-46A9-914A-C2AA0DBACE93", &CLSID_AmrSplitter);
	GUID CLSID_AmrDecoder;
	UuidFromStringA((unsigned char*)"50DDA33E-C529-4343-9689-338ADC793BB5", &CLSID_AmrDecoder);
	GUID CLSID_AcmWrapper;
	UuidFromStringA((unsigned char*)"6A08CF80-0E18-11CF-A24D-0020AFD79767", &CLSID_AcmWrapper);
	GUID CLSID_WavDest;
	UuidFromStringA((unsigned char*)"3C78B8E2-6C4D-11D1-ADE2-0000F8754B99", &CLSID_WavDest);

	HRESULT hr = 0;
	int nRet = 0;
	IGraphBuilder* pGraphBuilder = NULL;
	IBaseFilter* pWavDest = NULL;
	IBaseFilter* pCodec = NULL;
	IBaseFilter* pAcmDecoder = NULL;
	IBaseFilter* pAcmSplitter = NULL;
	IBaseFilter* pFileWrite = NULL;
	IFileSinkFilter* pSink = NULL;
	IMediaEvent *pMediaEvent = NULL;
	IMediaControl* pMediaControl = NULL;

	bool bComInited = false;
	try
	{	
		hr = CoInitialize(NULL);
		switch (hr)
		{
		case S_FALSE:
			bComInited = true;
			break;
		default:
			break;
		}
		hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**) &pGraphBuilder);
		if (S_OK != hr)
		{
			nRet = hr;
			throw ERROR_THROW;
		}
/*
		hr = AddFilterByClsid(pGraphBuilder, L"WAV Dest", CLSID_WavDest, &pWavDest);
		if (S_OK != hr)
		{
			nRet = ERROR_CONV_ADDDEST;
			throw ERROR_THROW;
		}
		hr = AddFilterByClsid(pGraphBuilder, L"ACM Wrapper", CLSID_AcmWrapper, &pCodec);
		if (S_OK != hr)
		{
			nRet = ERROR_CONV_ADDCODEC;
			throw ERROR_THROW;
		}
		hr = AddFilterByClsid(pGraphBuilder, L"MONOGRAM AMR Decoder", CLSID_AmrDecoder, &pAcmDecoder);
		if (S_OK != hr)
		{
			nRet = ERROR_CONV_ADD_AMRDECODER;
			throw ERROR_THROW;
		}
		hr = AddFilterByClsid(pGraphBuilder, L"MONOGRAM AMR Splitter", CLSID_AmrSplitter, &pAcmSplitter);
		if (S_OK != hr)
		{
			nRet = ERROR_CONV_ADD_AMRSPLITTER;
			throw ERROR_THROW;
		}
		// Output file filter
		hr = AddFilterByClsid(pGraphBuilder, L"File Writer", CLSID_FileWriter, &pFileWrite);
		if (S_OK != hr)
		{
			nRet = ERROR_CONV_SETOUTPUT;
			throw ERROR_THROW;
		}
		if (S_OK != pFileWrite->QueryInterface(IID_IFileSinkFilter, (void**) &pSink))
		{
			nRet = ERROR_CONV_SETOUTPUT;
			throw ERROR_THROW;
		}
		if (S_OK != pSink->SetFileName(pOutput, NULL))
		{
			nRet = ERROR_CONV_SETOUTPUT;
			throw ERROR_THROW;
		}
		pSink->Release();
		pSink = NULL;

		// Check for output File
		CFileStatus fileStatus;
		CString str;
		str = pOutput;
		if (CFile::GetStatus(str, fileStatus))
		{
			try {
				CFile::Remove(str);
			} catch(...) {
				// nothing to do
			}			
		}
	*/
		hr = pGraphBuilder->RenderFile(pInput, NULL);

		if (S_OK != hr)
		{
			nRet = ERROR_CONV_RENDER;
			throw ERROR_THROW;
		}
// 		if (!SetFilterFormat(pAmType, pCodec))
// 		{
// 			nRet = ERROR_CONV_SETFMT;
// 			throw ERROR_THROW;
// 		}
		hr = pGraphBuilder->QueryInterface(IID_IMediaControl, (void**) &pMediaControl);
		if (S_OK != hr)
		{
			nRet = ERROR_CONV_RUN;
			throw ERROR_THROW;
		}
		hr = pMediaControl->Run();
// 		if (S_OK != hr)
// 		{
// 			nRet = ERROR_CONV_RUN;
// 			throw ERROR_THROW;
// 		}
		long nCode = 0;
		pGraphBuilder->QueryInterface(IID_IMediaEvent, (void**) &pMediaEvent);
		if (S_OK != hr)
		{
			nRet = ERROR_CONV_CTRL;
			throw ERROR_THROW;
		}
		// Wait until job complete
		while (nCode != EC_COMPLETE) {
			pMediaEvent->WaitForCompletion(1000, &nCode);
		}
		if (S_OK != hr)
		{
			nRet = ERROR_CONV_CTRL;
			throw ERROR_THROW;
		}
		pMediaControl->Stop();
		if (S_OK != hr)
		{
			nRet = ERROR_CONV_CTRL;
			throw ERROR_THROW;
		}
		wprintf(_T("O:  %s\n"), pOutput);
	}
	catch (int)
	{
		// error occurred
		wprintf(_T("E:  %s  C: 0x%X\n"), pInput, nRet);
	}
	catch (CInvalidArgException*)
	{
		wprintf(_T("E:  %s  C: 0x%X\n"), pInput, -2);
		return -2;
	}

	// clean up
	RELEASE_PTR_RESOURCE(pSink);
	RELEASE_PTR_RESOURCE(pMediaEvent);
	RELEASE_PTR_RESOURCE(pMediaControl);
	RELEASE_PTR_RESOURCE(pAcmDecoder);
	RELEASE_PTR_RESOURCE(pAcmSplitter);
	RELEASE_PTR_RESOURCE(pCodec);
	RELEASE_PTR_RESOURCE(pWavDest);
	RELEASE_PTR_RESOURCE(pGraphBuilder);

	if (!bComInited)
	{
		CoUninitialize();
	}
	return nRet;
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
	}

	CString strFileName;
	if (argc > 1)
	{
		strFileName = argv[1];
	}
	else
	{
		strFileName = L"D:\\Data\\AudioTest\\wav1.mp3";
	}

	IGraphBuilder *pGraph = NULL;
	IMediaControl *pControl = NULL;
	IMediaEvent   *pEvent = NULL;

	// Initialize the COM library.
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		printf("ERROR - Could not initialize COM library");
	}

	// Create the filter graph manager and query for interfaces.
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, 
		IID_IGraphBuilder, (void **)&pGraph);
	if (FAILED(hr))
	{
		printf("ERROR - Could not create the Filter Graph Manager.");
	}

	hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl);
	hr = pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);

	// Build the graph. IMPORTANT: Change this string to a file on your system.
	hr = pGraph->RenderFile(strFileName, NULL);
	if (SUCCEEDED(hr))
	{
		// Run the graph.
		hr = pControl->Run();
		if (SUCCEEDED(hr))
		{
			// Wait for completion.
			int ch = 0;
			long evCode = 0;
			bool bStop = false;
			while (evCode != EC_COMPLETE) 
			{
				// Note: Do not use INFINITE in a real application, because it
				// can block indefinitely.
				pEvent->WaitForCompletion(100, &evCode);

				while (ch = getch())
				{		
					if (ch == 'q' || ch == EOF)
					{
						pControl->Stop();
						printf("Stoped.\n");
						bStop = true;
						break;
					}	
				}	
				if (bStop)
				{
					break;
				}
			}
			
			printf("Finished\n");
		}
	}
	else
	{
		char szErr[MAX_ERROR_TEXT_LEN];
		DWORD res = AMGetErrorTextA(hr, szErr, MAX_ERROR_TEXT_LEN);
		if (res == 0)
		{
			printf("Unknown Error: 0x%2x\n", hr);
		}
		else
		{
			printf("0x%X: %s\n", hr, szErr);
		}
	}
	pControl->Release();
	pEvent->Release();
	pGraph->Release();
	CoUninitialize();

	return nRetCode;
}
