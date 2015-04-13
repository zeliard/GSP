#pragma once

#define LISTEN_PORT		9001
#define MAX_CONNECTION	10000

enum THREAD_TYPE
{
	THREAD_MAIN,
	THREAD_IO_WORKER
};

extern __declspec(thread) int LThreadType;