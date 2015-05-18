#include "stdafx.h"
#include "FastSpinlock.h"
#include "DummyClients.h"
#include "DummyClientSession.h"
#include "SessionManager.h"
#include "IocpManager.h"


SessionManager* GSessionManager = nullptr;

SessionManager::~SessionManager()
{
	for (auto it : mReadySessionList)
	{
		delete it;
	}
}

bool SessionManager::PrepareClientSessions()
{
	CRASH_ASSERT(LThreadType == THREAD_MAIN);

	for (int i = 0; i < MAX_CONNECTION; ++i)
	{
		DummyClientSession* client = new DummyClientSession();

		if (false == client->PrepareSession())
			return false;
			
		mReadySessionList.push_back(client);
	}

	return true;
}



void SessionManager::DecreaseClientSession()
{
	InterlockedIncrement(&mCurrentReturnCount);
}

int SessionManager::IncreaseClientSession()
{
	return InterlockedIncrement(&mCurrentConnectCount);
}

bool SessionManager::ConnectSessions()
{
	FastSpinlockGuard guard(mLock);

	for (auto session : mReadySessionList)
	{
		session->AddRef();
		if (false == session->ConnectRequest())
			return false;
	
		Sleep(10);
	}

	return true;
}

void SessionManager::DisconnectSessions()
{
	FastSpinlockGuard guard(mLock);

	for (auto session : mReadySessionList)
	{
		if (session->IsConnected())
			session->DisconnectRequest(DR_ACTIVE);
	}
}

void SessionManager::PrintTotalTransferred()
{
	FastSpinlockGuard guard(mLock);

	for (auto session : mReadySessionList)
	{
		mTotalSessionSend += session->GetSendTotal();
		mTotalSessionRecv += session->GetRecvTotal();
	}

	printf_s("Session Total, SEND=%I64d, RECV=%I64d\n", mTotalSessionSend, mTotalSessionRecv);
}