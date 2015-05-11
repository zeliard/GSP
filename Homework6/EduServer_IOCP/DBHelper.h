#pragma once

#include <sqlext.h>
#include <sqltypes.h>

struct SQL_CONN
{
	SQLHDBC		mSqlHdbc = nullptr;
	SQLHSTMT	mSqlHstmt = nullptr;
	bool		mUsingNow = false;
};

class DBHelper
{
public:
	DBHelper();
	~DBHelper();

	static bool Initialize(const wchar_t* connInfoStr, int workerThreadCount);
	static void Finalize();

	bool Execute(const wchar_t* sqlstmt);
	bool FetchRow();


	/// 아래는 DbHelper 객체를 선언하고 사용해야 함

	bool BindParamInt(int* param);
	bool BindParamFloat(float* param);
	bool BindParamBool(bool* param);
	bool BindParamText(const wchar_t* text);
	
	/// FetchRow시 데이터가 있다면 반드시 차례대로 호출해야함
	void BindResultColumnInt(int* r);
	void BindResultColumnFloat(float* r);
	void BindResultColumnBool(bool* r);
	void BindResultColumnText(wchar_t* text, size_t count);

	void PrintSqlStmtError();

private:
	 
	SQLHSTMT	mCurrentSqlHstmt = nullptr ;
	
	SQLSMALLINT	mCurrentResultCol;
	SQLSMALLINT	mCurrentBindParam;

	static SQLHENV mSqlHenv;
	static SQL_CONN* mSqlConnPool; ///< 워커스레드수만큼
	static int mDbWorkerThreadCount;

};