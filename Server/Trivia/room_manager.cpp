#include "room_manager.h"

#include "exception.h"

int RoomManager::createRoom(const LoggedUser& user, std::string roomName, unsigned int maxPlayers, unsigned int timePerQuestion, unsigned int questionCount)
{
    RoomData roomMetadata;

	// Lock the rooms mutex
	roomsMutex.lock();

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

	// Unlock the room mutex
	roomsMutex.unlock();

    // Set the room's metadata
    roomMetadata.name = roomName;
    roomMetadata.maxPlayers = maxPlayers;
    roomMetadata.timePerQuestion = timePerQuestion;
	roomMetadata.questionCount = questionCount;
    roomMetadata.isActive = true;

    // Save the new room in the map of rooms
    m_rooms[roomMetadata.id] = Room(roomMetadata);

	return roomMetadata.id;
}

bool RoomManager::deleteRoom(unsigned int roomId)
{
	// Lock the rooms mutex
	roomsMutex.lock();

    // Try to delete the room
    if (!m_rooms.erase(roomId))
    {
		return false;
    }

	// Unlock the room mutex
	roomsMutex.unlock();

    return true;
}

bool RoomManager::joinRoom(const LoggedUser& user, unsigned int roomId)
{
	bool success;

	// Lock the rooms mutex
	roomsMutex.lock();

	try {
		// Try to add the user to the room
		success = m_rooms.at(roomId).addUser(user);
	}
	catch (...) {
		// Unlock the room mutex
		roomsMutex.unlock();

		throw Exception("No room with the id " + std::to_string(roomId));
	}

	// Unlock the room mutex
	roomsMutex.unlock();

	return success;
}

bool RoomManager::getRoomState(unsigned int roomId)
{
	bool isActive;

	// Lock the rooms mutex
	roomsMutex.lock();

    try {
		isActive = m_rooms.at(roomId).getMetadata().isActive;
    }
    catch (...) {
		// Unlock the room mutex
		roomsMutex.unlock();

		throw Exception("No room with the id " + std::to_string(roomId));
    }

	// Unlock the room mutex
	roomsMutex.unlock();

	return isActive;
}

std::vector<std::string> RoomManager::getPlayersInRoom(unsigned int roomId)
{
	std::vector<std::string> players;
	std::vector<LoggedUser> users;

	// Lock the rooms mutex
	roomsMutex.lock();

	try {
		users = m_rooms.at(roomId).getAllUsers();
	}
	catch (...) {
		// Unlock the room mutex
		roomsMutex.unlock();

		throw Exception("No room with the id " + std::to_string(roomId));
	}

	// Unlock the room mutex
	roomsMutex.unlock();

	// For each user get it's name
	for (LoggedUser& user : users)
	{
		players.push_back(user.getUsername());
	}

	return players;
}

std::vector<RoomData> RoomManager::getRooms()
{
    std::vector<RoomData> rooms;

	// Lock the rooms mutex
	roomsMutex.lock();

    // For each room insert it's room metadata to the rooms vector
    for (auto& roomPair : m_rooms)
    {
		// Get the data of the room and insert it to the list of rooms
		rooms.push_back(roomPair.second.getMetadata());
    }

	// Unlock the room mutex
	roomsMutex.unlock();

    return rooms;
}

Room RoomManager::getRoom(unsigned int roomId)
{
	// Lock the rooms mutex
	roomsMutex.lock();

	try {
		return m_rooms.at(roomId);
	}
	catch (...) {
		// Unlock the room mutex
		roomsMutex.unlock();

		throw Exception("No room with the id " + std::to_string(roomId));
	}

	// Unlock the room mutex
	roomsMutex.unlock();
}
