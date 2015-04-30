#pragma once


#define LISTEN_PORT		9990
#define MAX_CONNECTION	10000

#define CONNECT_SERVER_ADDR	"127.0.0.1"
#define CONNECT_SERVER_PORT 9001

//todo: SQL 연결 스트링 여러분의 상황에 맞게 수정
#define SQL_SERVER_CONN_STR	L"Driver={SQL Server};Server=127.0.0.1\\SQL4GAMESERVER;Database=GameDB;UID=sa;PWD=...."

#define GQCS_TIMEOUT	10 //INFINITE

enum THREAD_TYPE
{
	THREAD_MAIN,
	THREAD_IO_WORKER
};

extern __declspec(thread) int LThreadType;