#pragma once
#include <unordered_map>
#include <string>

#include "room_data.h"
#include "room.h"

class RoomManager
{
public:
    void createRoom(LoggedUser& user, std::string roomName, unsigned int maxPlayers, unsigned int timePerQuestion);
    bool deleteRoom(unsigned int roomId);
	bool joinRoom(const LoggedUser& user, unsigned int roomId);

    bool getRoomState(unsigned int roomId) const;
	
	std::vector<std::string> getPlayersInRoom(unsigned int roomId) const;
    std::vector<RoomData> getRooms() const;

private:
    std::unordered_map<unsigned int, Room> m_rooms;
};

