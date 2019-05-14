#include "logged_user.h"

LoggedUser::LoggedUser(std::string username) : m_username(username)
{
}

std::string LoggedUser::getUsername()
{
    return m_username;
}
