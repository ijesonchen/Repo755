for network connection
support both long & short tcp connection

*********************
improve:
combine some NetLayer & LayerBase functions, with default parameter MsgQueue& msgQueue = g_msgQueue
*********************


USER CODE:

for LONG connection:
1. derive user-defined UserLink class from NetLink
	OnMsgRecved to proc received message
		use NetLinkTask derived task to proc complex message.
	GetSendSocket to retrive SOCKET to send data
		(e.g. sdRecv = connect peer for P2P long connection if not invalid)
	If necessary, overwrite RecvCheck 
		to check before recv one message (e.g heart-beat), 
		and return false to cause NetLink removed.
	if necessary, overwrite OnNoDataSend 
		(e.g queue heart-beat msg when no send traffic for a certain time.)
		
	recv & send thread will start when Init (link base class)
	
2. derive user-defined UserLayer class from NetLayer
	use SingleLink / LinkMap to manage links
	
	InitLayer (e.g. check parameter)
	OnConnected: new link, init and captured by linkmap
	WaitLayerExit: clear linkmap
	
	interface to send msg to link:
		QueueMsg (single-link mode)
		QueueMsg2AllLink (multi-link mode)
		
	
for SHORT connection
	no NetLink, task-oriented.

	InitLayer: eg, check parameter, alloc mem, start SendThread threads
	OnConnected: recv msg, 
		OnShortMsgRecved proc msg with NetLinkTask derived task 
	WaitLayerExit: wait user thread to exit.
	
for both Layer, connect thread will start when Init (layer base class)
	
HOT TO:

	// optional
	NetLayer::g_msgQueue.SetPrefix(L"GlobalNetLayerMsg");

	NetLayer* pLayer = new NetLayer();

	if (!pLayer->Init())
	{
		// init failed.
		return;
	}
	
	proc recved msg
	// long conn: UserLink: OnMsgRecved
	// short conn: OnConnected -> OnShortMsgRecved -> UserNetLinkTask
	
	// queue msg to send.
	pLayer->QueueMsg(nMsgType, nMsgSeqNo, ProtoBufMsg);


	// ExitNotify & ExitWait is optional
	// will call automatically when delete layer
	
	// notify thread to exit
	// links will cleard when all link thread exit
	pLayer->ExitNotify();
	
	// wait thread exit & all links cleard
	pLayer->ExitWait();
	
	delete pLayer;


MODEL:
user specify:
short/long conn?
is server?	
is P2P?

for long connection, P2P mode suggested.

for short connection, client mode is very simple, no framework needed.


HOW IT WORKS:
	see LayerBase & NetLink header