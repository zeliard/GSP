#pragma once

#include "ObjectPool.h"

enum IOType
{
	IO_NONE,
	IO_SEND,
	IO_RECV,
	IO_RECV_ZERO,

	IO_DISCONNECT,
	IO_CONNECT
};

enum DisconnectReason
{
	DR_NONE,
	DR_ACTIVE,
	DR_ONCONNECT_ERROR,
	DR_IO_REQUEST_ERROR,
	DR_COMPLETION_ERROR,
	DR_SENDFLUSH_ERROR
};



class Session;

struct OverlappedIOContext
{
	OverlappedIOContext(Session* owner, IOType ioType);

	OVERLAPPED	mOverlapped;
	Session*	mSessionObject;
	IOType		mIoType;
	WSABUF		mWsaBuf;

};


struct OverlappedSendContext : public OverlappedIOContext, public ObjectPool<OverlappedSendContext>
{
	OverlappedSendContext(Session* owner) : OverlappedIOContext(owner, IO_SEND)
	{
	}
};

struct OverlappedRecvContext : public OverlappedIOContext, public ObjectPool<OverlappedRecvContext>
{
	OverlappedRecvContext(Session* owner) : OverlappedIOContext(owner, IO_RECV)
	{
	}
};

struct OverlappedPreRecvContext : public OverlappedIOContext, public ObjectPool<OverlappedPreRecvContext>
{
	OverlappedPreRecvContext(Session* owner) : OverlappedIOContext(owner, IO_RECV_ZERO)
	{
	}
};

struct OverlappedDisconnectContext : public OverlappedIOContext, public ObjectPool<OverlappedDisconnectContext>
{
	OverlappedDisconnectContext(Session* owner, DisconnectReason dr)
	: OverlappedIOContext(owner, IO_DISCONNECT), mDisconnectReason(dr)
	{
	}

	DisconnectReason mDisconnectReason;
};


struct OverlappedConnectContext : public OverlappedIOContext, public ObjectPool<OverlappedConnectContext>
{
	OverlappedConnectContext(Session* owner) : OverlappedIOContext(owner, IO_CONNECT)
	{
	}
};


void DeleteIoContext(OverlappedIOContext* context);

