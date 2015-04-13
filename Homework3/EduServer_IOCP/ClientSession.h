#pragma once
#include "ObjectPool.h"
#include "MemoryPool.h"
#include "CircularBuffer.h"

#define BUFSIZE	65536

class ClientSession ;
class SessionManager;

enum IOType
{
	IO_NONE,
	IO_SEND,
	IO_RECV,
	IO_RECV_ZERO,
	IO_ACCEPT,
	IO_DISCONNECT
} ;

enum DisconnectReason
{
	DR_NONE,
	DR_ACTIVE,
	DR_ONCONNECT_ERROR,
	DR_IO_REQUEST_ERROR,
	DR_COMPLETION_ERROR,
};

struct OverlappedIOContext
{
	OverlappedIOContext(ClientSession* owner, IOType ioType);

	OVERLAPPED		mOverlapped ;
	ClientSession*	mSessionObject ;
	IOType			mIoType ;
	WSABUF			mWsaBuf;
	
} ;

//TODO: 아래의 OverlappedXXXXContext는 ObjectPool<>을 사용하도록 수정

struct OverlappedSendContext : public OverlappedIOContext
{
	OverlappedSendContext(ClientSession* owner) : OverlappedIOContext(owner, IO_SEND)
	{
	}
};

struct OverlappedRecvContext : public OverlappedIOContext
{
	OverlappedRecvContext(ClientSession* owner) : OverlappedIOContext(owner, IO_RECV)
	{
	}
};

struct OverlappedPreRecvContext : public OverlappedIOContext
{
	OverlappedPreRecvContext(ClientSession* owner) : OverlappedIOContext(owner, IO_RECV_ZERO)
	{
	}
};

struct OverlappedDisconnectContext : public OverlappedIOContext
{
	OverlappedDisconnectContext(ClientSession* owner, DisconnectReason dr) 
	: OverlappedIOContext(owner, IO_DISCONNECT), mDisconnectReason(dr)
	{}

	DisconnectReason mDisconnectReason;
};

struct OverlappedAcceptContext : public OverlappedIOContext
{
	OverlappedAcceptContext(ClientSession* owner) : OverlappedIOContext(owner, IO_ACCEPT)
	{}
};


void DeleteIoContext(OverlappedIOContext* context) ;

//TODO: 아래의 ClientSession은 xnew/xdelete사용 가능하도록 클래스 정의 부분 수정
class ClientSession
{
public:
	ClientSession();
	~ClientSession() {}

	void	SessionReset();

	bool	IsConnected() const { return !!mConnected; }

	bool	PostAccept();
	void	AcceptCompletion();

	bool	PreRecv() ; ///< zero byte recv

	bool	PostRecv();
	void	RecvCompletion(DWORD transferred);

	bool	PostSend();
	void	SendCompletion(DWORD transferred);
	
	void	DisconnectRequest(DisconnectReason dr);
	void	DisconnectCompletion(DisconnectReason dr);
	
	void	AddRef();
	void	ReleaseRef();

	void	SetSocket(SOCKET sock) { mSocket = sock; }
	SOCKET	GetSocket() const { return mSocket;  }

private:
	
	SOCKET			mSocket ;

	SOCKADDR_IN		mClientAddr ;
		
	FastSpinlock	mBufferLock;

	CircularBuffer	mBuffer;

	volatile long	mRefCount;
	volatile long	mConnected;

	friend class SessionManager;
} ;



