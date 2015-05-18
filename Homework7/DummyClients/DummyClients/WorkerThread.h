#pragma once


class WorkerThread
{
public:
	WorkerThread(int workerThreadId, HANDLE hThread, HANDLE hCompletionPort);
	~WorkerThread();

	DWORD Run();

	bool DoIocpJob(); ///< false리턴 되면 종료를 의미
	void DoSendJob();

	HANDLE GetHandle() { return mThreadHandle;  }
	
private:

	HANDLE mThreadHandle;
	HANDLE mCompletionPort;
	int mWorkerThreadId;
};

