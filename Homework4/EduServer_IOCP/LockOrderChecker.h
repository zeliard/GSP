#pragma once


class FastSpinlock;

class LockOrderChecker
{
public:
	LockOrderChecker(int tid);
	
	void Push(FastSpinlock* lock);
	void Pop(FastSpinlock* lock);

private:
	enum 
	{
		MAX_LOCK_DEPTH = 32
	};
	
	int mWorkerThreadId;
	int mStackTopPos;

	FastSpinlock* mLockStack[MAX_LOCK_DEPTH];
	
};