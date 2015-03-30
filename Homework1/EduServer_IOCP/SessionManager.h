#pragma once
#include <map>
#include <WinSock2.h>
#include "FastSpinlock.h"

class ClientSession;

class SessionManager
{
public:
	SessionManager() : mCurrentConnectionCount(0)	{}

	ClientSession* CreateClientSession(SOCKET sock);

	void DeleteClientSession(ClientSession* client);

	int IncreaseConnectionCount() { return InterlockedIncrement(&mCurrentConnectionCount); }
	int DecreaseConnectionCount() { return InterlockedDecrement(&mCurrentConnectionCount); }


private:
	typedef std::map<SOCKET, ClientSession*> ClientList;
	ClientList	mClientList;

	//TODO: mLock; ¼±¾ð

	volatile long mCurrentConnectionCount;
};

extern SessionManager* GSessionManager;
