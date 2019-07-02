#pragma once
#include <unordered_map>

#include "logged_user.h"
#include "game_data.h"
#include "question.h"

class Game
{
public:
	Game(unsigned int id, std::vector<Question> questions, std::vector<LoggedUser> players, unsigned int answerTimeout);

	Question getQuestionForUser(const LoggedUser& user);
	bool submitAnswer(const LoggedUser& player, unsigned int answerId, unsigned int timeToAnswer);
	void removePlayer(const LoggedUser& player);

	bool canBeDeleted();
	bool gameOver();
	unsigned int getGameId();
	std::unordered_map<LoggedUser, GameData> getPlayersData();

private:
	unsigned int m_id;
	unsigned int m_answerTimeout;
	std::vector<Question> m_questions;
	std::unordered_map<LoggedUser, GameData> m_players;
};

