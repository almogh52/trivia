#pragma once
#include <unordered_map>

#include "logged_user.h"
#include "game_data.h"
#include "question.h"

class Game
{
public:
	Game(unsigned int id, std::vector<Question> questions, std::vector<LoggedUser> players);

	Question getQuestionForUser(LoggedUser& user);
	void submitAnswer(LoggedUser& player, unsigned int answerId, unsigned int timeToAnswer);
	void removePlayer(LoggedUser& player);
	unsigned int getGameId();

private:
	unsigned int m_id;
	std::vector<Question> m_questions;
	std::unordered_map<LoggedUser, GameData> m_players;
};

