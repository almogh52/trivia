#include "room.h"

#include <algorithm>

Room::Room()
{
}

Room::Room(RoomData & metadata) : m_metadata(metadata)
{
}

std::vector<LoggedUser> Room::getAllUsers() const
{
    return m_users;
}

bool Room::addUser(const LoggedUser & user)
{
    // Search for the user in the logged users vector, if found, return false
    if (find(m_users.begin(), m_users.end(), user) != m_users.end() || m_users.size() >= m_metadata.maxPlayers)
    {
		return false;
    }

    // Push the user to the vector of users
    m_users.push_back(user);

    return true;
}

bool Room::removeUser(LoggedUser & user)
{
    auto userIterator = find(m_users.begin(), m_users.end(), user);

    // If the user isn't found
    if (userIterator == m_users.end())
    {
	return false;
    }

    // Erase the user from the vector
    m_users.erase(userIterator);
    return true;
}

RoomData Room::getMetadata() const
{
    return m_metadata;
}
