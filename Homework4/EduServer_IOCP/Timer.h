#pragma once
#include "XTL.h"

class SyncExecutable;
typedef std::function<void()> TimerTask;
typedef std::shared_ptr<SyncExecutable> SyncExecutablePtr;



struct TimerJobElement
{
	TimerJobElement(SyncExecutablePtr owner, const TimerTask& task, int64_t execTick)
	: mOwner(owner), mTask(task), mExecutionTick(execTick)
	{}

	virtual ~TimerJobElement() {}

	SyncExecutablePtr mOwner;
	TimerTask	mTask;
	int64_t		mExecutionTick;
};

struct TimerJobComparator
{
	bool operator()(const TimerJobElement& lhs, const TimerJobElement& rhs)
	{
		return lhs.mExecutionTick > rhs.mExecutionTick;
	}
};


typedef std::priority_queue<TimerJobElement, std::vector<TimerJobElement, STLAllocator<TimerJobElement> >, TimerJobComparator> TimerJobPriorityQueue;

class Timer
{
public:

	Timer();

	void PushTimerJob(SyncExecutablePtr owner, const TimerTask& task, uint32_t after);

	void DoTimerJob();

private:

	TimerJobPriorityQueue	mTimerJobQueue;

};

