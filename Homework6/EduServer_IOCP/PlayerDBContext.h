#pragma once
#include "ContentsConfig.h"
#include "DBContext.h"

//todo: Player 생성 작업 DB context만들기
//struct CreatePlayerDataContext
//{
//	 ... ...
//};

//todo: Player 삭제 작업 DB context 만들기

//struct DeletePlayerDataContext
//{
//};


/// player load 작업
struct LoadPlayerDataContext : public DatabaseJobContext, public ObjectPool<LoadPlayerDataContext>
{
	LoadPlayerDataContext(ClientSession* owner, int pid) : DatabaseJobContext(owner)
	, mPlayerId(pid), mPosX(0), mPosY(0), mPosZ(0), mIsValid(false)
	{
		memset(mPlayerName, 0, sizeof(mPlayerName));
		memset(mComment, 0, sizeof(mComment));
	}

	virtual bool OnSQLExecute();
	virtual void OnSuccess();
	virtual void OnFail();

	int		mPlayerId = -1;
	float	mPosX;
	float	mPosY;
	float	mPosZ;
	bool	mIsValid;
	wchar_t	mPlayerName[MAX_NAME_LEN];
	wchar_t	mComment[MAX_COMMENT_LEN];

};




/// Player 업데이트 작업

struct UpdatePlayerPositionContext : public DatabaseJobContext, public ObjectPool<UpdatePlayerPositionContext>
{
	UpdatePlayerPositionContext(ClientSession* owner, int pid) : DatabaseJobContext(owner)
	, mPlayerId(pid), mPosX(0), mPosY(0), mPosZ(0)
	{
	}

	virtual bool OnSQLExecute();
	virtual void OnSuccess();
	virtual void OnFail() {}

	int		mPlayerId = -1;
	float	mPosX;
	float	mPosY;
	float	mPosZ;
};


struct UpdatePlayerCommentContext : public DatabaseJobContext, public ObjectPool<UpdatePlayerCommentContext>
{
	UpdatePlayerCommentContext(ClientSession* owner, int pid) : DatabaseJobContext(owner), mPlayerId(pid)
	{
		memset(mComment, 0, sizeof(mComment));
	}

	virtual bool OnSQLExecute();
	virtual void OnSuccess();
	virtual void OnFail() {}

	void SetNewComment(const wchar_t* comment);

	int		mPlayerId = -1;
	wchar_t	mComment[MAX_COMMENT_LEN];
};


struct UpdatePlayerValidContext : public DatabaseJobContext, public ObjectPool<UpdatePlayerValidContext>
{
	UpdatePlayerValidContext(ClientSession* owner, int pid) : DatabaseJobContext(owner), mPlayerId(pid), mIsValid(false)
	{
	}

	virtual bool OnSQLExecute();
	virtual void OnSuccess();
	virtual void OnFail() {}

	int		mPlayerId = -1;
	bool	mIsValid;
};
