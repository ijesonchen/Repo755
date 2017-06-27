/////////////////////////////////////////////////////////////////////////////
// $Header: /cvsroot/mmconvert/mmconvert/MMConvert/Src/Common/DShow.h,v 1.9 2005/12/30 07:47:26 ping235 Exp $
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
// $Date: 2005/12/30 07:47:26 $        $Author: ping235 $              $Name:  $
//
/////////////////////////////////////////////////////////////////////////////
// CVS Log
// $Log: DShow.h,v $
// Revision 1.9  2005/12/30 07:47:26  ping235
// updated license period
//
// Revision 1.8  2005/11/26 08:07:57  ping235
// migrate whole solution to Visual Studio 2005
//
// Revision 1.7  2005/11/21 10:05:23  ping235
// completed wav encode
//
// Revision 1.6  2005/11/16 05:47:06  ping235
// added mp3 option
//
// Revision 1.5  2005/11/15 10:03:16  ping235
// added mp3 options
//
// Revision 1.4  2005/11/14 09:45:07  ping235
// added mp3 config items
//
// Revision 1.3  2005/11/10 09:48:18  ping235
// fixed avi convert error: set first audio stream as avi master stream
//
// Revision 1.2  2005/10/15 08:06:31  ping235
// . added avi compressor options(avi option dlg)
// . fixed AddFilterByClsid or Name filter name error when name not specified explicitly
// . added DShow::LoadGraphFile&SaveGraphFile for debugging
// . move cmdline specified code to CConverterCMD
// . changed time format to human reading format
//
// Revision 1.1.1.1  2005/08/02 05:17:18  ping235
// initial,GUI 1.0.0.0,CMD 1.3.0.1
//
//////////////////////////////////////////////////////////////////////////////

#include <list>
#include <vector>
#include <map>
#include "DShowComm.h"

void HANDLE_HR_EXP(HRESULT);

#pragma once

extern "C" const GUID __declspec(selectany) CLSID_NullRenderer =
    {0xc1f400a4,0x3f08,0x11d3,{0x9f,0x0b,0x00,0x60,0x08,0x03,0x9e,0x37}};
extern "C" const GUID __declspec(selectany) CLSID_FileWriter =
    {0x8596E5F0,0x0DA5,0x11D0,{0xBD,0x21,0x00,0xA0,0xC9,0x11,0xce,0x86}};
extern "C" const GUID __declspec(selectany) CLSID_WaveDest =
    {0x3C78B8E2,0x6C4D,0x11D1,{0xAD,0xE2,0x00,0x00,0xF8,0x75,0x4B,0x99}};

//mp3 encoder(built-in Windows)
static const TCHAR c_tszMp3CodecDispName[] = _T("@device:cm:{33D9A761-90C8-11D0-BD43-00A0C911CE86}\\85MPEG Layer-3");
//lame mp3 encoder
static const TCHAR c_tszLameMp3CodecDispName[] = _T("@device:sw:{083863F1-70DE-11D0-BD40-00A0C911CE86}\\{B8D27088-DF5F-4B7C-98DC-0E91A1696286}");

//һЩDirectShow�ĸ�������
class CDShow
{
public:
	class CBaseFilterList:public std::list<IBaseFilter*>
	{
	public:
		~CBaseFilterList()
		{
			iterator it = begin();
			for(;it!=end();it++)
			{
				(*it)->Release();
			}
		}
	};
	typedef struct _DX_OBJ
	{
		CString strDispName;//CLSID
		CString strFriendName;
	}DX_OBJ;
	typedef std::map<UINT,DX_OBJ> CDX_ObjList;
	typedef bool PENUMDXSYSDEVICE(IMoniker* pMoniker,void* pData);

public:
	CDShow(void);
	~CDShow(void);

public:
	//ע�ᵽROT
	static void AddToROT(IGraphBuilder* spGraphBuild,bool bPause,DWORD& dwROTToken);
	//��ROTɾ��
	static void RemoveFromROT(DWORD dwROTToken);
	//��GraphBuild�в���ʵ���˸����ӿڵ�Filter
	static HRESULT FindFilter(IGraphBuilder* pGraph,REFGUID iid,void** ppUnk);
	//���Filter����ƵPin�����ӣ��򷵻�false�����򷵻�true
	static bool IsVideoPinUnconnected(IBaseFilter* pBF);
	//���Filter�����ӣ��򷵻�Filter����VideoInPin�����ӵ�Pin
	static HRESULT FindVideoOutPin(IBaseFilter* pBF,IPin** ppPin);
	//��inFilter��ʼ�����λ�˷��ɾ����֮������ֻ��һ�����Pin��Filter�������ص�һ��
	//����1��Pin��Filter�����Pin����ppPin�������Ǹ���
	//inFilter����ɾ��
	static HRESULT NukeUpstream(IGraphBuilder * inGraph, IBaseFilter * inFilter);

	// ����decoder��splitter
	static HRESULT ConnectToSplitter(IGraphBuilder * inGraph, 
		IBaseFilter * inFilter, 
		REFCLSID clsid,
		IBaseFilter** ppDecoder);
	//����pBF��Dir�����Pin��Ŀ
	static int GetPinCount(IBaseFilter *pBF, PIN_DIRECTION Dir);
	//����Null Renderer��������뵽GraphBuild
	static CComPtr<IPin> CreateNullRender(IGraphBuilder * pGB);
	//����GraphBuild�е�Video��Audio Renderer Filter
	static HRESULT GetAVRenderer(IGraphBuilder* pGB,CBaseFilterList& lstAudio,CBaseFilterList& lstVideo);
	//���pBF�����ӣ����������һ�����ӵ�Pin���ӵ�Pin�����򷵻�NULL
	static HRESULT FindConnectedPin(IBaseFilter* pBF,IPin** ppPin);
	//�Ͽ���inFilterֱ������������Pin��Ȼ�����GraphBuild��ɾ����������ɾ��inFilter����
	static HRESULT RemoveFilter(IGraphBuilder * inGraph, IBaseFilter * inFilter);
	//This function frees a task-allocated AM_MEDIA_TYPE structure from memory. 
	static void WINAPI FreeMediaType(AM_MEDIA_TYPE& mt);
	//This function deletes a task-allocated AM_MEDIA_TYPE structure. 
	static void WINAPI DeleteMediaType(AM_MEDIA_TYPE *pmt);
	// this also comes in useful when using the IEnumMediaTypes interface so
	// that you can copy a media type, you can do nearly the same by creating
	// a CMediaType object but as soon as it goes out of scope the destructor
	// will delete the memory it allocated (this takes a copy of the memory)
	static AM_MEDIA_TYPE * WINAPI CreateMediaType(AM_MEDIA_TYPE const *pSrc);
	//  Copy 1 media type to another
	static HRESULT WINAPI CopyMediaType(AM_MEDIA_TYPE *pmtTarget, const AM_MEDIA_TYPE *pmtSource);
	//����pBF�ĵ�һ����������Pin
	static CComPtr<IPin> GetPin(IBaseFilter *pBF,PIN_DIRECTION eDir);
	static HRESULT GetPin(IBaseFilter *pBF,PIN_DIRECTION eDir,IPin** ppPin);
	static HRESULT GetPin(IBaseFilter *pBF,PIN_DIRECTION eDir,IPin** ppPin,bool bConnected);
	//���pPin�����ӣ��򷵻�true
	static bool IsPinConnected(IPin* pPin);
	//��pBF������δ���ӵ����Pin���ӵ�Null Renderer
	static HRESULT ConnectAllPinToNullRender(IGraphBuilder * pGB,IBaseFilter *pBF);
	// ͨ��������clsidʵ����Filter����������뵽GraphBuilder
	static HRESULT AddFilterByClsid(IGraphBuilder* pGB, REFCLSID clsid,LPCTSTR lpszName,IBaseFilter** ppFilter = NULL);
	// ͨ��������clsidʵ����Filter����������뵽GraphBuilder
	static HRESULT AddFilterByName(IGraphBuilder* pGB, LPCTSTR lpszDispName,LPCTSTR lpszName,IBaseFilter** ppFilter);
	//ö��ϵͳ�е�DX�豸���磺����Ƶ��׽��ѹ�����豸���˾�
	static void EnumDXSystemDevice(CLSID clsidType,PENUMDXSYSDEVICE pFn,void* pData=NULL);
	//EnumDXSystemDevice�Ļص���������GetDXObjListʹ��
	static bool OnEnumDXObjList(IMoniker* pMoniker,void* pData);
	static void GetDXObjList(CLSID clsidType,CDX_ObjList& list);
	//EnumDXSystemDevice�Ļص���������FindSysDevʹ��
	static bool OnEnumFindSysDev(IMoniker* pMoniker,void* pData);
	static HRESULT FindSysDev(CLSID clsidType,LPCTSTR lpszName,IMoniker** ppmk);
	// �ɸ�����Index���ض�Ӧ����Ƶ����Ƶѹ������DispName
	static CString GetDispNameByIndex(bool bAudio,int nIndex);
	static bool OnEnumGetDispNameByIndex(IMoniker* pMoniker,void* pData);
	// �г�ϵͳ�а�װ������Ƶѹ����
	static HRESULT ListCompressors(bool bAudio);
	static bool OnEnumListCompressors(IMoniker* pMoniker,void* pData);
	static HRESULT CreateFilterByName(LPCTSTR lpszName,IBaseFilter** ppFilter);
	//get filter name
	static CString GetFilterName(IBaseFilter* pBF);
	//base filter related property pages
	static bool HasDialog(IBaseFilter* pBF);
	static HRESULT ShowDialog(HWND hParent, IBaseFilter* pBF);
	//load a GraphEdit (.grf) file to graph, only for debugging
	static HRESULT LoadGraphFile(IGraphBuilder *pGraph, const TCHAR* tszName);
	//save a filter graph to a GraphEdit (.grf) file. This can be useful for debugging.
	static HRESULT SaveGraphFile(IGraphBuilder *pGraph, TCHAR *tszPath);
	//connect pBF1 with pBF2
	static HRESULT ConnectFilters(IGraphBuilder *pGraph, IBaseFilter* pBF1,IBaseFilter* pBF2);
	//connect pPin1 with pBF2
	static HRESULT ConnectFilters(IGraphBuilder *pGraph, IPin* pPin1,IBaseFilter* pBF2);
	// set audio output filter's media type, change all output pins' media type
	static HRESULT SetAudioFilterMediaType(IBaseFilter* pBaseFilter,DWORD nSamplesPerSec,WORD wBitsPerSample,WORD wChannels,DWORD dwAvgBytesPerSec);
};
