#pragma once

#include "ThreadLocal.h"

struct DatabaseJobContext;
class DBThread;

class DBManager
{
public:
	DBManager();
	~DBManager();

	bool Initialize();
	void Finalize();

	bool StartDatabaseThreads();

	void PostDatabsaseRequest(DatabaseJobContext* dbContext);

private:
	static unsigned int WINAPI DbWorkerThread(LPVOID lpParam);

private:

	HANDLE	mDbCompletionPort;

	DBThread* mDbWorkerThread[MAX_DB_THREAD];

};

extern DBManager* GDatabaseManager;
