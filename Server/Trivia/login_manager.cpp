#include "login_manager.h"

#include <regex>

LoginManager::LoginManager(std::shared_ptr<IDatabase> database) : m_database(database)
{
}

bool LoginManager::isValidEmail(std::string email) const
{
    const std::regex emailPattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");

    // Check if the email matches the regex pattern
    return std::regex_match(email, emailPattern);
}

std::shared_ptr<LoggedUser> LoginManager::signup(std::string username, std::string password, std::string email)
{
    LoggedUser user(username);

    // If the username, password or email are invalid return null
    if (username.length() == 0 || password.length() == 0 || !isValidEmail(email))
    {
	return nullptr;
    }

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

    // If the username or passsword are invalid return null
    if (username.length() == 0 || password.length() == 0)
    {
	return nullptr;
    }

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
