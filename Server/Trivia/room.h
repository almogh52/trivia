#pragma once
#include <vector>

#include "logged_user.h"
#include "room_data.h"

class Room
{
public:
    Room();
    Room(RoomData& metadata);

    std::vector<LoggedUser> getAllUsers() const;
    bool addUser(const LoggedUser &user);
    bool removeUser(const LoggedUser &user);
	void setRoomState(bool state);

    RoomData getMetadata() const;

private:
    std::vector<LoggedUser> m_users;
    RoomData m_metadata;
};

