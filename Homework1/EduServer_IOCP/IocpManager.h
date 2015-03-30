#pragma once

class ClientSession;
struct OverlappedIOContext;

class IocpManager
{
public:
	IocpManager();
	~IocpManager();

	bool Initialize();
	void Finalize();

	bool StartIoThreads();
	bool StartAcceptLoop();

	HANDLE GetComletionPort()	{ return mCompletionPort; }
	int	GetIoThreadCount()		{ return mIoThreadCount;  }


private:

	static unsigned int WINAPI IoWorkerThread(LPVOID lpParam);

	static bool ReceiveCompletion(const ClientSession* client, OverlappedIOContext* context, DWORD dwTransferred);
	static bool SendCompletion(const ClientSession* client, OverlappedIOContext* context, DWORD dwTransferred);

private:

	HANDLE	mCompletionPort;
	int		mIoThreadCount;

	SOCKET	mListenSocket;



};

extern __declspec(thread) int LIoThreadId;
extern IocpManager* GIocpManager;