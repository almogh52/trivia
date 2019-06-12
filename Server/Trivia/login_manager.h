#pragma once
#include <string>
#include <vector>

#include "logged_user.h"
#include "database_interface.h"

class LoginManager
{
public:
    LoginManager(std::shared_ptr<IDatabase> database);

    std::shared_ptr<LoggedUser> signup(std::string username, std::string password, std::string email);
    std::shared_ptr<LoggedUser> login(std::string username, std::string password);
    bool logout(std::string username);

private:
    std::shared_ptr<IDatabase> m_database;
    std::vector<LoggedUser> m_loggedUsers;
};

