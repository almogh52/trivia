#pragma once
#include <unordered_map>
#include <string>
#include <mutex>

#include "room_data.h"
#include "room.h"

class RoomManager
{
public:
    int createRoom(const LoggedUser& user, std::string roomName, unsigned int maxPlayers, unsigned int timePerQuestion, unsigned int questionCount);
    bool deleteRoom(unsigned int roomId);

	bool joinRoom(const LoggedUser& user, unsigned int roomId);
	bool leaveRoom(const LoggedUser& user, unsigned int roomId);

    bool getRoomState(unsigned int roomId);
	
	void startGame(unsigned int roomId, unsigned int gameId);
	unsigned int getGameIdOfRoom(unsigned int roomId);
	void deleteRoomWithGameId(unsigned int gameId);

	std::vector<std::string> getPlayersInRoom(unsigned int roomId);
	std::vector<LoggedUser> getLoggedPlayersInRoom(unsigned int roomId);
    std::vector<RoomData> getRooms();

	RoomData getRoomData(unsigned int roomId);

private:
	std::unordered_map<unsigned int, unsigned int> m_roomsGames;
    std::unordered_map<unsigned int, Room> m_rooms;
	std::mutex roomsMutex;
};

