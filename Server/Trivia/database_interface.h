#pragma once
#include <string>

#include <unordered_map>

class IDatabase {
public:
	virtual ~IDatabase() {};

	virtual void initDatabase() = 0;
	virtual void closeDatabase() = 0;

	virtual bool doesUserExist(std::string username) = 0;
	virtual void signUpUser(std::string username, std::string password, std::string email) = 0;
	virtual bool authUser(std::string username, std::string password) = 0;
	virtual std::unordered_map<std::string, std::unordered_map<int, int>> getAllScores() = 0;
	virtual unsigned int createQuestion(std::string question, std::string correctAns, std::string ans2, std::string ans3, std::string ans4) = 0;
	virtual unsigned int createGame() = 0;
	virtual void endGame(unsigned int gameId) = 0;
	virtual void submitAnswer(unsigned int gameId, unsigned int questionId, std::string username, unsigned int answer, bool correctAns) = 0;
};