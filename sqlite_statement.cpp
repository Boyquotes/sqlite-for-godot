/*
SQLite Database Statement
*/

#include "sqlite_statement.h"


//Functions
//=================================================================================
void SQLiteStatement::_bind_methods()
{
    ObjectTypeDB::bind_method(_MD("execute", "params"), &SQLiteStatement::execute);
    ObjectTypeDB::bind_method(_MD("next_row"), &SQLiteStatement::next_row);
}


bool SQLiteStatement::bind_params(Array &params)
{
    //Bind each param
    Variant param;
    
    for(int i = 0; i < params.size(); i++)
    {
        //Fetch next param
        param = params[i];
        
        //Choose type
        switch(param.get_type())
        {
            //Integer?
        case Variant::INT:
        {
            //Bind integer param
            sqlite3_bind_int(stmt, i + 1, (int)param);
            break;
        }
        
            //Float?
        case Variant::REAL:
        {
            //Bind float param
            sqlite3_bind_double(stmt, i + 1, (double)param);
            break;
        }
        
            //Null?
        case Variant::NIL:
        {
            //Bind null param
            sqlite3_bind_null(stmt, i + 1);
            break;
        }
        
            //String?
        case Variant::STRING:
        {
            //Bind string param
            sqlite3_bind_text16(stmt, i + 1, ((String)param).c_str(), -1, 
                nullptr);
            break;
        }
        
            //Unknown?
        default:
            return false;
        }
    }
    
    return true;
}


SQLiteStatement::SQLiteStatement()
{
    stmt = nullptr;
    has_results = false;
}


SQLiteStatement::~SQLiteStatement()
{
    //Free the compiled statement if it exists
    if(stmt)
    {
        sqlite3_finalize(stmt);
    }
}


bool SQLiteStatement::compile(sqlite3 *db, const CharType *sql)
{
    //Compile the SQL code
    return sqlite3_prepare16_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK;
}


bool SQLiteStatement::execute(Array params)
{
    //Return if there is no compiled statement
    if(!stmt)
    {
        return false;
    }
    
    //Reset the statement, bind the given parameters, and execute the statement up 
    //until the first row of results
    sqlite3_reset(stmt);
    
    if(!bind_params(params))
    {
        return false;
    }
    
    int res = sqlite3_step(stmt);
    
    if(res == SQLITE_DONE)
    {
        has_results = false;
        return true;
    }
    
    if(res == SQLITE_ROW)
    {
        has_results = true;
        return true;
    }
    
    return false;
}


Variant SQLiteStatement::next_row()
{
    //Return if there is no statement or no result set
    if(!stmt || !has_results)
    {
        return Variant();
    }
    
    //Create result set dictionary
    Dictionary results;
    
    //Populate result set dictionary
    Variant key;
    Variant value;
    
    for(int i = 0; i < sqlite3_column_count(stmt); i++)
    {
        //Fetch the column name
        const char *col_name = sqlite3_column_name(stmt, i);
        
        if(!col_name)
        {
            return Variant();
        }
        
        key = Variant(col_name);
        
        //Choose type
        switch(sqlite3_column_type(stmt, i))
        {
            //Integer?
        case SQLITE_INTEGER:
        {
            //Build int result value
            value = Variant(sqlite3_column_int(stmt, i));
            break;
        }
        
            //Float?
        case SQLITE_FLOAT:
        {
            //Build float result value
            value = Variant(sqlite3_column_double(stmt, i));
            break;
        }
        
            //Blob?
        case SQLITE_BLOB:
        {
            //Build blob result value
            value = Variant(); //not implemented yet!
            break;
        }
        
            //Null?
        case SQLITE_NULL:
        {
            //Build null result value
            value = Variant();
            break;
        }
        
            //Text or unknown?
        case SQLITE_TEXT:
        default:
        {
            //Build text result value
            const CharType *text = (const CharType *)sqlite3_column_text16(stmt, i);
            
            if(!text)
            {
                return Variant();
            }
            
            value = Variant(text);
            break;
        }
        }
        
        //Add value to result set dictionary
        results[key] = value;
    }
    
    //Execute statement up until next row of results
    has_results = sqlite3_step(stmt) == SQLITE_ROW;
    
    //Return result set dictionary
    return Variant(results);
}