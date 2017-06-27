#include "NetLink.h"

NetLink::NetLink(NetBase* pBase, const LinkInfo link, bool autoDel)
: pLayer(pBase)
, linkInfo(link)
, autoDelete(autoDel)
, pRecvTask(NULL)
, pSendTask(NULL)
{
}

NetLink::~NetLink(void)
{
}


bool NetLink::Init(void)
{
	// check pTask
	// start task
	// set pTask
	// clear pTask in task
	return false;
}
