#pragma once
#include <unordered_map>
#include <string>

#include "room.h"

class RoomManager
{
public:
    void createRoom(LoggedUser& user, std::string roomName, unsigned int maxPlayers, unsigned int timePerQuestion);
    bool deleteRoom(unsigned int roomId);

private:
    std::unordered_map<unsigned int, Room> m_rooms;
};

