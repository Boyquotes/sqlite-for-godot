#ifndef SQLITE_STATEMENT_H
#define SQLITE_STATEMENT_H

/*
SQLite Database Statement
*/

#include "core/reference.h"
#include "core/variant.h"

#include "sqlite3.h"


//Classes
//=================================================================================
class SQLiteStatement : public Reference
{
    OBJ_TYPE(SQLiteStatement, Reference);
    
    sqlite3_stmt *stmt;
    bool has_results;
    
protected:
    static void _bind_methods();
    bool bind_params(Array &params);
    
public:
    SQLiteStatement();
    ~SQLiteStatement();
    bool compile(sqlite3 *db, const CharType *sql);
    bool execute(Array params);
    Variant next_row();
};

#endif