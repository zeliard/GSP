#include "stdafx.h"
#include "Exception.h"
#include <DbgHelp.h>


LONG WINAPI ExceptionFilter(EXCEPTION_POINTERS* exceptionInfo)
{
	if ( IsDebuggerPresent() )
		return EXCEPTION_CONTINUE_SEARCH ;


	/// dump file 남기자.

	HANDLE hFile = CreateFileA("EasyServer_minidump.dmp", GENERIC_READ | GENERIC_WRITE, 
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ) ; 

	if ( ( hFile != NULL ) && ( hFile != INVALID_HANDLE_VALUE ) ) 
	{
		MINIDUMP_EXCEPTION_INFORMATION mdei ; 

		mdei.ThreadId           = GetCurrentThreadId() ; 
		mdei.ExceptionPointers  = exceptionInfo ; 
		mdei.ClientPointers     = FALSE ; 


		MINIDUMP_TYPE mdt = (MINIDUMP_TYPE)(MiniDumpWithPrivateReadWriteMemory | 
			MiniDumpWithDataSegs | MiniDumpWithHandleData | MiniDumpWithFullMemoryInfo | 
			MiniDumpWithThreadInfo | MiniDumpWithUnloadedModules ) ; 

		MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), 
			hFile, mdt, (exceptionInfo != 0) ? &mdei : 0, 0, NULL ) ; 

		CloseHandle( hFile ) ; 

	}
	else 
	{
		printf("CreateFile failed. Error: %u \n", GetLastError()) ; 
	}

	return EXCEPTION_EXECUTE_HANDLER  ;
	
}