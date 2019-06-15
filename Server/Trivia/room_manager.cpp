#include "room_manager.h"

#include "exception.h"

void RoomManager::createRoom(LoggedUser & user, std::string roomName, unsigned int maxPlayers, unsigned int timePerQuestion)
{
    RoomData roomMetadata;

    for (unsigned int i = 0; i <= m_rooms.size(); i++)
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

bool RoomManager::joinRoom(const LoggedUser& user, unsigned int roomId)
{
	try {
		return m_rooms.at(roomId).addUser(user);
	}
	catch (...) {
		throw Exception("No room with the id " + std::to_string(roomId));
	}
}

bool RoomManager::getRoomState(unsigned int roomId) const
{
    try {
		return m_rooms.at(roomId).getMetadata().isActive;
    }
    catch (...) {
		throw Exception("No room with the id " + std::to_string(roomId));
    }
}

std::vector<std::string> RoomManager::getPlayersInRoom(unsigned int roomId) const
{
	std::vector<std::string> players;
	std::vector<LoggedUser> users;

	try {
		users = m_rooms.at(roomId).getAllUsers();
	}
	catch (...) {
		throw Exception("No room with the id " + std::to_string(roomId));
	}

	// For each user get it's name
	for (LoggedUser& user : users)
	{
		players.push_back(user.getUsername());
	}

	return players;
}

std::vector<RoomData> RoomManager::getRooms() const
{
    std::vector<RoomData> rooms;

    // For each room insert it's room metadata to the rooms vector
    for (auto& roomPair : m_rooms)
    {
		// Get the data of the room and insert it to the list of rooms
		rooms.push_back(roomPair.second.getMetadata());
    }

    return rooms;
}
