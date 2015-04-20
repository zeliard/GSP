#pragma once

enum LockOrder
{
	LO_DONT_CARE = 0,
	LO_FIRST_CLASS,
	LO_BUSINESS_CLASS,
	LO_ECONOMLY_CLASS,
	LO_LUGGAGE_CLASS
};

class LockOrderChecker;

class FastSpinlock
{
public:
	FastSpinlock(const int lockOrder=LO_DONT_CARE);
	~FastSpinlock();

	/// exclusive mode
	void EnterWriteLock();
	void LeaveWriteLock();

	/// shared mode
	void EnterReadLock();
	void LeaveReadLock();


	long GetLockFlag() const { return mLockFlag;  }
	
private:

	enum LockFlag
	{
		LF_WRITE_MASK	= 0x7FF00000,
		LF_WRITE_FLAG	= 0x00100000,
		LF_READ_MASK	= 0x000FFFFF ///< 하위 20비트를 readlock을 위한 플래그로 사용한다.
	};

	FastSpinlock(const FastSpinlock& rhs);
	FastSpinlock& operator=(const FastSpinlock& rhs);

	volatile long mLockFlag;

	const int mLockOrder;

	friend class LockOrderChecker;
};

class FastSpinlockGuard
{
public:
	FastSpinlockGuard(FastSpinlock& lock, bool exclusive = true) : mLock(lock), mExclusiveMode(exclusive)
	{
		if (mExclusiveMode)
			mLock.EnterWriteLock();
		else
			mLock.EnterReadLock();
	}

	~FastSpinlockGuard()
	{
		if (mExclusiveMode)
			mLock.LeaveWriteLock();
		else
			mLock.LeaveReadLock();
	}

private:
	FastSpinlock& mLock;
	bool mExclusiveMode;
};

template <class TargetClass>
class ClassTypeLock
{
public:
	struct LockGuard
	{
		LockGuard()
		{
			TargetClass::mLock.EnterWriteLock();
		}

		~LockGuard()
		{
			TargetClass::mLock.LeaveWriteLock();
		}

	};

private:
	static FastSpinlock mLock;
};

template <class TargetClass>
FastSpinlock ClassTypeLock<TargetClass>::mLock;