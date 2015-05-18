#pragma once
#include "ThreadLocal.h"
#include "DummyClients.h"

class Session;
class WorkerThread;

struct OverlappedSendContext;
struct OverlappedPreRecvContext;
struct OverlappedRecvContext;
struct OverlappedDisconnectContext;

enum COMPLETION_KEY 
{
	CK_NONE,
	CK_STOP_NOW = 0xDEAD
};


class IocpManager
{
public:
	IocpManager();
	~IocpManager();

	bool Initialize();
	void Finalize();

	bool StartIoThreads();
	void StopIoThreads();

	HANDLE GetComletionPort()	{ return mCompletionPort; }
	int	GetIoThreadCount()		{ return mIoThreadCount;  }

	static LPFN_DISCONNECTEX mFnDisconnectEx;
	static LPFN_CONNECTEX mFnConnectEx;

private:

	static unsigned int WINAPI IoWorkerThread(LPVOID lpParam);

private:

	HANDLE	mCompletionPort;
	int		mIoThreadCount;


	WorkerThread* mWorkerThread[MAX_IOTHREAD];
};


extern IocpManager* GIocpManager;

BOOL DisconnectEx(SOCKET hSocket, LPOVERLAPPED lpOverlapped, DWORD dwFlags, DWORD reserved);

BOOL ConnectEx(SOCKET hSocket, const struct sockaddr* name, int namelen, PVOID lpSendBuffer, DWORD dwSendDataLength,
	LPDWORD lpdwBytesSent, LPOVERLAPPED lpOverlapped);