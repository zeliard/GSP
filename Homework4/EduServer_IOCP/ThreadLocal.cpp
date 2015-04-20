#include "stdafx.h"
#include "ThreadLocal.h"
#include "Timer.h"
#include "LockOrderChecker.h"

__declspec(thread) int LThreadType = -1;
__declspec(thread) int LIoThreadId = -1;
__declspec(thread) Timer* LTimer = nullptr;
__declspec(thread) int64_t LTickCount = 0;
__declspec(thread) LockOrderChecker* LLockOrderChecker = nullptr;

