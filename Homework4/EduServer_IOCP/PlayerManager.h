#pragma once

#include "FastSpinlock.h"
#include "XTL.h"

class Player;
typedef xvector<std::shared_ptr<Player>>::type PlayerList;

class PlayerManager
{
public:
	PlayerManager();

	/// 플레이어를 등록하고 ID를 발급
	int RegisterPlayer(std::shared_ptr<Player> player);

	/// ID에 해당하는 플레이어 제거
	void UnregisterPlayer(int playerId);

	int GetCurrentPlayers(PlayerList& outList);
	
private:
	FastSpinlock mLock;
	int mCurrentIssueId;
	xmap<int, std::shared_ptr<Player>>::type mPlayerMap;
};

extern PlayerManager* GPlayerManager;
