#include "stdafx.h"
#include "Exception.h"
#include "ThreadLocal.h"
#include "EduServer_IOCP.h"
#include "WorkerThread.h"
#include "ClientSession.h"
#include "ServerSession.h"
#include "IocpManager.h"


WorkerThread::WorkerThread(int workerThreadId, HANDLE hThread, HANDLE hCompletionPort)
: mWorkerThreadId(workerThreadId), mThreadHandle(hThread), mCompletionPort(hCompletionPort)
{
}


WorkerThread::~WorkerThread()
{
	CloseHandle(mThreadHandle);
}

DWORD WorkerThread::Run()
{

	while (true)
	{
		DoIocpJob();

		DoSendJob(); ///< aggregated sends

		//... ...
	}

	return 1;
}

void WorkerThread::DoIocpJob()
{

	DWORD dwTransferred = 0;
	OverlappedIOContext* context = nullptr;
	ULONG_PTR completionKey = 0;

	int ret = GetQueuedCompletionStatus(mCompletionPort, &dwTransferred, (PULONG_PTR)&completionKey, (LPOVERLAPPED*)&context, GQCS_TIMEOUT);

	Session* remote = context ? context->mSessionObject : nullptr;

	if (ret == 0 || dwTransferred == 0)
	{
		/// check time out first 
		if ( context == nullptr && GetLastError() == WAIT_TIMEOUT)
			return;

		if (context->mIoType == IO_RECV || context->mIoType == IO_SEND)
		{
			CRASH_ASSERT(nullptr != remote);

			/// In most cases in here: ERROR_NETNAME_DELETED(64)

			remote->DisconnectRequest(DR_COMPLETION_ERROR);

			DeleteIoContext(context);

			return;
		}
	}

	CRASH_ASSERT(nullptr != remote);

	bool completionOk = false;
	switch (context->mIoType)
	{
	case IO_CONNECT:
		dynamic_cast<ServerSession*>(remote)->ConnectCompletion();
		completionOk = true;
		break;

	case IO_DISCONNECT:
		remote->DisconnectCompletion(static_cast<OverlappedDisconnectContext*>(context)->mDisconnectReason);
		completionOk = true;
		break;

	case IO_ACCEPT:
		dynamic_cast<ClientSession*>(remote)->AcceptCompletion();
		completionOk = true;
		break;

	case IO_RECV_ZERO:
		completionOk = remote->PostRecv();
		break;

	case IO_SEND:
		remote->SendCompletion(dwTransferred);

		if (context->mWsaBuf.len != dwTransferred)
			printf_s("Partial SendCompletion requested [%d], sent [%d]\n", context->mWsaBuf.len, dwTransferred);
		else
			completionOk = true;
		
		break;

	case IO_RECV:
		remote->RecvCompletion(dwTransferred);
	
		/// for test
		remote->EchoBack();
		
		completionOk = remote->PreRecv();

		break;

	default:
		printf_s("Unknown I/O Type: %d\n", context->mIoType);
		CRASH_ASSERT(false);
		break;
	}

	if (!completionOk)
	{
		/// connection closing
		remote->DisconnectRequest(DR_IO_REQUEST_ERROR);
	}

	DeleteIoContext(context);
	
}


void WorkerThread::DoSendJob()
{
	while (!LSendRequestSessionList->empty())
	{
		auto& session = LSendRequestSessionList->front();
	
		if (session->FlushSend())
		{
			/// true 리턴 되면 빼버린다.
			LSendRequestSessionList->pop_front();
		}
	}
	
}

