#pragma once


class WorkerThread
{
public:
	WorkerThread(int workerThreadId, HANDLE hThread, HANDLE hCompletionPort);
	~WorkerThread();

	DWORD Run();

	void DoIocpJob();
	void DoSendJob();

	HANDLE GetHandle() { return mThreadHandle;  }
	
private:

	HANDLE mThreadHandle;
	HANDLE mCompletionPort;
	int mWorkerThreadId;
};

