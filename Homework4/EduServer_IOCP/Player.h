#pragma once

#include "SyncExecutable.h"
class ClientSession;


class Player : public SyncExecutable
{
public:
	Player(ClientSession* session);
	virtual ~Player();

	int GetPlayerId() { return mPlayerId; }
	bool IsAlive() { return mIsAlive;  }
	void Start(int heartbeat);

	void OnTick(); ///< 로그인후 1초마다 불리는 기능

	void PlayerReset();

	/// 플레이어에게 버프를 걸어주는 함수라고 치자.
	void AddBuff(int fromPlayerId, int buffId, int duration);

	/// 주기적으로 버프 시간 업데이트하는 함수
	void DecayTickBuff();

private:

	int		mPlayerId;
	int		mHeartBeat;
	bool	mIsAlive;

	/// 버프 리스트는 lock없이 GCE로 해보기
	std::map<int, int> mBuffList; ///< (id, time)

	ClientSession* const mSession;
	friend class ClientSession;
};