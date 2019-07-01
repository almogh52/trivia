#include "game.h"

#include <algorithm>

Game::Game(unsigned int id, std::vector<Question> questions, std::vector<LoggedUser> players, unsigned int answerTimeout)
	: m_id(id), m_questions(questions), m_answerTimeout(answerTimeout)
{
	GameData gameData = { m_questions[0], 0, 0, 0, false };

	// For each player set the default game data
	for (LoggedUser& player : players)
	{
		m_players[player] = gameData;
	}
}

Question Game::getQuestionForUser(const LoggedUser & user)
{
	return m_players[user].currentQuestion;
}

bool Game::submitAnswer(const LoggedUser & player, unsigned int answerId, unsigned int timeToAnswer)
{
	GameData gameData = m_players[player];
	unsigned int amountOfAnswers = gameData.correctAnswerCount + gameData.wrongAnswerCount + 1;
	bool correct = answerId == gameData.currentQuestion.getCorrectAnswer() && timeToAnswer <= m_answerTimeout;

	unsigned int currentQuestionIdx = find(m_questions.begin(), m_questions.end(), gameData.currentQuestion) - m_questions.begin();

	// Check if the user was correct and he didn't pass the answer timeout
	if (correct) {
		gameData.correctAnswerCount++;
	}
	else {
		gameData.wrongAnswerCount++;
	}

	// Re-calculate the average answer time
	gameData.averageAnswerTime = (gameData.averageAnswerTime * (amountOfAnswers - 1) + timeToAnswer) / amountOfAnswers;

	// Set the next question if not reached the end
	gameData.currentQuestion = (currentQuestionIdx == m_questions.size() - 1) ? Question() : m_questions[currentQuestionIdx + 1];

	return correct;
}

void Game::removePlayer(const LoggedUser & player)
{
	m_players[player].playerLeft = true;
}

bool Game::canBeDeleted()
{
	bool clear = true;

	// Check if all the players have left
	for (auto gamePair : m_players)
	{
		if (!gamePair.second.playerLeft)
		{
			clear = false;
		}
	}

	return clear;
}

unsigned int Game::getGameId()
{
	return m_id;
}
 