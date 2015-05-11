#include "stdafx.h"
#include "Exception.h"
#include "ThreadLocal.h"
#include "Log.h"
#include "DBHelper.h"

SQLHENV DBHelper::mSqlHenv ;
SQL_CONN* DBHelper::mSqlConnPool ;
int DBHelper::mDbWorkerThreadCount = 0;

DBHelper::DBHelper()
{
	CRASH_ASSERT(LThreadType == THREAD_DB_WORKER);

	CRASH_ASSERT(mSqlConnPool[LWorkerThreadId].mUsingNow == false);

	mCurrentSqlHstmt = mSqlConnPool[LWorkerThreadId].mSqlHstmt;
	mCurrentResultCol = 1;
	mCurrentBindParam = 1;
	CRASH_ASSERT(mCurrentSqlHstmt != nullptr);

	mSqlConnPool[LWorkerThreadId].mUsingNow = true;
}

DBHelper::~DBHelper()
{
	SQLFreeStmt(mCurrentSqlHstmt, SQL_UNBIND);
	SQLFreeStmt(mCurrentSqlHstmt, SQL_RESET_PARAMS);
	SQLFreeStmt(mCurrentSqlHstmt, SQL_CLOSE);

	mSqlConnPool[LWorkerThreadId].mUsingNow = false;
}

bool DBHelper::Initialize(const wchar_t* connInfoStr, int workerThreadCount)
{
	mSqlConnPool = new SQL_CONN[workerThreadCount];
	mDbWorkerThreadCount = workerThreadCount;

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &mSqlHenv))
	{
		printf_s("DbHelper Initialize SQLAllocHandle failed\n");
		return false;
	}

	if (SQL_SUCCESS != SQLSetEnvAttr(mSqlHenv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER))
	{
		printf_s("DbHelper Initialize SQLSetEnvAttr failed\n");
		return false;
	}
		
	/// 스레드별로 SQL connection을 풀링하는 방식
	for (int i = 0; i < mDbWorkerThreadCount; ++i)
	{
		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, mSqlHenv, &mSqlConnPool[i].mSqlHdbc))
		{
			printf_s("DbHelper Initialize SQLAllocHandle failed\n");
			return false;
		}
		
		SQLSMALLINT resultLen = 0;
		SQLRETURN ret = SQLDriverConnect(mSqlConnPool[i].mSqlHdbc, NULL, (SQLWCHAR*)connInfoStr, (SQLSMALLINT)wcslen(connInfoStr), NULL, 0, &resultLen, SQL_DRIVER_NOPROMPT);
		if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
		{
			SQLWCHAR sqlState[1024] = { 0, } ;
			SQLINTEGER nativeError = 0;
			SQLWCHAR msgText[1024] = { 0, } ;
			SQLSMALLINT textLen = 0 ;

			SQLGetDiagRec(SQL_HANDLE_DBC, mSqlConnPool[i].mSqlHdbc, 1, sqlState, &nativeError, msgText, 1024, &textLen);

			wprintf_s(L"DbHelper Initialize SQLDriverConnect failed: %s \n", msgText);

			return false;
		}

		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, mSqlConnPool[i].mSqlHdbc, &mSqlConnPool[i].mSqlHstmt))
		{
			printf_s("DbHelper Initialize SQLAllocHandle SQL_HANDLE_STMT failed\n");
			return false;
		}

	}

	return true;
}


void DBHelper::Finalize()
{
	for (int i = 0; i < mDbWorkerThreadCount; ++i)
	{
		SQL_CONN* currConn = &mSqlConnPool[i];
		if (currConn->mSqlHstmt)
			SQLFreeHandle(SQL_HANDLE_STMT, currConn->mSqlHstmt);

		if (currConn->mSqlHdbc)
			SQLFreeHandle(SQL_HANDLE_DBC, currConn->mSqlHdbc);
	}

	delete[] mSqlConnPool;


}

bool DBHelper::Execute(const wchar_t* sqlstmt)
{

	SQLRETURN ret = SQLExecDirect(mCurrentSqlHstmt, (SQLWCHAR*)sqlstmt, SQL_NTSL);
	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		PrintSqlStmtError();
		return false;
	}

	return true;
}

bool DBHelper::FetchRow()
{
	SQLRETURN ret = SQLFetch(mCurrentSqlHstmt);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		if (SQL_NO_DATA != ret)
		{
			PrintSqlStmtError();
		}
		
		return false;
	}

	return true;
}



bool DBHelper::BindParamInt(int* param)
{
	SQLRETURN ret = SQLBindParameter(mCurrentSqlHstmt, mCurrentBindParam++, SQL_PARAM_INPUT,
		SQL_C_LONG, SQL_INTEGER, 10, 0, param, 0, NULL);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		PrintSqlStmtError();
		return false;
	}

	return true;
}

bool DBHelper::BindParamFloat(float* param)
{
	SQLRETURN ret = SQLBindParameter(mCurrentSqlHstmt, mCurrentBindParam++, SQL_PARAM_INPUT,
		SQL_C_FLOAT, SQL_REAL, 15, 0, param, 0, NULL);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		PrintSqlStmtError();
		return false;
	}

	return true;
}

bool DBHelper::BindParamBool(bool* param)
{
	SQLRETURN ret = SQLBindParameter(mCurrentSqlHstmt, mCurrentBindParam++, SQL_PARAM_INPUT,
		SQL_C_TINYINT, SQL_TINYINT, 3, 0, param, 0, NULL);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		PrintSqlStmtError();
		return false;
	}

	return true;
}

bool DBHelper::BindParamText(const wchar_t* text)
{
	SQLRETURN ret = SQLBindParameter(mCurrentSqlHstmt, mCurrentBindParam++, SQL_PARAM_INPUT,
		SQL_C_WCHAR, SQL_WVARCHAR, wcslen(text), 0, (SQLPOINTER)text, 0, NULL);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		PrintSqlStmtError();
		return false;
	}

	return true;
}


void DBHelper::BindResultColumnInt(int* r)
{
	SQLLEN len = 0;
	SQLRETURN ret = SQLBindCol(mCurrentSqlHstmt, mCurrentResultCol++, SQL_C_LONG, r, 4, &len);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		PrintSqlStmtError();
	}
}
void DBHelper::BindResultColumnFloat(float* r)
{
	SQLLEN len = 0;
	SQLRETURN ret = SQLBindCol(mCurrentSqlHstmt, mCurrentResultCol++, SQL_C_FLOAT, r, 4, &len);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		PrintSqlStmtError();
	}
}

void DBHelper::BindResultColumnBool(bool* r)
{
	SQLLEN len = 0;
	SQLRETURN ret = SQLBindCol(mCurrentSqlHstmt, mCurrentResultCol++, SQL_C_TINYINT, r, 1, &len);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		PrintSqlStmtError();
	}
}
void DBHelper::BindResultColumnText(wchar_t* text, size_t count)
{
	SQLLEN len = 0;
	SQLRETURN ret = SQLBindCol(mCurrentSqlHstmt, mCurrentResultCol++, SQL_C_WCHAR, text, count*2, &len);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		PrintSqlStmtError();
	}
}


void DBHelper::PrintSqlStmtError()
{
	SQLWCHAR sqlState[1024] = { 0, };
	SQLINTEGER nativeError = 0;
	SQLWCHAR msgText[1024] = { 0, };
	SQLSMALLINT textLen = 0;

	SQLGetDiagRec(SQL_HANDLE_STMT, mCurrentSqlHstmt, 1, sqlState, &nativeError, msgText, 1024, &textLen);

	wprintf_s(L"DbHelper SQL Statement Error: %ls \n", msgText);
}