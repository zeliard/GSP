#pragma once

#include <iostream>
#include "Exception.h"
#include "ThreadLocal.h"

class ThreadCallHistory
{
public:
	ThreadCallHistory(int tid) : mThreadId(tid)
	{
		memset(mHistory, 0, sizeof(mHistory));
	}

	inline void Append(const char* funsig)
	{
		mHistory[mCounter++ % MAX_HISTORY] = funsig;
	}

	void DumpOut(std::ostream& ost = std::cout);

private:
	enum
	{
		MAX_HISTORY = 1024
	};

	uint64_t	mCounter = 0;
	int			mThreadId = -1;
	const char*	mHistory[MAX_HISTORY];
};


#define TRACE_THIS	\
	__if_exists (this)	\
	{	\
		LRecentThisPointer = (void*)this;	\
	}	\
	if (LThreadType != THREAD_MAIN)	\
	{	\
		/*todo: 스레드로컬에 함수 호출(__FUNCSIG__) 기록남기기*/ \
	}	
	


class ThreadCallElapsedRecord
{
public:
	ThreadCallElapsedRecord(int tid) : mThreadId(tid)
	{
		memset(mElapsedFuncSig, 0, sizeof(mElapsedFuncSig));
		memset(mElapsedTime, 0, sizeof(mElapsedTime));
	}

	inline void Append(const char* funcsig, int64_t elapsed)
	{
		//todo: mElapsedFuncSig, mElapsedTime에 정보(funcsig, elapsed) 남기기
	}

	void DumpOut(std::ostream& ost = std::cout);

private:
	enum
	{
		MAX_ELAPSED_RECORD = 512
	};

	uint64_t	mCounter = 0;
	int			mThreadId = -1;
	const char*	mElapsedFuncSig[MAX_ELAPSED_RECORD];
	int64_t		mElapsedTime[MAX_ELAPSED_RECORD];
};

class ScopeElapsedCheck
{
public:
	ScopeElapsedCheck(const char* funcsig) : mFuncSig(funcsig)
	{
		/* FYI
		 * 10~16 ms 해상도로 체크하려면 GetTickCount 사용
		 * 1 us 해상도로 체크하려면  QueryPerformanceCounter 사용
		*/ 
		mStartTick = GetTickCount64();
	}

	~ScopeElapsedCheck()
	{
		if (LThreadType != THREAD_MAIN)
		{
			//todo: LThreadCallElapsedRecord에 함수 수행 시간 남기기
		}
	}

private:

	const char*	mFuncSig;
	int64_t	mStartTick = 0;
};

#define TRACE_PERF	\
	ScopeElapsedCheck __scope_elapsed_check__(__FUNCSIG__);


namespace LoggerUtil
{

	struct LogEvent
	{
		int mThreadId = -1;
		int	mAdditionalInfo = 0;
		const char* mMessage = nullptr; 
	};

	#define MAX_LOG_SIZE  65536   ///< Must be a power of 2

	extern LogEvent gLogEvents[MAX_LOG_SIZE];
	extern __int64 gCurrentLogIndex;

	inline void EventLog(const char* msg, int info)
	{
		__int64 index = _InterlockedIncrement64(&gCurrentLogIndex) - 1;
		
		//todo: gLogEvents에 LogEvent정보 남기기
	}

	void EventLogDumpOut(std::ostream& ost = std::cout);
}

#define EVENT_LOG(x, info) LoggerUtil::EventLog(x, info)

