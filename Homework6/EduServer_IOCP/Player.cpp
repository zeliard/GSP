#include "stdafx.h"
#include "ClientSession.h"
#include "Player.h"
#include "PlayerDBContext.h"
#include "DBManager.h"

Player::Player(ClientSession* session) : mSession(session)
{
	PlayerReset();
}

Player::~Player()
{
}

void Player::PlayerReset()
{
	FastSpinlockGuard criticalSection(mPlayerLock);

	memset(mPlayerName, 0, sizeof(mPlayerName));
	memset(mComment, 0, sizeof(mComment));
	mPlayerId = -1;
	mIsValid = false;
	mPosX = mPosY = mPosZ = 0;
}

void Player::RequestLoad(int pid)
{
 	LoadPlayerDataContext* context = new LoadPlayerDataContext(mSession, pid);
 	GDatabaseManager->PostDatabsaseRequest(context);
}

void Player::ResponseLoad(int pid, float x, float y, float z, bool valid, wchar_t* name, wchar_t* comment)
{
	FastSpinlockGuard criticalSection(mPlayerLock);

	mPlayerId = pid;
	mPosX = x;
	mPosY = y;
	mPosZ = z;
	mIsValid = valid;

	wcscpy_s(mPlayerName, name);
	wcscpy_s(mComment, comment);

	wprintf_s(L"PID[%d], X[%f] Y[%f] Z[%f] NAME[%s] COMMENT[%s]\n", mPlayerId, mPosX, mPosY, mPosZ, mPlayerName, mComment);
}

void Player::RequestUpdatePosition(float x, float y, float z)
{
	//todo: DB에 플레이어 위치를 x,y,z로 업데이트 요청하기
}

void Player::ResponseUpdatePosition(float x, float y, float z)
{
	FastSpinlockGuard criticalSection(mPlayerLock);
	mPosX = x;
	mPosY = y;
	mPosZ = z;
}

void Player::RequestUpdateComment(const wchar_t* comment)
{
	UpdatePlayerCommentContext* context = new UpdatePlayerCommentContext(mSession, mPlayerId);
	context->SetNewComment(comment);
	GDatabaseManager->PostDatabsaseRequest(context);
}

void Player::ResponseUpdateComment(const wchar_t* comment)
{
	FastSpinlockGuard criticalSection(mPlayerLock);
	wcscpy_s(mComment, comment);
}

void Player::RequestUpdateValidation(bool isValid)
{
	UpdatePlayerValidContext* context = new UpdatePlayerValidContext(mSession, mPlayerId);
	context->mIsValid = isValid;
	GDatabaseManager->PostDatabsaseRequest(context);
}

void Player::ResponseUpdateValidation(bool isValid)
{
	FastSpinlockGuard criticalSection(mPlayerLock);
	mIsValid = isValid;
}


void Player::TestCreatePlayerData(const wchar_t* newName)
{
	//todo: DB스레드풀에 newName에 해당하는 플레이어 생성 작업을 수행시켜보기

}

void Player::TestDeletePlayerData(int playerId)
{
	//todo: DB스레드풀에 playerId에 해당하는 플레이어 생성 삭제 작업을 수행시켜보기
}

