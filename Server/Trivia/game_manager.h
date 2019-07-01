#pragma once
#include <vector>
#include <memory>

#include "database_interface.h"
#include "game.h"
#include "room.h"
#include "question.h"

class GameManager
{
public:
	GameManager(std::shared_ptr<IDatabase> database);

	unsigned int createGame(Room& room);

private:
	std::shared_ptr<IDatabase> m_database;
	std::vector<Game> m_games;

	std::vector<Question> createQuestions(unsigned int amount);
	std::string decodeURLEncodedString(std::string encoded);
};

