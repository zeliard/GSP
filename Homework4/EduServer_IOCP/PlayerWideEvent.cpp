#include "stdafx.h"
#include "GrandCentralExecuter.h"
#include "Player.h"
#include "PlayerWideEvent.h"
#include "PlayerManager.h"

AllPlayerBuffEvent::AllPlayerBuffEvent(int buffId, int duration) : mBuffId(buffId), mDuration(duration)
{

}


void AllPlayerBuffEvent::DoBuffToAllPlayers(int fromPlayerId)
{
	PlayerList currentPlayers;

	if (GPlayerManager->GetCurrentPlayers(currentPlayers) > 0)
	{
		for (auto& it : currentPlayers)
		{
			it->AddBuff(fromPlayerId, mBuffId, mDuration);
		}
	}
}


void AllPlayerBuffDecay::CheckBuffTimeout()
{
	PlayerList currentPlayers;

	if (GPlayerManager->GetCurrentPlayers(currentPlayers) > 0)
	{
		for (auto& it : currentPlayers)
		{
			it->DecayTickBuff();
		}
	}
}
