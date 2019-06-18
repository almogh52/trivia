#pragma once
#include <memory>

#include "highscore.h"
#include "database_interface.h"

class HighscoreTable
{
public:
	HighscoreTable(std::shared_ptr<IDatabase> database);

	std::vector<Highscore> getHighscores();

private:
	std::vector<Highscore> getAllScores();

	std::shared_ptr<IDatabase> m_database;
};

