#pragma once
#include <string>

class IDatabase {
public:
	virtual ~IDatabase() {};

	virtual void initDatabase() = 0;
	virtual bool doesUserExist(std::string username) = 0;
	virtual void signUpUser(std::string username, std::string password, std::string email) = 0;
	virtual bool authUser(std::string username, std::string password) = 0;
};