#ifndef NetLink_h__
#define NetLink_h__

#include "NetLayer.h"

class NetBase;
class NetLinkTask;
class NetSendTask;
class NetLink
{
public:
	NetLink(NetBase* pBase, const LinkInfo link, bool autoDel);
	~NetLink(void);

	bool Init(void);
private:
	friend class NetLinkTask;
	friend class NetSendTask;

	NetBase* pLayer;
	LinkInfo linkInfo;
	bool autoDelete;
	NetLinkTask* pRecvTask;
	NetSendTask* pSendTask;
};

#endif // NetLink_h__
