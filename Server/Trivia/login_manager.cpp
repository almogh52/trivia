#include "login_manager.h"

LoginManager::LoginManager(std::shared_ptr<IDatabase> database) : m_database(database)
{
}

std::shared_ptr<LoggedUser> LoginManager::signup(std::string username, std::string password, std::string email)
{
    LoggedUser user(username);

    try {
	// Try to signup the user
	m_database->signUpUser(username, password, email);
    }
    catch (...) {
	return nullptr;
    }
    
    // Add the user to the logged users list
    m_loggedUsers.push_back(user);

    return std::shared_ptr<LoggedUser>(new LoggedUser(user));
}

std::shared_ptr<LoggedUser> LoginManager::login(std::string username, std::string password)
{
    LoggedUser user(username);

    try {
	// Try to login the user
	m_database->authUser(username, password);
    }
    catch (...) {
	return nullptr;
    }

    // Add the user to the logged users list
    m_loggedUsers.push_back(user);

    return std::shared_ptr<LoggedUser>(new LoggedUser(user));
}

bool LoginManager::logout(std::string username)
{
    auto userIterator = m_loggedUsers.begin();

    // Search for the logged user
    for (; userIterator != m_loggedUsers.end(); userIterator++)
    {
	if (userIterator->getUsername() == username)
	{
	    // Remove the user from the logged users vector
	    m_loggedUsers.erase(userIterator);
	    break;
	}
    }

    // If the user isn't found in the logged users vector, return false
    if (userIterator == m_loggedUsers.end())
    {
	return false;
    }

    return true;
}
