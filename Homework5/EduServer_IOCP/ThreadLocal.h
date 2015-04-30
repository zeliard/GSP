#pragma once

class Session;

extern __declspec(thread) int LWorkerThreadId;
extern __declspec(thread) std::deque<Session*>* LSendRequestSessionList;