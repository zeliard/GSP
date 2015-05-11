#include "stdafx.h"
#include "ThreadLocal.h"
#include "FastSpinlock.h"
#include "ClientSession.h"
#include "DBContext.h"


DatabaseJobContext::DatabaseJobContext(ClientSession* owner) : mSessionObject(owner), mSuccess(false)
{
	mSessionObject->AddRef();
}

DatabaseJobContext::~DatabaseJobContext()
{
	mSessionObject->ReleaseRef();
}

bool DatabaseJobContext::SQLExecute()
{
	//todo: 이 함수는 반드시 DB스레드풀에서 수행되어야 한다. 그렇지 않으면 CRASH시키기

	return OnSQLExecute();
}

void DatabaseJobContext::OnResult()
{
	//todo: 이 함수는 반드시 IO스레드풀에서 수행되어야 한다. 그렇지 않으면 CRASH시키기

	if (mSuccess)
		OnSuccess();
	else
		OnFail();
}



