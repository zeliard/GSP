#pragma once

const wchar_t* SQL_LoadPlayer = L"{ call dbo.spLoadPlayer (?) }"; ///< uid

const wchar_t* SQL_CreatePlayer = L"{ call dbo.spCreatePlayer ( ? ) }"; ///< name(32)

const wchar_t* SQL_DeletePlayer = L"{ call dbo.spDeletePlayer (?) }"; ///< uid

const wchar_t* SQL_UpdatePlayerPosition = L"{ call dbo.spUpdatePlayerPosition (?, ?, ? ,?) }"; ///< uid, x, y, z

const wchar_t* SQL_UpdatePlayerComment = L"{ call dbo.spUpdatePlayerComment (?, ?) }"; ///< uid, comment(256)

const wchar_t* SQL_UpdatePlayerValid = L"{ call dbo.spUpdatePlayerValid (?, ?) }"; ///< uid, isValid

