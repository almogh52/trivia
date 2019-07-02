#include "logged_user.h"

LoggedUser::LoggedUser(std::string username) : m_username(username)
{
}

/**
 * Gets the username
 *
 * @return The username
 */
std::string LoggedUser::getUsername() const
{
    return m_username;
}
