#pragma once

//todo: 아래의 SP를 호출하는 SQL 스트링을 제대로 정의

const wchar_t* SQL_LoadPlayer = L"{ call dbo.spLoadPlayer (?) }"; ///< uid

const wchar_t* SQL_CreatePlayer;//  spCreatePlayer

const wchar_t* SQL_DeletePlayer;// spDeletePlayer

const wchar_t* SQL_UpdatePlayerPosition;// spUpdatePlayerPosition

const wchar_t* SQL_UpdatePlayerComment; //spUpdatePlayerComment

const wchar_t* SQL_UpdatePlayerValid; // spUpdatePlayerValid

