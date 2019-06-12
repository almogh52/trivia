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
    
    // Lock the users mutex
    usersMutex.lock();

    // Add the user to the logged users list
    m_loggedUsers.push_back(user);

    // Unlock the users mutex
    usersMutex.unlock();

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

    // Lock the users mutex
    usersMutex.lock();

    // Add the user to the logged users list
    m_loggedUsers.push_back(user);

    // Unlock the users mutex
    usersMutex.unlock();

    return std::shared_ptr<LoggedUser>(new LoggedUser(user));
}

bool LoginManager::logout(LoggedUser user)
{
    // Try to find the user in the logged users vector
    auto userIterator = find(m_loggedUsers.begin(), m_loggedUsers.end(), user);

    // If the user isn't found in the logged users vector, return false
    if (userIterator == m_loggedUsers.end())
    {
	return false;
    }

    // Remove the user from the users vector
    m_loggedUsers.erase(userIterator);

    return true;
}
