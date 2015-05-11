#include "stdafx.h"
#include "Exception.h"
#include "EduServer_IOCP.h"
#include "OverlappedIOContext.h"
#include "ServerSession.h"
#include "IocpManager.h"

#define SERVER_BUFSIZE	(65536*4)

ServerSession::ServerSession(const char* serverAddr, unsigned short port)
: Session(SERVER_BUFSIZE, SERVER_BUFSIZE), mPort(port), mServerAddr(serverAddr)
{
	
}

ServerSession::~ServerSession()
{
}


bool ServerSession::ConnectRequest()
{
	if (mConnected)
	{
		/// 이미 접속 된 상태인데 이쪽으로 들어오면 잘못된 것
		CRASH_ASSERT(false);
		return false;
	}

	// Set up our socket address structure
	SOCKADDR_IN serverSockAddr;
	ZeroMemory(&serverSockAddr, sizeof(serverSockAddr));
	serverSockAddr.sin_port = htons(mPort);
	serverSockAddr.sin_family = AF_INET;
	serverSockAddr.sin_addr.s_addr = inet_addr(mServerAddr);

	if (SOCKET_ERROR == bind(mSocket, (SOCKADDR*)&serverSockAddr, sizeof(serverSockAddr)))
	{
		printf_s("ServerSession::ConnectRequest() bind error: %d\n", GetLastError());
		return false;
	}

	HANDLE handle = CreateIoCompletionPort((HANDLE)mSocket, GIocpManager->GetComletionPort(), (ULONG_PTR)this, 0);
	if (handle != GIocpManager->GetComletionPort())
	{
		printf_s("ServerSession::ConnectRequest() CreateIoCompletionPort error: %d\n", GetLastError());
		return false;
	}


	OverlappedConnectContext* context = new OverlappedConnectContext(this);

	if (FALSE == ConnectEx(mSocket, (sockaddr*)&serverSockAddr, sizeof(SOCKADDR_IN), NULL, 0, NULL, (LPWSAOVERLAPPED)context))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			DeleteIoContext(context);
			printf_s("ServerSession::ConnectRequest Error : %d\n", GetLastError());
		}
	}

	return true;
}

void ServerSession::ConnectCompletion()
{
	if ( SOCKET_ERROR == setsockopt(mSocket, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, NULL, 0) )
	{
		DWORD errCode = GetLastError();

		if ( WSAENOTCONN == errCode )
			printf_s("Connecting a server failed: maybe WSAENOTCONN??\n");
		else
			printf_s("SO_UPDATE_CONNECT_CONTEXT failed: %d\n", errCode);

		return;
	}

	int opt = 1;
	if (SOCKET_ERROR == setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(int)))
	{
		printf_s("[DEBUG] TCP_NODELAY error: %d\n", GetLastError());
		CRASH_ASSERT(false);
		return;
	}

	opt = 0;
	if (SOCKET_ERROR == setsockopt(mSocket, SOL_SOCKET, SO_RCVBUF, (const char*)&opt, sizeof(int)))
	{
		printf_s("[DEBUG] SO_RCVBUF change error: %d\n", GetLastError());
		CRASH_ASSERT(false);
		return;
	}


	if (1 == InterlockedExchange(&mConnected, 1))
	{
		CRASH_ASSERT(false);
	}

	if (false == PreRecv())
	{
		printf_s("[DEBUG] PreRecv for Server Connection error: %d\n", GetLastError());
		InterlockedExchange(&mConnected, 0);
		return;
	}

}


