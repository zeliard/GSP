#include "stdafx.h"
#include "SQLStatement.h"
#include "DBHelper.h"

/// 테스트용 임시 
void DbTestFunc()
{
	{
		DBHelper dbHelper;

		dbHelper.BindParamText(L"DbTestPlayer");
		if (dbHelper.Execute(SQL_CreatePlayer))
		{
			if (dbHelper.FetchRow())
			{
				printf("ok");
			}
		}
	}
	{
		DBHelper dbHelper;

		int uid = 100;
		float x = 2301.34f;
		float y = 56000.78f;
		float z = 990002.32f;

		dbHelper.BindParamInt(&uid);
		dbHelper.BindParamFloat(&x);
		dbHelper.BindParamFloat(&y);
		dbHelper.BindParamFloat(&z);

		if (dbHelper.Execute(SQL_UpdatePlayerPosition))
		{
			if (dbHelper.FetchRow())
			{
				printf("ok");
			}
		}
	}

	{
		DBHelper dbHelper;

		int uid = 100;

		dbHelper.BindParamInt(&uid);
		dbHelper.BindParamText(L"Update된 코멘트..입니다.");
		if (dbHelper.Execute(SQL_UpdatePlayerComment))
		{
			if (dbHelper.FetchRow())
			{
				printf("ok");
			}
		}
	}

	{
		DBHelper dbHelper;

		int uid = 100;
		bool v = true;
		dbHelper.BindParamInt(&uid);
		dbHelper.BindParamBool(&v);
		if (dbHelper.Execute(SQL_UpdatePlayerValid))
		{
			if (dbHelper.FetchRow())
			{
				printf("ok");
			}
		}
	}

	{
		DBHelper dbHelper;

		int uid = 100;
		dbHelper.BindParamInt(&uid);

		wchar_t name[32];
		float x = 0;
		float y = 0;
		float z = 0;
		bool valid = false;
		wchar_t comment[256];

		dbHelper.BindResultColumnText(name, 32);
		dbHelper.BindResultColumnFloat(&x);
		dbHelper.BindResultColumnFloat(&y);
		dbHelper.BindResultColumnFloat(&z);
		dbHelper.BindResultColumnBool(&valid);
		dbHelper.BindResultColumnText(comment, 256);

		if (dbHelper.Execute(SQL_LoadPlayer))
		{
			if (dbHelper.FetchRow())
			{
				printf("\n%ls %f %f %f %d %ls\n", name, x, y, z, valid, comment);
			}
		}
	}

	{
		DBHelper dbHelper;

		int uid = 100;

		dbHelper.BindParamInt(&uid);
		if (dbHelper.Execute(SQL_DeletePlayer))
		{
			if (dbHelper.FetchRow())
			{
				printf("ok");
			}
		}
	}

}

