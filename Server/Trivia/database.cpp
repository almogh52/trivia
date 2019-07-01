#include "database.h"

#include <io.h>
#include <string>
#include <regex>
#include <iostream>

#include "exception.h"

Database::Database() : m_db(nullptr)
{
}

Database::~Database()
{
	// Close the database
	closeDatabase();
}

void Database::initDatabase()
{
	const char* usersTableQuery = "CREATE TABLE users (username TEXT NOT NULL PRIMARY KEY UNIQUE, password TEXT NOT NULL, email TEXT NOT NULL UNIQUE);";
	const char* questionsTableQuery = "CREATE TABLE questions (question_id INTEGER NOT NULL PRIMARY KEY, question TEXT NOT NULL UNIQUE, correct_ans TEXT NOT NULL, ans2 TEXT NOT NULL, ans3 TEXT NOT NULL, ans4 TEXT NOT NULL);";
	const char* answersTableQuery = "CREATE TABLE answers (username TEXT NOT NULL REFERENCES users(username), game_id INTEGER NOT NULL REFERENCES games(game_id), question_id INTEGER NOT NULL REFERENCES questions(question_id), answer INTEGER NOT NULL, correct_ans INTEGER NOT NULL, PRIMARY KEY(username, game_id, question_id));";
	const char* gamesTableQuery = "CREATE TABLE games (game_id INTEGER NOT NULL PRIMARY KEY, start_time DATETIME NOT NULL, end_time DATETIME);";

	int res = 0;

	// Check if the database already exists
	int databaseExists = _access(DB_FILENAME, 0);

	// Open the sqlite db
	res = sqlite3_open(DB_FILENAME, &m_db);
	if (res != SQLITE_OK)
	{
		throw Exception("Unable to connect to SQLite DB!");
	}

	// Enable foreign keys
	res = sqlite3_exec(m_db, "PRAGMA foreign_keys=ON", nullptr, nullptr, nullptr);
	if (res != SQLITE_OK)
	{
		closeDatabase();
		throw Exception("Unable to enable foreign keys!");
	}

	// If the database didn't exist, initialize it
	if (databaseExists == -1)
	{
		// Create the users table
		res = sqlite3_exec(m_db, usersTableQuery, nullptr, nullptr, nullptr);
		if (res != SQLITE_OK)
		{
			closeDatabase();
			throw Exception("Unable to create users table!");
		}

		// Create the questions table
		res = sqlite3_exec(m_db, questionsTableQuery, nullptr, nullptr, nullptr);
		if (res != SQLITE_OK)
		{
			closeDatabase();
			throw Exception("Unable to create questions table!");
		}

		// Create the answers table
		res = sqlite3_exec(m_db, answersTableQuery, nullptr, nullptr, nullptr);
		if (res != SQLITE_OK)
		{
			closeDatabase();
			throw Exception("Unable to create answers table!");
		}

		// Create the games table
		res = sqlite3_exec(m_db, gamesTableQuery, nullptr, nullptr, nullptr);
		if (res != SQLITE_OK)
		{
			closeDatabase();
			throw Exception("Unable to create games table!");
		}
	}
}

void Database::closeDatabase()
{
	// Close the SQLite DB
	sqlite3_close(m_db);
	m_db = nullptr;
}

int exist_callback(void *data, int argc, char **argv, char **colNames)
{
	bool *exists = (bool *)data;

	// Check if it exists
	*exists = std::stoi(argv[0]);

	return 0;
}

int string_callback(void *data, int argc, char **argv, char **colNames)
{
    std::string *str = (std::string *)data;

    // Get the string value of the first res
    *str = argv[0];

    return 0;
}

int int_callback(void *data, int argc, char **argv, char **colNames)
{
	int *integer = (int *)data;

	// Get the int value of the first res
	*integer = std::stoi(argv[0]);

	return 0;
}

bool Database::doesUserExist(std::string username)
{
	int res = 0;
	bool exists = false;
	std::string userExistQuery = "SELECT EXISTS(SELECT 1 FROM users WHERE username = ':username');";

	// Bind parameters
	userExistQuery = std::regex_replace(userExistQuery, std::regex(":username"), username);

	// Check if the row exists
	res = sqlite3_exec(m_db, userExistQuery.c_str(), exist_callback, &exists, nullptr);
	if (res != SQLITE_OK)
	{
		return false;
	}

	return exists;
}

void Database::signUpUser(std::string username, std::string password, std::string email)
{
	std::string userInsertQuery = "INSERT INTO users(username, password, email) VALUES(':username', ':password', ':email');";

	int res = 0;

	// If the user already exists, throw error
	if (doesUserExist(username))
	{
		throw Exception("The user " + username + " already exists!");
	}

	// Bind parameters
	userInsertQuery = std::regex_replace(userInsertQuery, std::regex(":username"), username);
	userInsertQuery = std::regex_replace(userInsertQuery, std::regex(":password"), password);
	userInsertQuery = std::regex_replace(userInsertQuery, std::regex(":email"), email);

	// Try to execute the user insert query
	res = sqlite3_exec(m_db, userInsertQuery.c_str(), nullptr, nullptr, nullptr);
	if (res != SQLITE_OK)
	{
		throw Exception("Unable to sign up the user!");
	}
}

bool Database::authUser(std::string username, std::string password)
{
    std::string userPasswordQuery = "SELECT password FROM users WHERE username = ':username';";

    int res = 0;
    std::string userPassword;

    // If the user doesn't exists, throw error
    if (!doesUserExist(username))
    {
		throw Exception("The user " + username + " doesn't exists!");
    }

    // Bind parameters
    userPasswordQuery = std::regex_replace(userPasswordQuery, std::regex(":username"), username);

    // Try to execute the user password query
    res = sqlite3_exec(m_db, userPasswordQuery.c_str(), string_callback, &userPassword, nullptr);
    if (res != SQLITE_OK)
    {
		throw Exception("Unable to sign up the user!");
    }

    // Check if entered correct password
    return password == userPassword;
}

int score_callback(void *data, int argc, char **argv, char **colNames)
{
	auto playersGamesScores = (std::unordered_map<std::string, std::unordered_map<int, int>> *)data;
	
	std::string username = argv[0];
	int game_id = std::stoi(argv[1]);
	bool corrent_ans = std::stoi(argv[2]);

	// If the user was correct, increase the user's score by 1
	if (corrent_ans)
	{
		// Check if the dest cell exists already
		bool exists = playersGamesScores->count(username) && playersGamesScores->at(username).count(game_id);

		// Update the dest cell by handling if it exists or not
		(*playersGamesScores)[username][game_id] = exists ? (*playersGamesScores)[username][game_id] + 1 : 1;
	}

	return 0;
}

std::unordered_map<std::string, std::unordered_map<int, int>> Database::getAllScores()
{
	std::unordered_map<std::string, std::unordered_map<int, int>> playersGamesScores;
	
	int res = 0;
	std::string scoreQuery = "SELECT username, game_id, correct_ans FROM answers";

	// Try to execute the user password query
	res = sqlite3_exec(m_db, scoreQuery.c_str(), score_callback, &playersGamesScores, nullptr);
	if (res != SQLITE_OK)
	{
		throw Exception("Unable to get the scores!");
	}

	return playersGamesScores;
}

unsigned int Database::createQuestion(std::string question, std::string correctAns, std::string ans2, std::string ans3, std::string ans4)
{
	unsigned int questionId = 0;

	int res = 0;
	std::string insertQuestionQuery("INSERT INTO questions(question, correct_ans, ans2, ans3, ans4) VALUES(:question, :correct_ans, :ans2, :ans3, :ans4)");
	std::string getQuestionIdQuery("SELECT question_id FROM questions WHERE question = :question");

	// Bind parameters
	insertQuestionQuery = std::regex_replace(insertQuestionQuery, std::regex(":question"), question);
	insertQuestionQuery = std::regex_replace(insertQuestionQuery, std::regex(":correct_ans"), correctAns);
	insertQuestionQuery = std::regex_replace(insertQuestionQuery, std::regex(":ans2"), ans2);
	insertQuestionQuery = std::regex_replace(insertQuestionQuery, std::regex(":ans3"), ans3);
	insertQuestionQuery = std::regex_replace(insertQuestionQuery, std::regex(":ans4"), ans4);
	getQuestionIdQuery = std::regex_replace(getQuestionIdQuery, std::regex(":question"), question);

	// Try to insert the question to the database
	sqlite3_exec(m_db, insertQuestionQuery.c_str(), nullptr, nullptr, nullptr);

	// Try to get the id of the question
	res = sqlite3_exec(m_db, getQuestionIdQuery.c_str(), int_callback, &questionId, nullptr);
	if (res != SQLITE_OK)
	{
		throw Exception("Unable to get the id of the question!");
	}

	return questionId;
}

unsigned int Database::createGame()
{
	unsigned int gameId = 0;

	int res = 0;
	std::string insertGameQuery("INSERT INTO games(start_time) VALUES(:start_time)");
	std::string getGameIdQuery("SELECT seq FROM sqlite_sequence WHERE name = 'games'");

	// Bind parameters
	insertGameQuery = std::regex_replace(insertGameQuery, std::regex(":start_time"), std::to_string(std::time(nullptr)));

	// Try to insert the game to the database
	res = sqlite3_exec(m_db, insertGameQuery.c_str(), nullptr, nullptr, nullptr);
	if (res != SQLITE_OK)
	{
		throw Exception("Unable to create a new game!");
	}

	// Try to get the id of the game
	res = sqlite3_exec(m_db, getGameIdQuery.c_str(), int_callback, &gameId, nullptr);
	if (res != SQLITE_OK)
	{
		throw Exception("Unable to get the id of the game!");
	}

	return gameId;
}

void Database::endGame(unsigned int gameId)
{
	int res = 0;
	std::string endGameQuery("UPDATE games WHERE game_id = :game_id SET end_time = :end_time");

	// Bind parameters
	endGameQuery = std::regex_replace(endGameQuery, std::regex(":game_id"), std::to_string(gameId));
	endGameQuery = std::regex_replace(endGameQuery, std::regex(":end_time"), std::to_string(std::time(nullptr)));

	// Try to end the game
	res = sqlite3_exec(m_db, endGameQuery.c_str(), nullptr, nullptr, nullptr);
	if (res != SQLITE_OK)
	{
		throw Exception("Unable to end the game!");
	}
}

void Database::submitAnswer(unsigned int gameId, unsigned int questionId, std::string username, unsigned int answer, bool correctAns)
{
	int res = 0;
	std::string submitAnswerQuery("INSERT INTO games(username, game_id, question_id, answer, correct_answer) VALUES(:username, :game_id, :question_id, :answer, :correct_answer)");

	// Bind parameters
	submitAnswerQuery = std::regex_replace(submitAnswerQuery, std::regex(":username"), username);
	submitAnswerQuery = std::regex_replace(submitAnswerQuery, std::regex(":game_id"), std::to_string(gameId));
	submitAnswerQuery = std::regex_replace(submitAnswerQuery, std::regex(":question_id"), std::to_string(questionId));
	submitAnswerQuery = std::regex_replace(submitAnswerQuery, std::regex(":answer"), std::to_string(answer));
	submitAnswerQuery = std::regex_replace(submitAnswerQuery, std::regex(":correct_ans"), std::to_string(correctAns));

	// Try to submit the answer
	res = sqlite3_exec(m_db, submitAnswerQuery.c_str(), nullptr, nullptr, nullptr);
	if (res != SQLITE_OK)
	{
		throw Exception("Unable to submit the answer!");
	}
}
