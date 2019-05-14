#pragma once
#include "database_interface.h"

#include <memory>

#include "sqlite3.h"

#define DB_FILENAME "trivia_db.sqlite"

class Database :
	public IDatabase
{
public:
	Database();
	~Database();

	virtual void initDatabase();
	virtual void closeDatabase();

	virtual bool doesUserExist(std::string username);

private:
	sqlite3* m_db;
};

