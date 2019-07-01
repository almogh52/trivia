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
	virtual std::unordered_map<std::string, std::unordered_map<int, int>> getAllScores();
	virtual unsigned int createQuestion(std::string question, std::string correctAns, std::string ans2, std::string ans3, std::string ans4);
	virtual unsigned int createGame();
	virtual void endGame(unsigned int gameId);

private:
	sqlite3* m_db;
};

