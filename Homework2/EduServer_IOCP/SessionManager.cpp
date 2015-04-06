#include "stdafx.h"
#include "FastSpinlock.h"
#include "EduServer_IOCP.h"
#include "ClientSession.h"
#include "SessionManager.h"
#include "IocpManager.h"


SessionManager* GSessionManager = nullptr;

SessionManager::~SessionManager()
{
	for (auto it : mFreeSessionList)
	{
		delete it;
	}
}

void SessionManager::PrepareSessions()
{
	CRASH_ASSERT(LThreadType == THREAD_MAIN);

	for (int i = 0; i < MAX_CONNECTION; ++i)
	{
		ClientSession* client = new ClientSession();
			
		mFreeSessionList.push_back(client);
	}
}





void SessionManager::ReturnClientSession(ClientSession* client)
{
	FastSpinlockGuard guard(mLock);

	CRASH_ASSERT(client->mConnected == 0 && client->mRefCount == 0);

	client->SessionReset();

	mFreeSessionList.push_back(client);

	++mCurrentReturnCount;
}

bool SessionManager::AcceptSessions()
{
	FastSpinlockGuard guard(mLock);

	while (mCurrentIssueCount - mCurrentReturnCount < MAX_CONNECTION)
	{
		//TODO mFreeSessionList에서 ClientSession* 꺼내서 PostAccept() 해주기.. (위의 ReturnClientSession와 뭔가 반대로 하면 될 듯?)
		
		// AddRef()도 당연히 해줘야 하고...

		// 실패시 false
		//if (false == newClient->PostAccept())
		//	return false;
	}


	return true;
}