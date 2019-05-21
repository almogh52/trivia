#include "room.h"

Room::Room(RoomData & metadata) : m_metadata(metadata)
{
}

std::vector<LoggedUser> Room::getAllUsers()
{
    return m_users;
}
