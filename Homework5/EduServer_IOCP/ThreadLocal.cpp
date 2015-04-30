#include "stdafx.h"
#include "ThreadLocal.h"
#include "Session.h"

__declspec(thread) int LWorkerThreadId = -1;
__declspec(thread) std::deque<Session*>* LSendRequestSessionList = nullptr;