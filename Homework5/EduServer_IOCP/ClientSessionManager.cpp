#include "stdafx.h"
#include "FastSpinlock.h"
#include "EduServer_IOCP.h"
#include "ClientSession.h"
#include "ClientSessionManager.h"
#include "IocpManager.h"


ClientSessionManager* GClientSessionManager = nullptr;

ClientSessionManager::~ClientSessionManager()
{
	for (auto it : mFreeSessionList)
	{
		delete it;
	}
}

void ClientSessionManager::PrepareClientSessions()
{
	CRASH_ASSERT(LThreadType == THREAD_MAIN);

	for (int i = 0; i < MAX_CONNECTION; ++i)
	{
		ClientSession* client = new ClientSession();
			
		mFreeSessionList.push_back(client);
	}
}





void ClientSessionManager::ReturnClientSession(ClientSession* client)
{
	FastSpinlockGuard guard(mLock);

	CRASH_ASSERT(client->mConnected == 0 && client->mRefCount == 0);

	client->SessionReset();

	mFreeSessionList.push_back(client);

	++mCurrentReturnCount;
}

bool ClientSessionManager::AcceptClientSessions()
{
	FastSpinlockGuard guard(mLock);

	while (mCurrentIssueCount - mCurrentReturnCount < MAX_CONNECTION)
	{
		ClientSession* newClient = mFreeSessionList.back();
		mFreeSessionList.pop_back();

		++mCurrentIssueCount;

		newClient->AddRef(); ///< refcount +1 for issuing 
		
		if (false == newClient->PostAccept())
			return false;
	}


	return true;
}