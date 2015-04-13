#pragma once
//#include <WinSock2.h>
#include "XTL.h"
#include "FastSpinlock.h"

class ClientSession;

class SessionManager
{
public:
	SessionManager() : mCurrentIssueCount(0), mCurrentReturnCount(0)
	{}
	
	~SessionManager();

	void PrepareSessions();
	bool AcceptSessions();

	void ReturnClientSession(ClientSession* client);

	

private:
	typedef xlist<ClientSession*>::type ClientList;
	ClientList	mFreeSessionList;

	FastSpinlock mLock;

	uint64_t mCurrentIssueCount;
	uint64_t mCurrentReturnCount;
};

extern SessionManager* GSessionManager;
