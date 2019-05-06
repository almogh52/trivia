#include "database.h"

#include <io.h>

#include "exception.h"

Database::Database() : m_db(nullptr)
{
}

Database::~Database()
{
	// Close the database
	closeDatabase();
}

void Database::initDatabase()
{
	const char* usersTableQuery = "CREATE TABLE users (username PRIMARY KEY TEXT NOT NULL, password TEXT NOT NULL, email TEXT NOT NULL);";

	int res = 0;

	// Check if the database already exists
	int databaseExists = _access(DB_FILENAME, 0);

	// Open the sqlite db
	res = sqlite3_open(DB_FILENAME, &m_db);
	if (res != SQLITE_OK)
	{
		throw Exception("Unable to connect to SQLite DB!");
	}

	// Enable foreign keys
	res = sqlite3_exec(m_db, "PRAGMA foreign_keys=ON", nullptr, nullptr, nullptr);
	if (res != SQLITE_OK)
	{
		closeDatabase();
		throw Exception("Unable to enable foreign keys!");
	}

	// If the database didn't exist, initialize it
	if (databaseExists == -1)
	{
		// Create the users table
		res = sqlite3_exec(m_db, usersTableQuery, nullptr, nullptr, nullptr);
		if (res != SQLITE_OK)
		{
			closeDatabase();
			throw Exception("Unable to create users table!");
		}
	}
}

void Database::closeDatabase()
{
	// Close the SQLite DB
	sqlite3_close(m_db);
	m_db = nullptr;
}
