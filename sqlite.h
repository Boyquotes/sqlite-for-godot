#ifndef SQLITE_H
#define SQLITE_H

/*
SQLite Database Module
*/

#include "core/reference.h"
#include "core/variant.h"

#include "sqlite3.h"
#include "sqlite_statement.h"


//Classes
//=================================================================================
class SQLite : public Reference
{
    OBJ_TYPE(SQLite, Reference);
    
    sqlite3 *db;
    
protected:
    static void _bind_methods();
    
public:
    SQLite();
    ~SQLite();
    bool connect(String filename);
    Variant compile_sql(String sql);
    String get_last_error();
};

#endif