#pragma once
#include <string>
#include <vector>

#include "logged_user.h"
#include "database_interface.h"

class LoginManager
{
public:
    bool signup(std::string username, std::string password, std::string email);
    bool login(std::string username, std::string password);
    void logout(std::string username);

private:
    IDatabase *m_database;
    std::vector<LoggedUser> m_loggedUsers;
};

