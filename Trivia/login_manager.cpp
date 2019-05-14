#include "login_manager.h"

bool LoginManager::signup(std::string username, std::string password, std::string email)
{
    return false;
}

bool LoginManager::login(std::string username, std::string password)
{
    return false;
}

void LoginManager::logout(std::string username)
{
    // Search for the logged user
    for (auto userIterator = m_loggedUsers.begin(); userIterator != m_loggedUsers.end(); userIterator++)
    {
	if (userIterator->getUsername() == username)
	{
	    // Remove the user from the logged users vector
	    m_loggedUsers.erase(userIterator);
	    break;
	}
    }
}
