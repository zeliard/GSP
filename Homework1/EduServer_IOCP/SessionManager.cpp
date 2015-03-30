#include "stdafx.h"
#include "ClientSession.h"
#include "SessionManager.h"

SessionManager* GSessionManager = nullptr;

ClientSession* SessionManager::CreateClientSession(SOCKET sock)
{
	ClientSession* client = new ClientSession(sock);

	//TODO: lock으로 보호할 것
	{
		mClientList.insert(ClientList::value_type(sock, client));
	}

	return client;
}


void SessionManager::DeleteClientSession(ClientSession* client)
{
	//TODO: lock으로 보호할 것
	{
		mClientList.erase(client->mSocket);
	}
	
	delete client;
}