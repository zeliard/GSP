#pragma once

#define BUFSIZE	4096

class ClientSession ;
class SessionManager;

enum IOType
{
	IO_NONE,
	IO_SEND,
	IO_RECV,
	IO_RECV_ZERO,
	IO_ACCEPT
} ;

enum DisconnectReason
{
	DR_NONE,
	DR_RECV_ZERO,
	DR_ACTIVE,
	DR_ONCONNECT_ERROR,
	DR_COMPLETION_ERROR,
};

struct OverlappedIOContext
{
	OverlappedIOContext(const ClientSession* owner, IOType ioType) : mSessionObject(owner), mIoType(ioType)
	{
		memset(&mOverlapped, 0, sizeof(OVERLAPPED));
		memset(&mWsaBuf, 0, sizeof(WSABUF));
		memset(mBuffer, 0, BUFSIZE);
	}

	OVERLAPPED				mOverlapped ;
	const ClientSession*	mSessionObject ;
	IOType			mIoType ;
	WSABUF			mWsaBuf;
	char			mBuffer[BUFSIZE];
} ;


class ClientSession
{
public:
	ClientSession(SOCKET sock) 
		: mSocket(sock), mConnected(false)
	{
		memset(&mClientAddr, 0, sizeof(SOCKADDR_IN)) ;
	}

	~ClientSession() {}

	bool	OnConnect(SOCKADDR_IN* addr);
	bool	IsConnected() const { return mConnected; }

	bool	PostRecv() const ;
	bool	PostSend(const char* buf, int len) const ;
	void	Disconnect(DisconnectReason dr);
	

private:
	bool			mConnected ;
	SOCKET			mSocket ;

	SOCKADDR_IN		mClientAddr ;
		
	//TODO: mLock; 선언할 것

	friend class SessionManager;
} ;




