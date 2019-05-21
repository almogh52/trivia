#include "room_manager.h"

void RoomManager::createRoom(LoggedUser & user, std::string roomName, unsigned int maxPlayers, unsigned int timePerQuestion)
{
    RoomData roomMetadata;

    for (unsigned int i = 0; i < m_rooms.size(); i++)
    {	
	try {
	    // Try to find if there is a room with the id
	    m_rooms.at(i);
	}
	catch (...) {
	    // Set the id of the room
	    roomMetadata.id = i;

	    break;
	}
    }

    // Set the room's metadata
    roomMetadata.name = roomName;
    roomMetadata.maxPlayers = maxPlayers;
    roomMetadata.timePerQuestion = timePerQuestion;
    roomMetadata.isActive = true;

    // Save the new room in the map of rooms
    m_rooms[roomMetadata.id] = Room(roomMetadata);
}

bool RoomManager::deleteRoom(unsigned int roomId)
{
    // Try to delete the room
    if (!m_rooms.erase(roomId))
    {
	return false;
    }

    return true;
}
