#include "stdafx.h"
#include "Exception.h"
#include "ThreadLocal.h"
#include "Log.h"
#include "IocpManager.h"
#include "DummyClients.h"
#include "DummyClientSession.h"
#include "WorkerThread.h"
#include "SessionManager.h"


IocpManager* GIocpManager = nullptr;

LPFN_DISCONNECTEX IocpManager::mFnDisconnectEx = nullptr;
LPFN_CONNECTEX IocpManager::mFnConnectEx = nullptr;


BOOL DisconnectEx(SOCKET hSocket, LPOVERLAPPED lpOverlapped, DWORD dwFlags, DWORD reserved)
{
	return IocpManager::mFnDisconnectEx(hSocket, lpOverlapped, dwFlags, reserved);
}


BOOL ConnectEx(SOCKET hSocket, const struct sockaddr* name, int namelen, PVOID lpSendBuffer, DWORD dwSendDataLength, LPDWORD lpdwBytesSent, LPOVERLAPPED lpOverlapped)
{
	return IocpManager::mFnConnectEx(hSocket, name, namelen, lpSendBuffer, dwSendDataLength, lpdwBytesSent, lpOverlapped);
}

IocpManager::IocpManager() : mCompletionPort(NULL), mIoThreadCount(2)
{
	memset(mWorkerThread, 0, sizeof(mWorkerThread));
}


IocpManager::~IocpManager()
{
}

bool IocpManager::Initialize()
{
	/// set num of I/O threads
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	mIoThreadCount = min(si.dwNumberOfProcessors, MAX_IOTHREAD);

	/// winsock initializing
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;

	/// Create I/O Completion Port
	mCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (mCompletionPort == NULL)
		return false;

	/// create TCP socket
	SOCKET sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (sock == INVALID_SOCKET)
		return false;

	HANDLE handle = CreateIoCompletionPort((HANDLE)sock, mCompletionPort, 0, 0);
	if (handle != mCompletionPort)
	{
		printf_s("[DEBUG] listen socket IOCP register error: %d\n", GetLastError());
		return false;
	}

	int opt = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(int));

	GUID guidDisconnectEx = WSAID_DISCONNECTEX ;
	DWORD bytes = 0 ;
	if (SOCKET_ERROR == WSAIoctl(sock, SIO_GET_EXTENSION_FUNCTION_POINTER, 
		&guidDisconnectEx, sizeof(GUID), &mFnDisconnectEx, sizeof(LPFN_DISCONNECTEX), &bytes, NULL, NULL) )
		return false;

	GUID guidConnectEx = WSAID_CONNECTEX;
	if (SOCKET_ERROR == WSAIoctl(sock, SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guidConnectEx, sizeof(GUID), &mFnConnectEx, sizeof(LPFN_CONNECTEX), &bytes, NULL, NULL))
		return false;

	/// make session pool
	return GSessionManager->PrepareClientSessions();
}


bool IocpManager::StartIoThreads()
{
	/// create I/O Thread
	for (int i = 0; i < mIoThreadCount; ++i)
	{
		
		DWORD dwThreadId;
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, IoWorkerThread, (LPVOID)i, CREATE_SUSPENDED, (unsigned int*)&dwThreadId);
		if (hThread == NULL)
			return false;

		mWorkerThread[i] = new WorkerThread(i, hThread, mCompletionPort);
	}

	/// start!
	for (int i = 0; i < mIoThreadCount; ++i)
	{
		ResumeThread(mWorkerThread[i]->GetHandle());
	}

	return true;
}

void IocpManager::StopIoThreads()
{
	HANDLE hThread[MAX_IOTHREAD] = { NULL, };

	for (int i = 0; i < mIoThreadCount; ++i)
	{
		if (FALSE == PostQueuedCompletionStatus(mCompletionPort, 0, (ULONG_PTR)CK_STOP_NOW, NULL))
		{
			printf_s("PostQueuedCompletionStatus Error: %d\n", GetLastError());
		}

		hThread[i] = mWorkerThread[i]->GetHandle();
	}

	/// 스레드 종료까지 모두 기다린다
	WaitForMultipleObjects(mIoThreadCount, hThread, TRUE, INFINITE);
}





void IocpManager::Finalize()
{

	for (int i = 0; i < mIoThreadCount; ++i)
	{
		CloseHandle(mWorkerThread[i]->GetHandle());
	}

	CloseHandle(mCompletionPort);

	/// winsock finalizing
	WSACleanup();

}

unsigned int WINAPI IocpManager::IoWorkerThread(LPVOID lpParam)
{
	LThreadType = THREAD_IO_WORKER;
	LWorkerThreadId = reinterpret_cast<int>(lpParam);
	LSendRequestSessionList = new std::deque<Session*>;
	GThreadCallHistory[LWorkerThreadId] = LThreadCallHistory = new ThreadCallHistory(LWorkerThreadId);
	
	return GIocpManager->mWorkerThread[LWorkerThreadId]->Run();
}

