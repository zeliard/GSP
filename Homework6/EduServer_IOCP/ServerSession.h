#pragma once

#include "Session.h"

/// 다른 서버로의 접속을 위한 세션
class ServerSession : public Session, public ObjectPool<ServerSession>
{
public:
	ServerSession(const char* serverAddr, unsigned short port);
	virtual ~ServerSession();

	bool ConnectRequest();
	void ConnectCompletion();


private:
	const char*		mServerAddr;
	unsigned short	mPort;

};