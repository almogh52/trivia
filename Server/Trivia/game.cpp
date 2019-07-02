#include "game.h"

#include <algorithm>

/**
 * Game's c'tor
 *
 * @param id The id of the game
 * @param question The list of questions
 * @param players The players in the game
 * @param answerTimeout The time per question
 * @return None
 */
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

/**
 * Gets the current question for a user
 *
 * @param user The user
 * @return The question for the user
 */
Question Game::getQuestionForUser(const LoggedUser & user)
{
	return m_players[user].currentQuestion;
}

/**
 * Submits a user answer
 *
 * @param player The user
 * @param answerId The id of the answer
 * @param timeToAnswer The time it took the player to answer
 * @return Is the answer was correct or not
 */
bool Game::submitAnswer(const LoggedUser & player, unsigned int answerId, unsigned int timeToAnswer)
{
	GameData gameData = m_players[player];
	unsigned int amountOfAnswers = gameData.correctAnswerCount + gameData.wrongAnswerCount + 1;
	bool correct = answerId == gameData.currentQuestion.getCorrectAnswer() && timeToAnswer <= m_answerTimeout;

	unsigned int currentQuestionIdx = (unsigned int)(find(m_questions.begin(), m_questions.end(), gameData.currentQuestion) - m_questions.begin());

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
	gameData.currentQuestion = (currentQuestionIdx >= m_questions.size() - 1) ? Question() : m_questions[currentQuestionIdx + 1];

	// Save game data
	m_players[player] = gameData;

	return correct;
}

/**
 * Removes a player from the game
 *
 * @param player The user
 * @return None
 */
void Game::removePlayer(const LoggedUser & player)
{
	m_players[player].playerLeft = true;
}

/**
 * Checks if the game can be deleted
 *
 * @return Can the game be deleted
 */
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

/**
 * Checks if the game is over
 *
 * @return Is the game over
 */
bool Game::gameOver()
{
	bool clear = true;

	// Check if all the players have left or finished
	for (auto gamePair : m_players)
	{
		if (!gamePair.second.playerLeft && (gamePair.second.correctAnswerCount + gamePair.second.wrongAnswerCount) != m_questions.size())
		{
			clear = false;
		}
	}

	return clear;
}

/**
 * Gets the game id
 *
 * @return The game id
 */
unsigned int Game::getGameId()
{
	return m_id;
}

/**
 * Gets the players data
 *
 * @return The players data
 */
std::unordered_map<LoggedUser, GameData> Game::getPlayersData()
{
	return m_players;
}
 