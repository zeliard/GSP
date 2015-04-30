#include "stdafx.h"
#include "Exception.h"
#include "ThreadLocal.h"
#include "DBHelper.h"

//todo: DbHelper의 static 멤버변수 초기화


DbHelper::DbHelper()
{
	CRASH_ASSERT(mSqlConnPool[LWorkerThreadId].mUsingNow == false);

	mCurrentSqlHstmt = mSqlConnPool[LWorkerThreadId].mSqlHstmt;
	mCurrentResultCol = 1;
	mCurrentBindParam = 1;
	CRASH_ASSERT(mCurrentSqlHstmt != nullptr);

	mSqlConnPool[LWorkerThreadId].mUsingNow = true;
}

DbHelper::~DbHelper()
{
	//todo: SQLFreeStmt를 이용하여 현재 SQLHSTMT 해제(unbind, 파라미터리셋, close 순서로)
	
	mSqlConnPool[LWorkerThreadId].mUsingNow = false;
}

bool DbHelper::Initialize(const wchar_t* connInfoStr, int workerThreadCount)
{
	//todo: mSqlConnPool, mDbWorkerThreadCount를 워커스레스 수에 맞추어 초기화

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
		

	/// 스레드별로 SQL connection을 풀링하는 방식. 즉, 스레드마다 SQL서버로의 연결을 갖는다.
	for (int i = 0; i < mDbWorkerThreadCount; ++i)
	{
		//todo: SQLAllocHandle을 이용하여 SQL_CONN의 mSqlHdbc 핸들 사용가능하도록 처리
		
		SQLSMALLINT resultLen = 0;
		
		//todo: SQLDriverConnect를 이용하여 SQL서버에 연결하고 그 핸들을 SQL_CONN의 mSqlHdbc에 할당
		SQLRETURN ret = 0; // =  SQLDriverConnect(...);

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

		//todo: SQLAllocHandle를 이용하여 SQL_CONN의 mSqlHstmt 핸들 사용가능하도록 처리
	
	}

	return true;
}


void DbHelper::Finalize()
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

bool DbHelper::Execute(const wchar_t* sqlstmt)
{
	//todo: mCurrentSqlHstmt핸들 사용하여 sqlstmt를 수행.  
	SQLRETURN ret = 0; //= SQLExecDirect(...);
	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		PrintSqlStmtError();
		return false;
	}

	return true;
}

bool DbHelper::FetchRow()
{
	//todo: mCurrentSqlHstmt가 들고 있는 내용 fetch
	SQLRETURN ret = 0; 

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



bool DbHelper::BindParamInt(int* param)
{
	//todo: int형 파라미터 바인딩
	SQLRETURN ret = 0; // = SQLBindParameter(...);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		PrintSqlStmtError();
		return false;
	}

	return true;
}

bool DbHelper::BindParamFloat(float* param)
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

bool DbHelper::BindParamBool(bool* param)
{
	//todo: bool형 파라미터 바인딩
	SQLRETURN ret = 0; // = SQLBindParameter(...);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		PrintSqlStmtError();
		return false;
	}

	return true;
}

bool DbHelper::BindParamText(const wchar_t* text)
{

	//todo: 유니코드 문자열 바인딩
	SQLRETURN ret = 0; // = SQLBindParameter(...);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		PrintSqlStmtError();
		return false;
	}

	return true;
}


void DbHelper::BindResultColumnInt(int* r)
{
	SQLLEN len = 0;
	SQLRETURN ret = SQLBindCol(mCurrentSqlHstmt, mCurrentResultCol++, SQL_C_LONG, r, 4, &len);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		PrintSqlStmtError();
	}
}
void DbHelper::BindResultColumnFloat(float* r)
{
	SQLLEN len = 0;
	//todo: float형 결과 컬럼 바인딩
	SQLRETURN ret = 0;

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		PrintSqlStmtError();
	}
}

void DbHelper::BindResultColumnBool(bool* r)
{
	SQLLEN len = 0;
	SQLRETURN ret = SQLBindCol(mCurrentSqlHstmt, mCurrentResultCol++, SQL_C_TINYINT, r, 1, &len);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		PrintSqlStmtError();
	}
}
void DbHelper::BindResultColumnText(wchar_t* text, size_t count)
{
	SQLLEN len = 0;
	//todo: wchar_t*형 결과 컬럼 바인딩
	SQLRETURN ret = 0;

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		PrintSqlStmtError();
	}
}


void DbHelper::PrintSqlStmtError()
{
	SQLWCHAR sqlState[1024] = { 0, };
	SQLINTEGER nativeError = 0;
	SQLWCHAR msgText[1024] = { 0, };
	SQLSMALLINT textLen = 0;

	SQLGetDiagRec(SQL_HANDLE_STMT, mCurrentSqlHstmt, 1, sqlState, &nativeError, msgText, 1024, &textLen);

	wprintf_s(L"DbHelper SQL Statement Error: %ls \n", msgText);
}