#pragma once

class DBThread
{
public:
	DBThread(HANDLE hThread, HANDLE hCompletionPort);
	~DBThread();

	DWORD Run();
	
	void DoDatabaseJob();

	HANDLE GetHandle() { return mDbThreadHandle; }

private:
	HANDLE mDbThreadHandle;
	HANDLE mDbCompletionPort;
};