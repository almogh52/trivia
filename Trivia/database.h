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
	virtual void signUpUser(std::string username, std::string password, std::string email);
	virtual bool authUser(std::string username, std::string password);

private:
	sqlite3* m_db;
};

