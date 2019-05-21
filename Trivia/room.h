#pragma once
#include <vector>

#include "logged_user.h"
#include "room_data.h"

class Room
{
public:
    Room(RoomData& metadata);

    std::vector<LoggedUser> getAllUsers();

private:
    std::vector<LoggedUser> m_users;
    RoomData m_metadata;
};

