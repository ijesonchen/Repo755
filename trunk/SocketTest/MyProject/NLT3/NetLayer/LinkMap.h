///////////////////////////////////////////////////////////
//  LinkMap.h
//  Implementation of the Class LinkMap
//  Created on:      28-¾ÅÔÂ-2015 9:20:34
//  Original author: user
///////////////////////////////////////////////////////////

#if !defined(EA_0DCCED86_3614_448d_A70B_F5321F599B2F__INCLUDED_)
#define EA_0DCCED86_3614_448d_A70B_F5321F599B2F__INCLUDED_

#include <map>
#include "NetLayer.h"

namespace NetLayer
{


class NetLink;

class LinkMap
{
public:
	virtual ~LinkMap();

	bool Capture(SOCKET sd, NetLink* pLink);
	bool Remove(SOCKET sd);
	void WaitAllExit();
	void ClearLink(void);

	bool QueueMsg(int type, long long seq, const google::protobuf::Message& msg);

protected:
	typedef std::map<SOCKET, NetLink*> Map;
	typedef std::map<SOCKET, NetLink*>::value_type MapValue;
	typedef std::map<SOCKET, NetLink*>::iterator MapIter;

	Map m_mapLinks;
	CCriticalSection m_csMap;

	static void ClearLinks(Map& links);
};

class SingleLink : protected LinkMap
{
public:
	bool QueueMsg(int type, long long seq, const google::protobuf::Message& msg);
	bool Reset(NetLink* pLink);
	using LinkMap::ClearLink;
};



} // namespace NetLayer


#endif // !defined(EA_0DCCED86_3614_448d_A70B_F5321F599B2F__INCLUDED_)
