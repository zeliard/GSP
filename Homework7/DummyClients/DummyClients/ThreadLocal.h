#pragma once

#define MAX_IOTHREAD	8

enum THREAD_TYPE
{
	THREAD_MAIN,
	THREAD_IO_WORKER
};


class ThreadCallHistory;

extern __declspec(thread) int LThreadType;
extern __declspec(thread) int LWorkerThreadId;
extern __declspec(thread) ThreadCallHistory* LThreadCallHistory;
extern __declspec(thread) void* LRecentThisPointer;

extern ThreadCallHistory* GThreadCallHistory[MAX_IOTHREAD];