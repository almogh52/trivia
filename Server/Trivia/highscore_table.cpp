#include "highscore_table.h"

#include <algorithm>
#include <iostream>

HighscoreTable::HighscoreTable(std::shared_ptr<IDatabase> database) : m_database(database)
{
}

std::vector<Highscore> HighscoreTable::getHighscores()
{
	std::vector<Highscore> allScores = getAllScores();
	std::vector<Highscore> highscores;
	
	// Get the 10 best scores
	if (allScores.size() > 10)
	{
		highscores.insert(highscores.end(), allScores.begin(), allScores.begin() + 10);
	}
	else {
		highscores = allScores;
	}

	return highscores;
}

bool compareHighscores(Highscore &s1, Highscore &s2)
{
	return (s1.score > s2.score);
}

std::vector<Highscore> HighscoreTable::getAllScores()
{
	std::vector<Highscore> highscores;
	std::unordered_map<std::string, std::unordered_map<int, int>> playersGamesScores = m_database->getAllScores();

	// Get the highest score for each player
	for (auto playerScores : playersGamesScores)
	{
		int bestScore = -1;
		Highscore highscore;

		// For each score, check if it's the best
		for (auto gameScore : playerScores.second)
		{
			// If it's higher than the current best score set it as the best score
			if (gameScore.second > bestScore)
			{
				bestScore = gameScore.second;
			}
		}

		// Set the highscore fields
		highscore.name = playerScores.first;
		highscore.score = bestScore;

		// Add the highscore of the user to the vector of highscores
		highscores.push_back(highscore);
	}

	// Sort the highscores using the compare function
	sort(highscores.begin(), highscores.end(), compareHighscores);

	return highscores;
}