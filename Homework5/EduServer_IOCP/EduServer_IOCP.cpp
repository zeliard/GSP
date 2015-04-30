// EduServer_IOCP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Exception.h"
#include "EduServer_IOCP.h"
#include "ServerSession.h"
#include "ClientSession.h"
#include "ClientSessionManager.h"
#include "IocpManager.h"
#include "DBHelper.h"


__declspec(thread) int LThreadType = -1;

int _tmain(int argc, _TCHAR* argv[])
{
	LThreadType = THREAD_MAIN;

	/// for dump on crash
	SetUnhandledExceptionFilter(ExceptionFilter);

	/// Global Managers
	GClientSessionManager = new ClientSessionManager;
	GIocpManager = new IocpManager;


	if (false == GIocpManager->Initialize())
		return -1;

	if (false == DbHelper::Initialize(SQL_SERVER_CONN_STR, GIocpManager->GetIoThreadCount()))
		return -1;

	if (false == GIocpManager->StartIoThreads())
		return -1;

	printf_s("Start Server\n");

 	
	GIocpManager->StartAccept(); ///< block here...

	GIocpManager->Finalize();

	printf_s("End Server\n");

	DbHelper::Finalize();

	delete GIocpManager;
	delete GClientSessionManager;

	return 0;
}

