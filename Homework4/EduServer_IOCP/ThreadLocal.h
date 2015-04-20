#pragma once

#define MAX_IO_THREAD	4

enum THREAD_TYPE
{
	THREAD_MAIN,
	THREAD_IO_WORKER,
	THREAD_DB_WORKER
};


class Timer;
class LockOrderChecker;

extern __declspec(thread) int LThreadType;
extern __declspec(thread) int LIoThreadId;
extern __declspec(thread) Timer* LTimer;
extern __declspec(thread) int64_t LTickCount;
extern __declspec(thread) LockOrderChecker* LLockOrderChecker;

