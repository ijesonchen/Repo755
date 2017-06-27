for network connection
support both long & short tcp connection

HOT TO:

	NetLayer* pLayer = new NetLayer();

	if (!pLayer->Init())
	{
		// init failed.
		return;
	}
	
	proc recved msg in OnRecvMsg()
	// long conn: RecvThread
	// short conn: AcceptThread -> AcceptTask::DoTask

	// queue msg to send.
	pLayer->QueueMsg(nMsgType, nMsgSeqNo, ProtoBufMsg);

	// get status
	pLayer->IsRunning();

	// stop thread, general not required.
	// thread will stoped when delete NetLayer
	pLayer->StopThread();

	delete pLayer;

USER CODE:
1. for long connection:
derive user-defined class from NetLayerLong
OnRecvMsg to proc received message
call QueueMsg to queue user message to send

if proc received msg with TaskPool in OnRecvMsg
drive user-defined class from PbLongTask

2. for short connection:
derive user-defined class from NetLayerShort
OnConnection to proc incomming connection
OnSendMsg to proc queued message
call QueueMsg to queue user message to send

if proc connection with TaskPool
drive user-defined class from NetLayerAcceptTask
OnRecvMsg to proc received message

if proc received msg with TaskPool in OnRecvMsg
drive user-defined class from PbShortTask

HOW IT WORKS:
do task with ThreadPool<ThreadWorker>