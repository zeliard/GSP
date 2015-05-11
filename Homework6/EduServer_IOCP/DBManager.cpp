#include "stdafx.h"
#include "ThreadLocal.h"
#include "EduServer_IOCP.h"
#include "Log.h"
#include "DBContext.h"
#include "DBThread.h"
#include "DBManager.h"
#include "DBHelper.h"

DBManager* GDatabaseManager = nullptr;

DBManager::DBManager() : mDbCompletionPort(NULL)
{
	memset(mDbWorkerThread, 0, sizeof(mDbWorkerThread));
}

DBManager::~DBManager()
{

}


bool DBManager::Initialize()
{
	/// Create I/O Completion Port
	mDbCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (mDbCompletionPort == NULL)
		return false;

	if (false == DBHelper::Initialize(SQL_SERVER_CONN_STR, MAX_DB_THREAD))
		return false;

	return true;
}

void DBManager::Finalize()
{
	DBHelper::Finalize();

	for (int i = 0; i < MAX_DB_THREAD; ++i)
	{
		CloseHandle(mDbWorkerThread[i]->GetHandle());
	}

	CloseHandle(mDbCompletionPort);
}

bool DBManager::StartDatabaseThreads()
{
	/// create DB Thread
	for (int i = 0; i < MAX_DB_THREAD; ++i)
	{
		DWORD dwThreadId;
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, DbWorkerThread, (LPVOID)i, CREATE_SUSPENDED, (unsigned int*)&dwThreadId);
		if (hThread == NULL)
			return false;

		mDbWorkerThread[i] = new DBThread(hThread, mDbCompletionPort);
	}

	/// start!
	for (int i = 0; i < MAX_DB_THREAD; ++i)
	{
		ResumeThread(mDbWorkerThread[i]->GetHandle());
	}

	return true;
}


unsigned int WINAPI DBManager::DbWorkerThread(LPVOID lpParam)
{
	LThreadType = THREAD_DB_WORKER;
	LWorkerThreadId = reinterpret_cast<int>(lpParam);
	GThreadCallHistory[LWorkerThreadId] = LThreadCallHistory = new ThreadCallHistory(LWorkerThreadId);
	GThreadCallElapsedRecord[LWorkerThreadId] = LThreadCallElapsedRecord = new ThreadCallElapsedRecord(LWorkerThreadId);

	/// 반드시 DB 쓰레드가 먼저 띄우도록..
	CRASH_ASSERT(LWorkerThreadId < MAX_DB_THREAD);

	return GDatabaseManager->mDbWorkerThread[LWorkerThreadId]->Run();
}

void DBManager::PostDatabsaseRequest(DatabaseJobContext* dbContext)
{
	//todo: PQCS를 이용하여 dbContext를 mDbCompletionPort에 보내기

}
