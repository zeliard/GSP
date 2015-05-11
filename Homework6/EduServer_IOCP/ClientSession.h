#pragma once

#include "Session.h"
#include "Player.h"

class ClientSessionManager;


class ClientSession : public Session, public ObjectPool<ClientSession>
{
public:
	ClientSession();
	virtual ~ClientSession();

	void SessionReset();

	bool PostAccept();
	void AcceptCompletion();
	
	virtual void OnDisconnect(DisconnectReason dr);
	virtual void OnRelease();

public:
	Player			mPlayer;

private:
	
	SOCKADDR_IN		mClientAddr ;

	
	friend class ClientSessionManager;
} ;



