#include "login_manager.h"

bool LoginManager::signup(std::string username, std::string password, std::string email)
{
    try {
	// Try to signup the user
	m_database.signup(username, password, email);
    }
    catch (...) {
	return false;
    }

    return true;
}

bool LoginManager::login(std::string username, std::string password)
{
    try {
	// Try to login the user
	return m_database.authUser(username, password);
    }
    catch (...) {
	return false;
    }

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
