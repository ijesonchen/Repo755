///////////////////////////////////////////////////////////
//  LinkMap.cpp
//  Implementation of the Class LinkMap
//  Created on:      28-¾ÅÔÂ-2015 9:20:34
//  Original author: user
///////////////////////////////////////////////////////////

#include "LinkMap.h"
#include "NetLink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace NetLayer
{


LinkMap::~LinkMap()
{
	ClearLink();
}

bool LinkMap::Capture(SOCKET sd, NetLink* pLink){
	CSingleLock lock(&m_csMap, true);
	return m_mapLinks.insert(MapValue(sd, pLink)).second;
}

bool LinkMap::Remove(SOCKET sd){
	CSingleLock lock(&m_csMap, true);
	MapIter iter = m_mapLinks.find(sd);
	if (iter == m_mapLinks.end())
	{
		return false;
	}
	lock.Unlock();
	delete iter->second;
	return m_mapLinks.erase(sd) ? true : false;
}

void LinkMap::WaitAllExit(){
	CSingleLock lock(&m_csMap, true);
	size_t nHandle = m_mapLinks.size() * 2;
	HANDLE* pHandles = new HANDLE[nHandle];
	int ii = -1;
	for (MapIter iter = m_mapLinks.begin();
		iter != m_mapLinks.end();
		++iter)
	{
		iter->second->ExitNotify();
		pHandles[++ii] = iter->second->m_hRecvThread;
		pHandles[++ii] = iter->second->m_hSendThread; 
	}
	lock.Unlock();
	WaitForMultipleObjects(nHandle, pHandles, true, INFINITE);
}

bool LinkMap::QueueMsg(int type, long long seq, const google::protobuf::Message& msg)
{
	CSingleLock lock(&m_csMap, true);
	bool bRet = false;
	for (size_t ii = 0, ni = m_mapLinks.size();
		ii < ni; ++ii)
	{
		bRet |= m_mapLinks[ii]->QueueMsg(type, seq, msg);
	}
	return bRet;
}

void LinkMap::ClearLink(void)
{
	CSingleLock lock(&m_csMap, true);
	Map links = m_mapLinks;
	m_mapLinks.clear();
	lock.Unlock();
	ClearLinks(links);
}

void LinkMap::ClearLinks(Map& links)
{
	if (!links.size())
	{
		return;
	}
	for (MapIter iter = links.begin();
		iter != links.end();
		++iter)
	{
		iter->second->ExitNotify();
	}
	for (MapIter iter = links.begin();
		iter != links.end();
		++iter)
	{
		delete iter->second;
	}
	links.clear();
}

bool SingleLink::Reset(NetLink* pLink)
{
	CSingleLock lock(&m_csMap, true);
	ClearLink();
	return m_mapLinks.insert(MapValue(INVALID_SOCKET, pLink)).second;
}


bool SingleLink::QueueMsg(int type, long long seq, const google::protobuf::Message& msg)
{
	CSingleLock lock(&m_csMap, true);
	if (m_mapLinks.size() != 1)
	{
		return false;
	}
	return m_mapLinks.begin()->second->QueueMsg(type, seq, msg);
}


} // namespace NetLayer

