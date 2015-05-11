#pragma once

#include "ObjectPool.h"



class ClientSession;

/// 모든 DB관련 작업은 아래의 추상 클래스를 상속받아 사용해야 함
struct DatabaseJobContext
{
	DatabaseJobContext(ClientSession* owner);
	virtual ~DatabaseJobContext();

	void OnResult();

	bool SQLExecute();

	virtual bool OnSQLExecute() = 0;	///< DB풀에서 수행되는 SQL 작업
	virtual void OnSuccess() {}		///< SQL 작업 성공시 로직풀에서 최종 수행되어야 하는 내용
	virtual void OnFail() {}		///< SQL 작업 실패시 로직풀에서 최종 수행되어야 하는 내용


	ClientSession* mSessionObject;
	bool mSuccess;
};

