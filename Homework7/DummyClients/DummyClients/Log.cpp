#include "stdafx.h"
#include "Log.h"

#include <iostream>

void ThreadCallHistory::DumpOut(std::ostream& ost)
{
	uint64_t count = mCounter < MAX_HISTORY ? mCounter : MAX_HISTORY;

	ost << "===== Recent Call History [Thread:" << mThreadId << "]" << std::endl ;

	for (int i = 1; i <= count; ++i)
	{
		ost << "   " << mHistory[(mCounter-i) % MAX_HISTORY] << std::endl;
	}
	ost << "===== End of Recent Call History" << std::endl << std::endl;
}
	
	
namespace LoggerUtil
{
	LogEvent gLogEvents[MAX_LOG_SIZE];
	__int64 gCurrentLogIndex = 0;
}
