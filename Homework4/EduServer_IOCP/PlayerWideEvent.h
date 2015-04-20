#pragma once

class AllPlayerBuffEvent
{
public:
	AllPlayerBuffEvent(int buffId, int duration);

	void DoBuffToAllPlayers(int fromPlayerId);

private:
	int mBuffId;
	int mDuration;

};

class AllPlayerBuffDecay
{
public:
	AllPlayerBuffDecay() {}

	void CheckBuffTimeout();

};