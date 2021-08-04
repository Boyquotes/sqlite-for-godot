# sqlite-for-godot
A Godot module that provides a convenient object-orientated wrapper for sqlite.


## Instructions
1. clone the Godot repo
2. clone the branch of this repo that matches your Godot version into the "modules" 
folder inside the Godot repo
3. rename the "sqlite-for-godot" folder to "sqlite"
4. build Godot


## Usage
Example Program:
```gdscript
func test_sqlite():
	#Test sqlite driver
	print("\nsqlite Driver Test")
	print("====================")
	print("Starting sqlite driver...")
	var sql = SQLite.new()
	
	#Test sqlite database connection
	print("Connecting to sqlite database...")
	var db_path = "test.db"
	
	if not sql.connect(db_path):
		print("Failed to open database.")
		
	#Try to drop existing users table
	print("Dropping users table...")
	var stmt = sql.compile_sql("DROP TABLE IF EXISTS users;")
	
	if not stmt.execute([]):
		print("Failed to drop table.")
		
	#Try to create new users table
	print("Creating users table...")
	stmt = sql.compile_sql("CREATE TABLE users(id INTEGER PRIMARY KEY, name VARCHAR(32), score INT, hp REAL);")
	
	if not stmt.execute([]):
		print("Failed to create table.")
	
	#Try to insert into users table
	print("Inserting records into users table...")
	stmt = sql.compile_sql("INSERT INTO users(name, score, hp) VALUES (?, ?, ?);")
	var users = [["Dylan", 10000, 500.8], ["Emmi", 5000, 800.37], ["Fiona", 2500, 568.975]]
	
	for user in users:
		stmt.execute(user)
		
	#Try to select data from users table
	print("Selecting data from users table...")
	stmt = sql.compile_sql("SELECT * FROM users;")
	
	if not stmt.execute([]):
		print("Failed to select data from users table.")
		
	var row = stmt.next_row()
	
	while row != null:
		print(row)
		row = stmt.next_row()
		
	#Try an sql statement that should fail
	print("Testing error handling...")
	stmt = sql.compile_sql("SELECT * FROM log;")
	
	if stmt != null:
		print("The following statement should have failed to compile!")
		
	print("Error message is '" + sql.get_last_error() + "'")
```


## Class Reference
### SQLite
sqlite driver class (manages exactly one database connection, can have multiple 
instances)

### SQLiteStatement
sqlite compiled SQL statement class (do not create instances directly)
                  
                  
## Method Reference
### bool SQLite.connect(String filename)
connect to an sqlite database

### SQLiteStatement SQLite.compile_sql(String sql)
compile an SQL statement, returns null on failure

###String SQLite.get_last_error()
get the last error message, useful for debugging when a function call failed
                                                 
### bool SQLStatment.execute(Array params)
execute an SQL statement up until the first row of results, "params" should contain 
one value per "?" placeholder code in the SQL statement, pass an empty array if 
there are no placeholder codes

### Dictionary SQLStatement.next_row()
returns a dictionary containing the contents of the current row in the result set 
and executes the statement up until the next row of results