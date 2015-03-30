#pragma once

#define LISTEN_PORT	9001


enum THREAD_TYPE
{
	THREAD_MAIN_ACCEPT,
	THREAD_IO_WORKER
};

extern __declspec(thread) int LThreadType;