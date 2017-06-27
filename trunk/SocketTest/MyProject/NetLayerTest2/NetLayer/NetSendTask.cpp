#include "NetSendTask.h"
#include "NetLink.h"

NetSendTask::NetSendTask(NetLinkTask* pvLink, SOCKET s)
: pLink(pvLink)
, sd(s)
{
}

void NetSendTask::DoTask(void* , OVERLAPPED* )
{

}

NetSendTask::~NetSendTask(void)
{
}
