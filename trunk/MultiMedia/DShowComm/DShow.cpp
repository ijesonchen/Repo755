/////////////////////////////////////////////////////////////////////////////
// $Header: /cvsroot/mmconvert/mmconvert/MMConvert/Src/Common/DShow.cpp,v 1.18 2006/04/13 09:56:02 ping235 Exp $
//
/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2005 - 2006 ping235.  All rights reserved.
/////////////////////////////////////////////////////////////////////////////
//
//  This file is subject to the terms of the GNU General Public License as
//  published by the Free Software Foundation.  A copy of this license is
//  included with this software distribution in the file COPYING.  If you
//  do not have a copy, you may obtain a copy by writing to the Free
//  Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details
/////////////////////////////////////////////////////////////////////////////
// Change Log
//
// Date          Developer             Changes
// $Date: 2006/04/13 09:56:02 $        $Author: ping235 $              $Name:  $
//
/////////////////////////////////////////////////////////////////////////////
// CVS Log
// $Log: DShow.cpp,v $
// Revision 1.18  2006/04/13 09:56:02  ping235
// . changed ffdshow version compare of setuphelper
// . added *.vob and *.mpa to supported formats list
// . added compressor cache feature(avi, mkx)
// . added settings "Settings\\Encoder" "VBlackItemCount"|"VBlackItemx"
// . added video and audio compressor black items support
// . added helper.h: macro DIM_OF
// . added WriteString and ReadString to basefile.h
// . improved CBaseFile implementation.
// . changed Filters\clean.bat: added clean project.h _i.c,_p.c
// . renamed CBaseFile to CStdioFilePort
// . added CFilePort as base class of File class
// . added CWin32File
// . changed basefile.h to Unix LF mode
// . changed aboutpage to propertysheet, removed CAboutBox class
// . added AboutGeneralPage, AboutCreditsPage, AboutHistoryPage class and related resource
// . added string table resource, for the caption of AboutPropertySheet
// . added string table resource, for error msg of loading history file
// . renamed IDD_ABOUTBOX to IDD_ABOUT_GENERAL_PAGE
// . added about_general_page hyperlink feature
// . added tooltips for donate & mailto button of about_general_page and related string table resource(IDS_CAPTION_DONATE, IDS_CAPTION_MAILTO)
// . added compressor cache feature when app startup
// . added Asf Profile cache
// . updated zh_cn & zh_tw resource
// . added output path history
//
// Revision 1.17  2006/01/18 09:23:40  ping235
// replaced wchar ansi conversion marco with conversion class; removed all USES_CONVERSION
//
// Revision 1.16  2005/12/30 07:47:26  ping235
// updated license period
//
// Revision 1.15  2005/12/30 06:23:21  ping235
// added MpgConverter
//
// Revision 1.14  2005/12/23 08:56:50  ping235
// changed cmdline version printf to _tprintf
//
// Revision 1.13  2005/12/19 06:51:25  ping235
// fixed some warning,merged resource into zh_cn&zh_tw
//
// Revision 1.12  2005/11/26 08:07:57  ping235
// migrate whole solution to Visual Studio 2005
//
// Revision 1.11  2005/11/21 10:05:23  ping235
// completed wav encode
//
// Revision 1.10  2005/11/21 03:47:58  ping235
// modified mp3 wav implementation, begin using writer
//
// Revision 1.9  2005/11/16 05:47:06  ping235
// added mp3 option
//
// Revision 1.8  2005/11/15 10:03:16  ping235
// added mp3 options
//
// Revision 1.7  2005/11/10 09:48:18  ping235
// fixed avi convert error: set first audio stream as avi master stream
//
// Revision 1.6  2005/10/15 08:06:31  ping235
// . added avi compressor options(avi option dlg)
// . fixed AddFilterByClsid or Name filter name error when name not specified explicitly
// . added DShow::LoadGraphFile&SaveGraphFile for debugging
// . move cmdline specified code to CConverterCMD
// . changed time format to human reading format
//
// Revision 1.5  2005/09/16 08:24:39  ping235
// modified resource
//
// Revision 1.4  2005/09/08 09:43:18  ping235
// translate MMConvert into english
//
// Revision 1.3  2005/08/22 03:38:33  ping235
// modified exception:added context
//
// Revision 1.2  2005/08/10 08:13:14  ping235
// added asf&avi task option function
//
// Revision 1.1.1.1  2005/08/02 05:17:18  ping235
// initial,GUI 1.0.0.0,CMD 1.3.0.1
//
//////////////////////////////////////////////////////////////////////////////

#include ".\dshow.h"

void HANDLE_HR_EXP(HRESULT){};

CDShow::CDShow(void)
{
}

CDShow::~CDShow(void)
{
}

void CDShow::AddToROT(IGraphBuilder* spGraphBuild,bool bPause,DWORD& dwROTToken)
{
	CComPtr<IRunningObjectTable> spROT = NULL;
	HRESULT hr = GetRunningObjectTable(0,&spROT);
	HANDLE_HR_EXP(hr);

	CComPtr<IMoniker> spMoniker = NULL;
	CComPtr<IUnknown> spunkGraph = NULL;
	hr = spGraphBuild->QueryInterface(&spunkGraph);
	HANDLE_HR_EXP(hr);

	CString strItem;
	strItem.Format(L"FilterGraph %08x pid %08x",
					(DWORD_PTR)(IUnknown*)spunkGraph,
					GetCurrentProcessId());
	hr = CreateItemMoniker(L"!",strItem.GetString(),&spMoniker);
	HANDLE_HR_EXP(hr);
	
	hr = spROT->Register(ROTFLAGS_REGISTRATIONKEEPSALIVE,spunkGraph,spMoniker,&dwROTToken);
	HANDLE_HR_EXP(hr);

	if(bPause)
		AfxMessageBox(_T("You can open GraphEdit to check Filter Graph now, press OK to continue."));
}

void CDShow::RemoveFromROT(DWORD dwROTToken)
{
	if(dwROTToken!=0)
	{
		CComPtr<IRunningObjectTable> spROT = NULL;
		HRESULT hr = GetRunningObjectTable(0,&spROT);
		HANDLE_HR_EXP(hr);

		spROT->Revoke(dwROTToken);
	}
}

HRESULT CDShow::FindFilter(IGraphBuilder* pGraph,REFGUID iid,void** ppUnk)
{
	if(!pGraph||!ppUnk)
		return E_POINTER;

	HRESULT hr = E_FAIL;
	IEnumFilters* pEnum = NULL;
	IBaseFilter* pF = NULL;
	if(FAILED(pGraph->EnumFilters(&pEnum)))
		return hr;

	while(S_OK == pEnum->Next(1,&pF,0))
	{
		hr = pF->QueryInterface(iid,ppUnk);
		pF->Release();
		if(SUCCEEDED(hr))
			break;
	}
	pEnum->Release();

	return hr;
}

bool CDShow::IsVideoPinUnconnected(IBaseFilter* pBF)
{
	bool bRes = false;

	HRESULT hr = S_OK;
	CComPtr<IEnumPins> spPins = NULL;
	hr = pBF->EnumPins(&spPins);
	if(SUCCEEDED(hr))
	{
		ULONG ulFetched = 0;
		IPin* pPin = NULL;
		while(spPins->Next(1,&pPin,&ulFetched) == S_OK)
		{
			CComPtr<IPin> spPeerPin = NULL;
			hr = pPin->ConnectedTo(&spPeerPin);
			if(spPeerPin==NULL)
			{
				bRes = true;
				break;
			}

			pPin->Release();
			pPin = NULL;
		}
	}

	return bRes;
}

HRESULT CDShow::FindVideoOutPin(IBaseFilter* pBF,IPin** ppPin)
{
	HRESULT hr = S_OK;
	CComPtr<IEnumPins> spPins = NULL;
	hr = pBF->EnumPins(&spPins);
	if(SUCCEEDED(hr))
	{
		ULONG ulFetched = 0;
		IPin* pPin = NULL;
		while(spPins->Next(1,&pPin,&ulFetched) == S_OK)
		{
			AM_MEDIA_TYPE mt;
			hr = pPin->ConnectionMediaType(&mt);
			if(SUCCEEDED(hr)&&(mt.majortype==MEDIATYPE_Video))
			{
				hr = pPin->ConnectedTo(ppPin);
			}

			if(SUCCEEDED(hr))
				FreeMediaType(mt);
			pPin->Release();
		}
	}

	return hr;
}

HRESULT CDShow::FindConnectedPin(IBaseFilter* pBF,IPin** ppPin)
{
	HRESULT hr = S_OK;
	CComPtr<IEnumPins> spPins = NULL;
	hr = pBF->EnumPins(&spPins);
	if(SUCCEEDED(hr))
	{
		ULONG ulFetched = 0;
		IPin* pPin = NULL;
		if(spPins->Next(1,&pPin,&ulFetched) == S_OK)
		{
			hr = pPin->ConnectedTo(ppPin);

			pPin->Release();
		}
	}

	return hr;
}

int CDShow::GetPinCount(
    IBaseFilter *pBF, // Pointer to the starting filter
    PIN_DIRECTION Dir)    // Direction to search (upstream or downstream)
{
	int nCount = 0;

	HRESULT hr = S_OK;
	CComPtr<IEnumPins> spPins = NULL;
	hr = pBF->EnumPins(&spPins);
	if(SUCCEEDED(hr))
	{
		ULONG ulFetched = 0;
		IPin* pPin = NULL;
		while(spPins->Next(1,&pPin,&ulFetched) == S_OK)
		{
			PIN_DIRECTION eDir;
			hr = pPin->QueryDirection(&eDir);
			if(SUCCEEDED(hr)&&(eDir==Dir))
			{
				nCount++;
			}
			pPin->Release();
		}
	}

	return nCount;
}

HRESULT CDShow::GetPin(IBaseFilter *pBF,PIN_DIRECTION eDir,IPin** ppPin,bool bConnected)
{
	HRESULT hr = S_OK;
	CComPtr<IEnumPins> spPins = NULL;
	hr = pBF->EnumPins(&spPins);
	if(SUCCEEDED(hr))
	{
		ULONG ulFetched = 0;
		CComPtr<IPin> spPin = NULL;
		while(spPins->Next(1,&spPin,&ulFetched) == S_OK)
		{
			PIN_DIRECTION eTmpDir;
			hr = spPin->QueryDirection(&eTmpDir);
			if(SUCCEEDED(hr)&&(eTmpDir==eDir)&&(IsPinConnected(spPin)==bConnected))
			{
				*ppPin = spPin;
				(*ppPin)->AddRef();

				break;
			}
			
			spPin = NULL;
		}
	}

	return hr;
}

HRESULT CDShow::GetPin(IBaseFilter *pBF,PIN_DIRECTION eDir,IPin** ppPin)
{
	HRESULT hr = S_OK;
	CComPtr<IEnumPins> spPins = NULL;
	hr = pBF->EnumPins(&spPins);
	if(SUCCEEDED(hr))
	{
		ULONG ulFetched = 0;
		CComPtr<IPin> spPin = NULL;
		while(spPins->Next(1,&spPin,&ulFetched) == S_OK)
		{
			PIN_DIRECTION eTmpDir;
			hr = spPin->QueryDirection(&eTmpDir);
			if(SUCCEEDED(hr)&&(eTmpDir==eDir))
			{
				*ppPin = spPin;
				(*ppPin)->AddRef();

				break;
			}
			
			spPin = NULL;
		}
	}

	return hr;
}

CComPtr<IPin> CDShow::GetPin(IBaseFilter *pBF,PIN_DIRECTION eDir)
{
	CComPtr<IPin> spPin = NULL;
	GetPin(pBF,eDir,&spPin);
	return spPin;
}

bool CDShow::IsPinConnected(IPin* pPin)
{
	CComPtr<IPin> spPeerPin = NULL;
	pPin->ConnectedTo(&spPeerPin);
	return spPeerPin!=NULL;
}

//create Null Render
CComPtr<IPin> CDShow::CreateNullRender(IGraphBuilder * pGB)
{
	//Create Null Render
	CComPtr<IBaseFilter> spNullRender = NULL;
	HRESULT hr = AddFilterByClsid(pGB,CLSID_NullRenderer,NULL/*_T("Null Render")*/,&spNullRender);
	if(SUCCEEDED(hr))
	{
		return GetPin(spNullRender,PINDIR_INPUT);
	}

	return NULL;
}

HRESULT CDShow::ConnectAllPinToNullRender(IGraphBuilder * pGB,IBaseFilter *pBF)
{
	HRESULT hr = S_OK;
	CComPtr<IEnumPins> spPins = NULL;
	hr = pBF->EnumPins(&spPins);
	if(SUCCEEDED(hr))
	{
		ULONG ulFetched = 0;
		IPin* pPin = NULL;
		while(spPins->Next(1,&pPin,&ulFetched) == S_OK)
		{
			PIN_DIRECTION eDir;
			hr = pPin->QueryDirection(&eDir);
			if(SUCCEEDED(hr)&&(eDir==PINDIR_OUTPUT)&&!IsPinConnected(pPin))
			{
				CComPtr<IPin> spInPin = CreateNullRender(pGB);
				if(spInPin!=NULL)
					hr = pGB->ConnectDirect(pPin,spInPin,NULL);
			}
			pPin->Release();
		}
	}

	return hr;
}

HRESULT CDShow::RemoveFilter(IGraphBuilder * inGraph, IBaseFilter * inFilter)
{
	HRESULT hr = E_FAIL;

	if (inGraph && inFilter)
	{
		IEnumPins * pinEnum = 0;
		if (SUCCEEDED(inFilter->EnumPins(&pinEnum)))
		{
			pinEnum->Reset();
			IPin * pin = 0;
			ULONG cFetched = 0;
			bool pass = true;
			while (pass && SUCCEEDED(pinEnum->Next(1, &pin, &cFetched)))
			{
				if (pin && cFetched)
				{
					IPin * connectedPin = 0;
					pin->ConnectedTo(&connectedPin);
					if(connectedPin) 
					{
						inGraph->Disconnect(connectedPin);
						inGraph->Disconnect(pin);

						connectedPin->Release();
					}
					pin->Release();
				}
				else
				{
					pass = false;
				}
			}
			pinEnum->Release();

			hr = inGraph->RemoveFilter(inFilter);
		}
	}

	return hr;
}

//从pPin开始向上游回朔，删除只有一个输出Pin的Filter，并返回第一个
//多于1个Pin的Filter的输出Pin（与ppPin相连的那个）
HRESULT CDShow::NukeUpstream(IGraphBuilder * inGraph, IBaseFilter * inFilter)
{
	HRESULT hr = E_FAIL;

	if (inGraph && inFilter)
	{
		IEnumPins * pinEnum = 0;
		if (SUCCEEDED(inFilter->EnumPins(&pinEnum)))
		{
			pinEnum->Reset();
			IPin * pin = 0;
			ULONG cFetched = 0;
			bool pass = true;
			while (pass && SUCCEEDED(pinEnum->Next(1, &pin, &cFetched)))
			{
				if (pin && cFetched)
				{
					IPin * connectedPin = 0;
					pin->ConnectedTo(&connectedPin);
					if(connectedPin) 
					{
						PIN_INFO pininfo;
						if (SUCCEEDED(connectedPin->QueryPinInfo(&pininfo)))
						{
							if(pininfo.dir == PINDIR_OUTPUT) 
							{
								int nPinCount = GetPinCount(pininfo.pFilter,PINDIR_OUTPUT);

								inGraph->Disconnect(connectedPin);
								inGraph->Disconnect(pin);

								//如果仅有一个输出Pin，则删除它及其上游
								if(nPinCount==1)
								{
									hr = NukeUpstream(inGraph, pininfo.pFilter);
									inGraph->RemoveFilter(pininfo.pFilter);
								}
								//如果多于一个输出Pin，则连接它的所有未连接的Pin到Null Render
								else if(nPinCount>1)
								{
									CComPtr<IPin> spInPin = CreateNullRender(inGraph);
									if(spInPin!=NULL)
										hr = inGraph->ConnectDirect(connectedPin,spInPin,NULL);
//									hr = ConnectAllPinToNullRender(inGraph,pininfo.pFilter,PINDIR_OUTPUT);
								}
							}
							pininfo.pFilter->Release();
						}
						connectedPin->Release();
					}
					pin->Release();
				}
				else
				{
					pass = false;
				}
			}
			pinEnum->Release();
		}
	}

	return hr;
}

HRESULT CDShow::ConnectToSplitter(IGraphBuilder * inGraph, 
								  IBaseFilter * inFilter, 
								  REFCLSID clsid,
								  IBaseFilter** ppDecoder)
{
	HRESULT hr = E_FAIL;

	if (inGraph && inFilter)
	{
		IEnumPins * pinEnum = 0;
		if (SUCCEEDED(inFilter->EnumPins(&pinEnum)))
		{
			pinEnum->Reset();
			IPin * pin = 0;
			ULONG cFetched = 0;
			bool pass = true;
			while (pass && SUCCEEDED(pinEnum->Next(1, &pin, &cFetched)))
			{
				if (pin && cFetched)
				{
					IPin * connectedPin = 0;
					pin->ConnectedTo(&connectedPin);
					if(connectedPin) 
					{
						PIN_INFO pininfo;
						if (SUCCEEDED(connectedPin->QueryPinInfo(&pininfo)))
						{
							if(pininfo.dir == PINDIR_OUTPUT) 
							{
								int nPinCount = GetPinCount(pininfo.pFilter,PINDIR_OUTPUT);

								inGraph->Disconnect(connectedPin);
								inGraph->Disconnect(pin);

								//如果仅有一个输出Pin，则删除它及其上游
								if(nPinCount==1)
								{
									hr = ConnectToSplitter(inGraph, pininfo.pFilter, clsid, ppDecoder);
//									hr = NukeUpstream(inGraph, pininfo.pFilter);
									inGraph->RemoveFilter(pininfo.pFilter);
								}
								//如果多于一个输出Pin，则连接它的所有未连接的Pin到Null Render
								else if(nPinCount>1)
								{
									hr = AddFilterByClsid(inGraph, clsid, NULL, ppDecoder);
									CComPtr<IPin> spInPin = GetPin(*ppDecoder,PINDIR_INPUT);
									if (spInPin)
									{
										hr = inGraph->ConnectDirect(connectedPin, spInPin, NULL);
									}
									else
									{
										hr = E_FAIL;
									}
								}
							}
							pininfo.pFilter->Release();
						}
						connectedPin->Release();
					}
					pin->Release();
				}
				else
				{
					pass = false;
				}
			}
			pinEnum->Release();
		}
	}

	return hr;
}


//
HRESULT CDShow::GetAVRenderer(IGraphBuilder* pGB,CBaseFilterList& lstAudio,CBaseFilterList& lstVideo)
{
	HRESULT hr = S_OK;
	CComPtr<IEnumFilters> spEnumFilters = NULL;
	hr = pGB->EnumFilters(&spEnumFilters);
	if(SUCCEEDED(hr))
	{
		CComPtr<IBaseFilter> spBF = NULL;
		while(spEnumFilters->Next(1,&spBF,NULL)==S_OK)
		{
			//因为我们仅需要知道是否有IBasicVideo或IBasicAudio，所以
			//设spFilter类型为IUnknown
			CComPtr<IUnknown> spFilter = NULL;
			if(SUCCEEDED(spBF->QueryInterface(IID_IBasicVideo,(void**)&spFilter)))
			{
				((IBaseFilter*)spBF)->AddRef();
				lstVideo.push_back(spBF);
			}
			spFilter = NULL;
			if(SUCCEEDED(spBF->QueryInterface(IID_IBasicAudio,(void**)&spFilter)))
			{
				((IBaseFilter*)spBF)->AddRef();
				lstAudio.push_back(spBF);
			}
			
			spBF = NULL;
		}
	}

	return hr;
}

// this also comes in useful when using the IEnumMediaTypes interface so
// that you can copy a media type, you can do nearly the same by creating
// a CMediaType object but as soon as it goes out of scope the destructor
// will delete the memory it allocated (this takes a copy of the memory)

AM_MEDIA_TYPE * WINAPI CDShow::CreateMediaType(AM_MEDIA_TYPE const *pSrc)
{
    ASSERT(pSrc);

    // Allocate a block of memory for the media type

    AM_MEDIA_TYPE *pMediaType =
        (AM_MEDIA_TYPE *)CoTaskMemAlloc(sizeof(AM_MEDIA_TYPE));

    if (pMediaType == NULL) {
        return NULL;
    }
    // Copy the variable length format block

    HRESULT hr = CopyMediaType(pMediaType,pSrc);
    if (FAILED(hr)) {
        CoTaskMemFree((PVOID)pMediaType);
        return NULL;
    }

    return pMediaType;
}


//  Copy 1 media type to another

HRESULT WINAPI CDShow::CopyMediaType(AM_MEDIA_TYPE *pmtTarget, const AM_MEDIA_TYPE *pmtSource)
{
    //  We'll leak if we copy onto one that already exists - there's one
    //  case we can check like that - copying to itself.
    ASSERT(pmtSource != pmtTarget);
    *pmtTarget = *pmtSource;
    if (pmtSource->cbFormat != 0) {
        ASSERT(pmtSource->pbFormat != NULL);
        pmtTarget->pbFormat = (PBYTE)CoTaskMemAlloc(pmtSource->cbFormat);
        if (pmtTarget->pbFormat == NULL) {
            pmtTarget->cbFormat = 0;
            return E_OUTOFMEMORY;
        } else {
            CopyMemory((PVOID)pmtTarget->pbFormat, (PVOID)pmtSource->pbFormat,
                       pmtTarget->cbFormat);
        }
    }
    if (pmtTarget->pUnk != NULL) {
        pmtTarget->pUnk->AddRef();
    }

    return S_OK;
}

//  Free an existing media type (ie free resources it holds)

void WINAPI CDShow::FreeMediaType(AM_MEDIA_TYPE& mt)
{
    if (mt.cbFormat != 0) {
        CoTaskMemFree((PVOID)mt.pbFormat);

        // Strictly unnecessary but tidier
        mt.cbFormat = 0;
        mt.pbFormat = NULL;
    }
    if (mt.pUnk != NULL) {
        mt.pUnk->Release();
        mt.pUnk = NULL;
    }
}

// general purpose function to delete a heap allocated AM_MEDIA_TYPE structure
// which is useful when calling IEnumMediaTypes::Next as the interface
// implementation allocates the structures which you must later delete
// the format block may also be a pointer to an interface to release

void WINAPI CDShow::DeleteMediaType(AM_MEDIA_TYPE *pmt)
{
    // allow NULL pointers for coding simplicity

    if (pmt == NULL) {
        return;
    }

    FreeMediaType(*pmt);
    CoTaskMemFree((PVOID)pmt);
}

// 通过给出的clsid实例化Filter，并将其加入到GraphBuilder
HRESULT CDShow::AddFilterByClsid(IGraphBuilder* pGB, REFCLSID clsid,LPCTSTR lpszName,IBaseFilter** ppFilter)
{
	//Create Filter
	CComPtr<IBaseFilter> spFilter = NULL;
	HRESULT hr = spFilter.CoCreateInstance(clsid);
	if(SUCCEEDED(hr))
	{
		//add null render to graph builder
		if(lpszName==NULL)
			hr = pGB->AddFilter(spFilter,GetFilterName(spFilter));
		else
			hr = pGB->AddFilter(spFilter,lpszName);
		if(SUCCEEDED(hr)&&(ppFilter!=NULL))
		{
			*ppFilter = spFilter;
			(*ppFilter)->AddRef();
		}
	}

	return hr;
}

void CDShow::EnumDXSystemDevice(CLSID clsidType,PENUMDXSYSDEVICE pFn,void* pData)
{
	HRESULT hr = S_OK;
	CComPtr<ICreateDevEnum> spEnum = NULL;
	hr = spEnum.CoCreateInstance(CLSID_SystemDeviceEnum,NULL,CLSCTX_INPROC);
	HANDLE_HR_EXP(hr);
	CComPtr<IEnumMoniker> spEnumMoniker = NULL;
	hr = spEnum->CreateClassEnumerator(clsidType,&spEnumMoniker,0);
	HANDLE_HR_EXP(hr);
	if(spEnumMoniker!=NULL)
	{
		IMoniker* pMoniker = NULL;
		ULONG ulFetched = 0;
		UINT index = 0;
		while(spEnumMoniker->Next(1,&pMoniker,&ulFetched) == S_OK)
		{
			bool bContinue = pFn(pMoniker,pData);
			pMoniker->Release();
			pMoniker = NULL;
			if(!bContinue)
				break;
		}
	}
}

bool CDShow::OnEnumDXObjList(IMoniker* pMoniker,void* pData)
{
	CDX_ObjList& list = *(CDX_ObjList*)pData;
	CComPtr<IPropertyBag> spBag = NULL;
	HRESULT hr = pMoniker->BindToStorage(0,0,IID_IPropertyBag,(void**)&spBag);
	if(SUCCEEDED(hr))
	{
		DX_OBJ DxObj;
		VARIANT var;
		var.vt = VT_BSTR;
		hr = spBag->Read(L"FriendlyName",&var,0);
		if(SUCCEEDED(hr))
		{
			DxObj.strFriendName = var.bstrVal;
			::SysFreeString(var.bstrVal);
		}
		var.vt = VT_BSTR;
		hr = spBag->Read(L"CLSID",&var,0);
		if(SUCCEEDED(hr))
		{
			DxObj.strDispName = var.bstrVal;
			::SysFreeString(var.bstrVal);
		}

		list[UINT(list.size())] = DxObj;
	}

	return true;
}

void CDShow::GetDXObjList(CLSID clsidType,CDX_ObjList& list)
{
	list.clear();
	EnumDXSystemDevice(clsidType,&OnEnumDXObjList,&list);
}

bool CDShow::OnEnumFindSysDev(IMoniker* pMoniker,void* pData)
{
	LPCTSTR lpszName = LPCTSTR(pData);
	IMoniker** ppmk = (IMoniker**)(pData);
	ppmk++;
	
	CComPtr<IPropertyBag> spBag = NULL;
	HRESULT hr = pMoniker->BindToStorage(0,0,IID_IPropertyBag,(void**)&spBag);
	if(SUCCEEDED(hr))
	{
		DX_OBJ DxObj;
		VARIANT var;
		var.vt = VT_BSTR;
		hr = spBag->Read(L"CLSID",&var,0);
		if(SUCCEEDED(hr))
		{
			DxObj.strDispName = var.bstrVal;
			::SysFreeString(var.bstrVal);
			if((DxObj.strDispName==lpszName)&&(*ppmk==NULL))
			{
				*ppmk = pMoniker;
				(*ppmk)->AddRef();
			}
		}
	}

	return true;
}

HRESULT CDShow::FindSysDev(CLSID clsidType,LPCTSTR lpszName,IMoniker** ppmk)
{
	*ppmk = NULL;
	void* pData[]={(void*)lpszName,ppmk};
	EnumDXSystemDevice(clsidType,&OnEnumFindSysDev,pData);
	
	return S_OK;
}

HRESULT CDShow::CreateFilterByName(LPCTSTR lpszName,IBaseFilter** ppFilter)
{
	IBindCtx* pCtx;
	HRESULT hr = ::CreateBindCtx(0,&pCtx);
	ULONG ulEaten = 0;
	IMoniker* pmk = NULL;
	hr = ::MkParseDisplayName(pCtx,lpszName,&ulEaten,&pmk);
	pCtx->Release();
	if(SUCCEEDED(hr))
	{
		hr = pmk->BindToObject(NULL,NULL,IID_IBaseFilter,(void**)ppFilter);
		pmk->Release();
	}

	return hr;
}

// 通过给出的displayname实例化Filter，并将其加入到GraphBuilder
HRESULT CDShow::AddFilterByName(IGraphBuilder* pGB, LPCTSTR lpszDispName,LPCTSTR lpszName,IBaseFilter** ppFilter)
{
	//Create Filter
	CComPtr<IBaseFilter> spFilter = NULL;
	HRESULT hr = CreateFilterByName(lpszDispName,&spFilter);
	if(SUCCEEDED(hr))
	{
		if(lpszName==NULL)
			hr = pGB->AddFilter(spFilter,GetFilterName(spFilter));
		else
			hr = pGB->AddFilter(spFilter,lpszName);
		if(SUCCEEDED(hr)&&(ppFilter!=NULL))
		{
			*ppFilter = spFilter;
			(*ppFilter)->AddRef();
		}
	}

	return hr;
}

bool CDShow::OnEnumGetDispNameByIndex(IMoniker* pMoniker,void* pData)
{
	bool bRes = true;
	LONGLONG* pTmpData = (LONGLONG*)pData;
	
	//如果找到了
	if(pTmpData[1]==pTmpData[0])
	{
		LPWSTR wszDisplayName = NULL;
		HRESULT hr = pMoniker->GetDisplayName(NULL,NULL,&wszDisplayName);
		if(SUCCEEDED(hr))
		{
			*(CString*)pTmpData[2] = wszDisplayName;
			CoTaskMemFree(wszDisplayName);
			bRes = false;
		}
	}
	pTmpData[1]++;

	return bRes;
}

// 由给出的Index返回对应的音频或视频压缩器的DispName
CString CDShow::GetDispNameByIndex(bool bAudio,int nIndex)
{
	HRESULT hr = S_OK;
	CString str;
	try
	{
		LONGLONG Data[3] = {nIndex,0,LONGLONG(&str)};

		CDShow::EnumDXSystemDevice(
			bAudio?CLSID_AudioCompressorCategory:CLSID_VideoCompressorCategory,
			&OnEnumGetDispNameByIndex,
			&Data);
	}
	catch(...)
	{
		hr = E_FAIL;
	}
	return str;
}

bool CDShow::OnEnumListCompressors(IMoniker* pMoniker,void* pData)
{
	int& nIndex = *(int*)pData;
	
	CComPtr<IPropertyBag> spBag = NULL;
	HRESULT hr = pMoniker->BindToStorage(0,0,IID_IPropertyBag,(void**)&spBag);
	if(SUCCEEDED(hr))
	{
		DX_OBJ DxObj;
		VARIANT var;
		var.vt = VT_BSTR;
		hr = spBag->Read(L"FriendlyName",&var,0);
		if(SUCCEEDED(hr))
		{
			CString str = var.bstrVal;
			_tprintf(_T("[%2d] %s\n"),nIndex++,(LPCTSTR)str);
			::SysFreeString(var.bstrVal);
		}
	}

	return true;
}

// 列出系统中安装的音视频压缩器
HRESULT CDShow::ListCompressors(bool bAudio)
{
	HRESULT hr = S_OK;
	try
	{
		int nIndex = 0;
		CDShow::EnumDXSystemDevice(
			bAudio?CLSID_AudioCompressorCategory:CLSID_VideoCompressorCategory,
			&OnEnumListCompressors,
			&nIndex);
	}
	catch(...)
	{
		hr = E_FAIL;
	}
	return hr;
}

CString CDShow::GetFilterName(IBaseFilter* pBF)
{
	ASSERT(pBF!=NULL);

	FILTER_INFO fi;
	memset(&fi,0,sizeof(fi));
	if(pBF!=NULL)
	{
		VERIFY(SUCCEEDED(pBF->QueryFilterInfo(&fi)));
	}
	return fi.achName;
}

//base filter related property pages
bool CDShow::HasDialog(IBaseFilter* pBF)
{
	ASSERT(pBF!=NULL);

	bool bHasProp = false;
	CComQIPtr<ISpecifyPropertyPages> spProp(pBF);
	if(spProp!=NULL)
	{
		//
		CAUUID caGUID;
		spProp->GetPages(&caGUID);
		CoTaskMemFree(caGUID.pElems);
		bHasProp = (caGUID.cElems!=0);
	}

	if(bHasProp)
		return true;

	CComQIPtr<IAMVfwCompressDialogs> spVfwDlg(pBF);
	bool bVfwDlg = ((spVfwDlg!=NULL)&&(spVfwDlg->ShowDialog(VfwCompressDialog_QueryConfig,NULL)==S_OK));
	return bVfwDlg;
}

HRESULT CDShow::ShowDialog(HWND hParent, IBaseFilter* pBF)
{
	ASSERT(pBF!=NULL);
	if(pBF==NULL)
		return E_POINTER;

	HRESULT hr = S_OK;
	CComQIPtr<ISpecifyPropertyPages> spProp(pBF);
	if(spProp!=NULL)
	{
		//show property pages
		CAUUID caGUID;
		hr = spProp->GetPages(&caGUID);
		spProp.Release();
		if(SUCCEEDED(hr))
		{
			if(caGUID.cElems!=0)
			{
				IUnknown* punk = NULL;
				hr = pBF->QueryInterface(IID_IUnknown,(void**)&punk);
				if(SUCCEEDED(hr))
				{
					hr = OleCreatePropertyFrame(
						hParent,
						0,
						0,
						GetFilterName(pBF),
						1,
						&punk,
						caGUID.cElems,
						caGUID.pElems,
						GetUserDefaultLCID(),
						0,
						NULL
					);
					punk->Release();
				}

				//clean up
				CoTaskMemFree(caGUID.pElems);
			}
		}
	}
	else
	{
		CComQIPtr<IAMVfwCompressDialogs> spVfwDlg(pBF);
		if(spVfwDlg!=NULL)
		{
//ASSERT(FALSE);
//			static LPVOID lp = NULL;
//			static int nSize = 0;
//			if(lp != NULL)
//				hr = spVfwDlg->SetState(lp, nSize);
			hr = spVfwDlg->ShowDialog(VfwCompressDialog_Config,hParent);
			//if(lp == NULL)
			//{
			//	hr = spVfwDlg->GetState(lp, &nSize);
			//	lp = new BYTE[nSize];
			//	hr = spVfwDlg->GetState(lp, &nSize);
			//}
		}
	}

	return hr;
}

//save a filter graph to a GraphEdit (.grf) file. This can be useful for debugging.
HRESULT CDShow::SaveGraphFile(IGraphBuilder *pGraph, TCHAR *tszPath)
{
    HRESULT hr;
    
    IStorage *pStorage = NULL;
    hr = StgCreateDocfile(
        CT2W(tszPath),
        STGM_CREATE | STGM_TRANSACTED | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
        0, &pStorage);
    if(FAILED(hr)) 
    {
        return hr;
    }

    const WCHAR wszStreamName[] = L"ActiveMovieGraph"; 
    IStream *pStream;
    hr = pStorage->CreateStream(
        wszStreamName,
        STGM_WRITE | STGM_CREATE | STGM_SHARE_EXCLUSIVE,
        0, 0, &pStream);
    if (FAILED(hr)) 
    {
        pStorage->Release();    
        return hr;
    }

    IPersistStream *pPersist = NULL;
    pGraph->QueryInterface(IID_IPersistStream, (void**)&pPersist);
    hr = pPersist->Save(pStream, TRUE);
    pStream->Release();
    pPersist->Release();
    if (SUCCEEDED(hr)) 
    {
        hr = pStorage->Commit(STGC_DEFAULT);
    }
    pStorage->Release();
    return hr;
}

//load a GraphEdit (.grf) file to graph, only for debugging
HRESULT CDShow::LoadGraphFile(IGraphBuilder *pGraph, const TCHAR *tszName)
{
    IStorage *pStorage = 0;
    if (S_OK != StgIsStorageFile(CT2CW(tszName)))
    {
        return E_FAIL;
    }
    HRESULT hr = StgOpenStorage(CT2CW(tszName), 0, 
        STGM_TRANSACTED | STGM_READ | STGM_SHARE_DENY_WRITE, 
        0, 0, &pStorage);
    if (FAILED(hr))
    {
        return hr;
    }
    IPersistStream *pPersistStream = 0;
    hr = pGraph->QueryInterface(IID_IPersistStream,
             reinterpret_cast<void**>(&pPersistStream));
    if (SUCCEEDED(hr))
    {
        IStream *pStream = 0;
        hr = pStorage->OpenStream(L"ActiveMovieGraph", 0, 
            STGM_READ | STGM_SHARE_EXCLUSIVE, 0, &pStream);
        if(SUCCEEDED(hr))
        {
            hr = pPersistStream->Load(pStream);
            pStream->Release();
        }
        pPersistStream->Release();
    }
    pStorage->Release();
    return hr;
}

//connect pBF1 with pBF2
HRESULT CDShow::ConnectFilters(IGraphBuilder *pGraph, IBaseFilter* pBF1,IBaseFilter* pBF2)
{
	HRESULT hr = S_OK;

	//obtain filter one's output pin
	CComPtr<IPin> spOutPin = NULL;
	hr = GetPin(pBF1,PINDIR_OUTPUT,&spOutPin,false);
    if (FAILED(hr))
    {
        return hr;
    }

    if (spOutPin==NULL)
    {
        return (hr = E_POINTER);
    }

	//obtain filter two's input pin
	CComPtr<IPin> spInPin = NULL;
	hr = GetPin(pBF2,PINDIR_INPUT,&spInPin,false);
    if (FAILED(hr))
    {
        return hr;
    }

    if (spInPin==NULL)
    {
        return (hr = E_POINTER);
    }

	//connect filter 1 and filter 2
	hr = pGraph->Connect(spOutPin,spInPin);
	
	return hr;
}

//connect pPin1 with pBF2
HRESULT CDShow::ConnectFilters(IGraphBuilder *pGraph, IPin* pPin1,IBaseFilter* pBF2)
{
	HRESULT hr = S_OK;

	ASSERT(pPin1!=NULL);

	//obtain filter two's input pin
	CComPtr<IPin> spInPin = NULL;
	hr = GetPin(pBF2,PINDIR_INPUT,&spInPin,false);
    if (FAILED(hr))
    {
        return hr;
    }

	//connect pPin1 and filter 2
	hr = pGraph->Connect(pPin1,spInPin);
	
	return hr;
}

// set audio output filter's media type, change all output pins' media type
HRESULT CDShow::SetAudioFilterMediaType(IBaseFilter* pBaseFilter,DWORD nSamplesPerSec,WORD wBitsPerSample,WORD wChannels,DWORD dwAvgBytesPerSec)
{
	ASSERT(pBaseFilter!=NULL);
	if(pBaseFilter==NULL)
		return E_POINTER;

	HRESULT hr = S_OK;

	// Pin enumeration
	CComPtr<IEnumPins> spEnumPins = NULL;
	hr = pBaseFilter->EnumPins(&spEnumPins);
	if (FAILED(hr))
	{
		// ERROR HERE
		return hr;
	}

	CComPtr<IPin> spPin = NULL;
	while (spEnumPins->Next(1, &spPin, 0) == S_OK)
	{
		PIN_DIRECTION sDirection;
		spPin->QueryDirection(&sDirection);
		// Output Pin ?
		if (sDirection == PINDIR_OUTPUT)
		{
			CComPtr<IAMStreamConfig> spStreamConfig = NULL;
			hr = spPin->QueryInterface(IID_IAMStreamConfig, (void**) &spStreamConfig);
			if (SUCCEEDED(hr))
			{
				AM_MEDIA_TYPE* pmt = NULL;
				hr = spStreamConfig->GetFormat(&pmt);
				if (SUCCEEDED(hr))
				{
					WAVEFORMATEX* pFmt = (WAVEFORMATEX*) pmt->pbFormat;
					if(dwAvgBytesPerSec!=0)
						pFmt->nAvgBytesPerSec = dwAvgBytesPerSec;
					if(nSamplesPerSec!=0)
						pFmt->nSamplesPerSec = nSamplesPerSec;
					if(wChannels!=0)
						pFmt->nChannels = wChannels;
					if(wBitsPerSample!=0)
						pFmt->wBitsPerSample = wBitsPerSample;
					//if has any change reset format
					if(dwAvgBytesPerSec|nSamplesPerSec|wChannels|wBitsPerSample)
						hr = spStreamConfig->SetFormat(pmt);
					
					//free media type
					FreeMediaType(*pmt);
				}
			}
		}
		spPin = NULL;
	}

	return hr;
}
