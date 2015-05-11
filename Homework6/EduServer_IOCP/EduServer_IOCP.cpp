// EduServer_IOCP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Exception.h"
#include "EduServer_IOCP.h"
#include "ServerSession.h"
#include "ClientSession.h"
#include "ClientSessionManager.h"
#include "IocpManager.h"
#include "DBManager.h"

int _tmain(int argc, _TCHAR* argv[])
{
	LThreadType = THREAD_MAIN;

	/// for dump on crash
	SetUnhandledExceptionFilter(ExceptionFilter);

	/// Global Managers
	GClientSessionManager = new ClientSessionManager;
	GIocpManager = new IocpManager;
	GDatabaseManager = new DBManager;
	

	if (false == GIocpManager->Initialize())
		return -1;

	if (false == GDatabaseManager->Initialize())
		return -1;

	if (false == GDatabaseManager->StartDatabaseThreads())
		return -1;

	if (false == GIocpManager->StartIoThreads())
		return -1;

	printf_s("Start Server\n");

 	GIocpManager->StartAccept(); ///< block here...


	GDatabaseManager->Finalize();
	GIocpManager->Finalize();

	printf_s("End Server\n");


	delete GDatabaseManager;
	delete GIocpManager;
	delete GClientSessionManager;

	return 0;
}

