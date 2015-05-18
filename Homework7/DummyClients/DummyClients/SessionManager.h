#pragma once
#include <list>
#include <WinSock2.h>
#include "FastSpinlock.h"

class DummyClientSession;

class SessionManager
{
public:
	SessionManager() : mCurrentConnectCount(0), mCurrentReturnCount(0)
	{}
	
	~SessionManager();

	bool PrepareClientSessions();

	void DecreaseClientSession();
	int IncreaseClientSession();

	bool ConnectSessions();
	void DisconnectSessions();

	void PrintTotalTransferred();

private:
	typedef std::list<DummyClientSession*> ClientList;
	ClientList	mReadySessionList;

	FastSpinlock mLock;

	long mCurrentConnectCount;
	long mCurrentReturnCount;

	int64_t mTotalSessionSend = 0;
	int64_t mTotalSessionRecv = 0;
	
};

extern SessionManager* GSessionManager;
