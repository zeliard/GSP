#pragma once
#include "ThreadLocal.h"
#include "EduServer_IOCP.h"

class Session;
class IOThread;

struct OverlappedSendContext;
struct OverlappedPreRecvContext;
struct OverlappedRecvContext;
struct OverlappedDisconnectContext;
struct OverlappedAcceptContext;

struct DatabaseJobContext;

class IocpManager
{
public:
	IocpManager();
	~IocpManager();

	bool Initialize();
	void Finalize();

	bool StartIoThreads();
	void StartAccept();

	void PostDatabaseResult(DatabaseJobContext* dbContext);

	HANDLE GetComletionPort()	{ return mCompletionPort; }

	SOCKET* GetListenSocket()  { return &mListenSocket;  }

	static char mAcceptBuf[64];
	static LPFN_DISCONNECTEX mFnDisconnectEx;
	static LPFN_ACCEPTEX mFnAcceptEx;
	static LPFN_CONNECTEX mFnConnectEx;

private:

	static unsigned int WINAPI IoWorkerThread(LPVOID lpParam);

private:

	HANDLE	mCompletionPort;
	SOCKET	mListenSocket;

	IOThread* mIoWorkerThread[MAX_IO_THREAD];
	//std::vector<
};



extern IocpManager* GIocpManager;


BOOL DisconnectEx(SOCKET hSocket, LPOVERLAPPED lpOverlapped, DWORD dwFlags, DWORD reserved);

BOOL AcceptEx(SOCKET sListenSocket, SOCKET sAcceptSocket, PVOID lpOutputBuffer, DWORD dwReceiveDataLength,
	DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength, LPDWORD lpdwBytesReceived, LPOVERLAPPED lpOverlapped);

BOOL ConnectEx(SOCKET hSocket, const struct sockaddr* name, int namelen, PVOID lpSendBuffer, DWORD dwSendDataLength,
	LPDWORD lpdwBytesSent, LPOVERLAPPED lpOverlapped);