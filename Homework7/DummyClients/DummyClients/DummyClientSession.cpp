#include "stdafx.h"
#include "Exception.h"
#include "Log.h"
#include "DummyClients.h"
#include "OverlappedIOContext.h"
#include "DummyClientSession.h"
#include "IocpManager.h"
#include "SessionManager.h"

#include "PacketInterface.h"
#include "MyPacket.pb.h"

#define CLIENT_BUFSIZE	65536

DummyClientSession::DummyClientSession() : Session(CLIENT_BUFSIZE, CLIENT_BUFSIZE)
{
}

DummyClientSession::~DummyClientSession()
{
}

bool DummyClientSession::PrepareSession()
{
	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = 0;

 	if (SOCKET_ERROR == bind(mSocket, (SOCKADDR*)&addr, sizeof(addr)))
 	{
 		printf_s("DummyClientSession::PrepareSession() bind error: %d\n", GetLastError());
 		return false ;
 	}

	HANDLE handle = CreateIoCompletionPort((HANDLE)mSocket, GIocpManager->GetComletionPort(), (ULONG_PTR)this, 0);
	if (handle != GIocpManager->GetComletionPort())
	{
		printf_s("DummyClientSession::PrepareSession() CreateIoCompletionPort error: %d\n", GetLastError());
		return false;
	}

	return true;
}

bool DummyClientSession::ConnectRequest()
{
	if (mConnected)
	{
		/// 이미 접속 된 상태인데 이쪽으로 들어오면 잘못된 것
		CRASH_ASSERT(false);
		return false;
	}

	// Set up our socket address structure
	ZeroMemory(&mConnectAddr, sizeof(mConnectAddr));
	mConnectAddr.sin_port = htons(CONNECT_PORT);
	mConnectAddr.sin_family = AF_INET;
	mConnectAddr.sin_addr.s_addr = inet_addr(CONNECT_ADDR);

	OverlappedConnectContext* context = new OverlappedConnectContext(this);

	if (FALSE == ConnectEx(mSocket, (sockaddr*)&mConnectAddr, sizeof(SOCKADDR_IN), NULL, 0, NULL, (LPWSAOVERLAPPED)context))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			DeleteIoContext(context);
			printf_s("DummyClientSession::ConnectRequest Error : %d\n", GetLastError());
		}
	}

	return true;
}

void DummyClientSession::ConnectCompletion()
{
	if (SOCKET_ERROR == setsockopt(mSocket, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, NULL, 0))
	{
		DWORD errCode = GetLastError();

		if (WSAENOTCONN == errCode)
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

	
	int curr = GSessionManager->IncreaseClientSession();
	printf_s("[DEBUG:%d] Session established: IP=%s, PORT=%d \n", curr, inet_ntoa(mConnectAddr.sin_addr), ntohs(mConnectAddr.sin_port));

	Login();
}


void DummyClientSession::OnRelease()
{
	GSessionManager->DecreaseClientSession();
}

bool DummyClientSession::SendRequest(short packetType, const protobuf::MessageLite& payload)
{
	if (!IsConnected())
		return false;

	FastSpinlockGuard criticalSection(mSendBufferLock);

	int totalSize = payload.ByteSize() + HEADER_SIZE;
	if (mSendBuffer.GetFreeSpaceSize() < totalSize)
		return false;

	protobuf::io::ArrayOutputStream arrayOutputStream(mSendBuffer.GetBuffer(), totalSize);
	protobuf::io::CodedOutputStream codedOutputStream(&arrayOutputStream);

	PacketHeader header;
	header.mSize = payload.ByteSize();
	header.mType = packetType;

	codedOutputStream.WriteRaw(&header, HEADER_SIZE);
	payload.SerializeToCodedStream(&codedOutputStream);


	/// flush later...
	LSendRequestSessionList->push_back(this);

	mSendBuffer.Commit(totalSize);

	return true;
}

void DummyClientSession::Login()
{
	static int id = 121;

	MyPacket::LoginRequest loginRequest;
	loginRequest.set_playerid(id++);

	SendRequest(MyPacket::PKT_CS_LOGIN, loginRequest);

}