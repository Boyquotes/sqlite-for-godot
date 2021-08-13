/*
SQLite Database Module
*/

#include "sqlite.h"


//Functions
//=================================================================================
void SQLite::_bind_methods()
{
    ObjectTypeDB::bind_method(_MD("connect", "filename"), &SQLite::connect);
    ObjectTypeDB::bind_method(_MD("compile_sql", "sql"), &SQLite::compile_sql);
    ObjectTypeDB::bind_method(_MD("execute_sql", "sql"), &SQLite::execute_sql);
    ObjectTypeDB::bind_method(_MD("get_last_error"), &SQLite::get_last_error);
}


SQLite::SQLite()
{
    db = nullptr;
}


SQLite::~SQLite()
{
    //Close the database connection if there is one
    if(db)
    {
        sqlite3_close(db);
    }
}


bool SQLite::connect(String filename)
{
    //Close existing database connection if there is one
    if(db)
    {
        sqlite3_close(db);
    }
    
    //Open a database connection
    return sqlite3_open16(filename.c_str(), &db) == SQLITE_OK;
}


Variant SQLite::compile_sql(String sql)
{
    //Create new statement and compile it
    SQLiteStatement *stmt = memnew(SQLiteStatement());
    
    if(!stmt)
    {
        return Variant();
    }
    
    if(!stmt->compile(db, sql.c_str()))
    {
        memdelete(stmt);
        return Variant();
    }
    
    return Variant((Object*)stmt);
}


bool SQLite::execute_sql(String sql)
{
    return sqlite3_exec(db, sql.utf8().get_data(), nullptr, nullptr, 
        nullptr) == SQLITE_OK;
}


String SQLite::get_last_error()
{
    return String((const CharType *)sqlite3_errmsg16(db));
}