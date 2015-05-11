#include "stdafx.h"
#include "Log.h"

#include <iostream>

void ThreadCallHistory::DumpOut(std::ostream& ost)
{
	//todo: 현재 스레드의 call history를 ost 스트림에 쓰기
	
}
	

void ThreadCallElapsedRecord::DumpOut(std::ostream& ost)
{
	uint64_t count = mCounter < MAX_ELAPSED_RECORD ? mCounter : MAX_ELAPSED_RECORD;

	ost << "===== Recent Call Performance [Thread:" << mThreadId << "]" << std::endl;

	for (int i = 1; i <= count; ++i)
	{
		ost << "  FUNC:" << mElapsedFuncSig[(mCounter - i) % MAX_ELAPSED_RECORD] 
			<< "ELAPSED: " << mElapsedTime[(mCounter - i) % MAX_ELAPSED_RECORD] << std::endl;
	}
	ost << "===== End of Recent Call Performance" << std::endl << std::endl;

}


namespace LoggerUtil
{
	LogEvent gLogEvents[MAX_LOG_SIZE];
	__int64 gCurrentLogIndex = 0;

	void EventLogDumpOut(std::ostream& ost)
	{
		//todo: gLogEvents내용 ost 스트림에 쓰기

		
	}
}
