#pragma once
#include <vector>
#include <memory>
#include <mutex>

#include "database_interface.h"
#include "game.h"
#include "room.h"
#include "question.h"
#include "logged_user.h"

class GameManager
{
public:
	GameManager(std::shared_ptr<IDatabase> database);

	unsigned int createGame(RoomData& room, std::vector<LoggedUser> players);

private:
	std::shared_ptr<IDatabase> m_database;
	std::vector<Game> m_games;
	std::mutex gamesMutex;

	std::vector<Question> createQuestions(unsigned int amount);
	std::string decodeURLEncodedString(std::string encoded);
};

