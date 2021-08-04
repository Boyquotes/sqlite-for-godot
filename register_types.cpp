#include "register_types.h"

#include "core/object_type_db.h"
#include "sqlite.h"
#include "sqlite_statement.h"


//Functions
//=================================================================================
void register_sqlite_types()
{
    ObjectTypeDB::register_type<SQLite>();
    ObjectTypeDB::register_type<SQLiteStatement>();
}


void unregister_sqlite_types()
{
    //Not needed for this example
}