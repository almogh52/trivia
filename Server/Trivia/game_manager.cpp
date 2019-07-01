#include "game_manager.h"

#include <iostream>
#include <sstream>
#include "HTTPRequest.hpp"

#include "exception.h"

GameManager::GameManager(std::shared_ptr<IDatabase> database) : m_database(database)
{
}

unsigned int GameManager::createGame(RoomData & room, std::vector<LoggedUser> players)
{
	// Create question for the room
	std::vector<Question> questions = createQuestions(room.questionCount);

	// Create a new game in the database
	unsigned int gameId = m_database->createGame();

	// Create a new game
	Game game = Game(gameId, questions, players, room.timePerQuestion * 1000);

	// Lock the games mutex
	gamesMutex.lock();

	// Add the game to the vector of games
	m_games.push_back(game);

	// Unlock the games mutex
	gamesMutex.unlock();

	return gameId;
}

bool GameManager::deleteGame(unsigned int gameId)
{
	bool deleted = false;

	// Lock the games mutex
	gamesMutex.lock();

	// Find the game
	auto game = findGame(gameId);

	// If the game can be deleted, delete it
	if (game->canBeDeleted())
	{
		m_games.erase(game);

		deleted = true;
	}

	// Unlock the games mutex
	gamesMutex.unlock();

	return deleted;
}

Question GameManager::getQuestionForUser(unsigned int gameId, const LoggedUser& user)
{
	Question question;

	// Lock the games mutex
	gamesMutex.lock();

	// Add the game to the vector of games
	question = findGame(gameId)->getQuestionForUser(user);

	// Unlock the games mutex
	gamesMutex.unlock();

	return question;
}

void GameManager::submitAnswer(unsigned int gameId, const LoggedUser & player, unsigned int answerId, unsigned int timeToAnswer)
{
	bool correct = false;
	unsigned int questionId = -1;

	// Lock the games mutex
	gamesMutex.lock();

	// Find the game
	auto game = findGame(gameId);

	// Submit the answer and get if it was correct or not
	correct = game->submitAnswer(player, answerId, timeToAnswer);

	// Get the question id of the current question of the user
	questionId = game->getQuestionForUser(player).getQuestionId();

	// Unlock the games mutex
	gamesMutex.unlock();

	// Submit the answer to the database
	m_database->submitAnswer(gameId, questionId, player.getUsername(), answerId, correct);
}

void GameManager::removePlayer(unsigned int gameId, const LoggedUser & player)
{
	// Lock the games mutex
	gamesMutex.lock();

	// Remove the player
	findGame(gameId)->removePlayer(player);

	// Unlock the games mutex
	gamesMutex.unlock();
}

std::vector<PlayerResults> GameManager::getPlayersResults(unsigned int gameId)
{
	std::vector<PlayerResults> playersResults;
	std::unordered_map<LoggedUser, GameData> playersData;

	// Lock the games mutex
	gamesMutex.lock();

	// If the game cannot be deleted (Game is not over), throw exception
	if (findGame(gameId)->canBeDeleted())
	{
		// Unlock the games mutex
		gamesMutex.unlock();

		throw Exception("Game isn't over yet!");
	}

	// Get the players data
	playersData = findGame(gameId)->getPlayersData();

	// Unlock the games mutex
	gamesMutex.unlock();

	// For each game data of player, create player results object
	for (auto gameDataPair : playersData)
	{
		PlayerResults playerResults;

		// Set the results of the player
		playerResults.username = gameDataPair.first.getUsername();
		playerResults.correctAnswerCount = gameDataPair.second.correctAnswerCount;
		playerResults.wrongAnswerCount = gameDataPair.second.wrongAnswerCount;
		playerResults.averageAnswerTime = gameDataPair.second.averageAnswerTime;

		// Push the player results to the vector of results
		playersResults.push_back(playerResults);
	}

	return playersResults;
}

std::vector<Question> GameManager::createQuestions(unsigned int amount)
{
	std::vector<Question> questions;
	nlohmann::json questionsRes;

	// Create a request to the trivia API
	try {
		http::Request req("https://opentdb.com/api.php?amount=" + std::to_string(amount) + "&difficulty=easy&encode=url3986");

		// Get a response from the server and parse it
		http::Response res = req.send();
		questionsRes = nlohmann::json::parse(res.body.data());
	}
	catch (...) {
		std::cout << "This server requires an active internet connection in order to fetch question!" << std::endl;

		throw Exception("This server requires an active internet connection in order to fetch question!");
	}

	// If an error occurred
	if (questionsRes["response_code"])
	{
		throw Exception("Unable to fetch question from the API!");
	}

	// For each question json in the results json
	for (nlohmann::json &questionJson : questionsRes["results"])
	{
		std::string question = decodeURLEncodedString(questionJson["question"]);
		std::string correctAns = decodeURLEncodedString(questionJson["correct_answer"]);
		std::string ans2 = decodeURLEncodedString(questionJson["incorrect_answers"][1]);
		std::string ans3 = decodeURLEncodedString(questionJson["incorrect_answers"][2]);
		std::string ans4 = decodeURLEncodedString(questionJson["incorrect_answers"][3]);

		// Create the question in the database
		unsigned int questionId = m_database->createQuestion(question, correctAns, ans2, ans3, ans4);

		// Create the question object and push it to the vector of question
		questions.push_back(Question(questionId, question, { correctAns, ans2, ans3, ans4 }, 0));
	}

	return questions;
}

std::string GameManager::decodeURLEncodedString(std::string encoded)
{
	char h;
	std::ostringstream escaped;
	escaped.fill('0');

	std::function<char(char)> from_hex = [](char ch) -> {
		return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
	};

	for (auto i = encoded.begin(), n = encoded.end(); i != n; ++i) {
		std::string::value_type c = (*i);

		if (c == '%') {
			if (i[1] && i[2]) {
				h = from_hex(i[1]) << 4 | from_hex(i[2]);
				escaped << h;
				i += 2;
			}
		}
		else if (c == '+') {
			escaped << ' ';
		}
		else {
			escaped << c;
		}
	}

	return escaped.str();
}

std::vector<Game>::iterator GameManager::findGame(unsigned int gameId)
{
	// Search for the game
	for (auto iterator = m_games.begin(); iterator != m_games.end(); iterator++)
	{
		// If found the game return it
		if (iterator->getGameId() == gameId)
		{
			return iterator;
		}
	}

	throw Exception("Game not found!");
}
