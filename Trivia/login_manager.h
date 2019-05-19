#pragma once
#include <string>
#include <vector>

#include "logged_user.h"
#include "database_interface.h"

class LoginManager
{
public:
    LoginManager(std::shared_ptr<IDatabase> database);

    bool signup(std::string username, std::string password, std::string email);
    bool login(std::string username, std::string password);
    void logout(std::string username);

private:
    std::shared_ptr<IDatabase> m_database;
    std::vector<LoggedUser> m_loggedUsers;
};

