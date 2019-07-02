#include "database.h"

#include <io.h>
#include <string>
#include <regex>
#include <iostream>

#include "exception.h"

/**
 * Database's c'tor
 *
 * @return None
 */
Database::Database() : m_db(nullptr)
{
}

/**
 * Database's d'tor
 *
 * @return None
 */
Database::~Database()
{
	// Close the database
	closeDatabase();
}

/**
 * Initiates the database
 *
 * @return None
 */
void Database::initDatabase()
{
	const char* usersTableQuery = "CREATE TABLE users (username TEXT NOT NULL PRIMARY KEY UNIQUE, password TEXT NOT NULL, email TEXT NOT NULL UNIQUE);";
	const char* questionsTableQuery = "CREATE TABLE questions (question_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, question TEXT NOT NULL UNIQUE, correct_ans TEXT NOT NULL, ans2 TEXT NOT NULL, ans3 TEXT NOT NULL, ans4 TEXT NOT NULL);";
	const char* answersTableQuery = "CREATE TABLE answers (username TEXT NOT NULL REFERENCES users(username), game_id INTEGER NOT NULL REFERENCES games(game_id), question_id INTEGER NOT NULL REFERENCES questions(question_id), answer INTEGER NOT NULL, correct_ans INTEGER NOT NULL, PRIMARY KEY(username, game_id, question_id));";
	const char* gamesTableQuery = "CREATE TABLE games (game_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, start_time DATETIME NOT NULL, end_time DATETIME);";

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

/**
 * Closes the database
 *
 * @return None
 */
void Database::closeDatabase()
{
	// Close the SQLite DB
	sqlite3_close(m_db);
	m_db = nullptr;
}

/**
 * Is a row exists callback
 *
 * @param data The pointer to the bool
 * @param argc The amount of arguments
 * @param argv The arguments
 * @param colNames The columns names
 * @return Exit code
 */
int exist_callback(void *data, int argc, char **argv, char **colNames)
{
	bool *exists = (bool *)data;

	// Check if it exists
	*exists = std::stoi(argv[0]);

	return 0;
}

/**
 * String extract callback
 *
 * @param data The pointer to the string
 * @param argc The amount of arguments
 * @param argv The arguments
 * @param colNames The columns names
 * @return Exit code
 */
int string_callback(void *data, int argc, char **argv, char **colNames)
{
    std::string *str = (std::string *)data;

    // Get the string value of the first res
    *str = argv[0];

    return 0;
}

/**
 * Int extract callback
 *
 * @param data The pointer to the int
 * @param argc The amount of arguments
 * @param argv The arguments
 * @param colNames The columns names
 * @return Exit code
 */
int int_callback(void *data, int argc, char **argv, char **colNames)
{
	int *integer = (int *)data;

	// Get the int value of the first res
	*integer = std::stoi(argv[0]);

	return 0;
}

/**
 * Checks if a user exists
 *
 * @param username The username of the user
 * @return Is the user exists
 */
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

/**
 * Signs up a user
 *
 * @param username The username of the user
 * @param password The password of the user
 * @param email The email of the user
 * @return None
 */
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

/**
 * Authenticates a user
 *
 * @param username The username of the user
 * @param password The password of the user
 * @return Did the authentication was successful or a failure
 */
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

/**
 * Extract Score callback
 *
 * @param data The pointer to the players games scores
 * @param argc The amount of arguments
 * @param argv The arguments
 * @param colNames The columns names
 * @return Exit code
 */
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

/**
 * Get all players' scores
 *
 * @return A map with the user that each row contains a map of it's games and scores
 */
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

/**
 * Escapes a string
 *
 * @param str The string to be escaped
 * @return The string escaped
 */
std::string escapeString(std::string str)
{
	size_t index = 0;
	while (true) {
		/* Locate the substring to replace. */
		index = str.find("'", index);
		if (index == std::string::npos) break;

		/* Make the replacement. */
		str.replace(index, 3, "''");

		/* Advance index forward so the next iteration doesn't pick it up as well. */
		index += 3;
	}

	return str;
}

/**
 * Creates a new question in the database
 *
 * @param question The question
 * @param correctAns The correct answer
 * @param ans2 Other answer
 * @param ans3 Other answer
 * @param ans4 Other answer
 * @return The id of the question
 */
unsigned int Database::createQuestion(std::string question, std::string correctAns, std::string ans2, std::string ans3, std::string ans4)
{
	unsigned int questionId = 0;

	int res = 0;
	std::string insertQuestionQuery("INSERT INTO questions(question, correct_ans, ans2, ans3, ans4) VALUES(':question', ':correct_ans', ':ans2', ':ans3', ':ans4')");
	std::string getQuestionIdQuery("SELECT question_id FROM questions WHERE question = ':question'");

	// Bind parameters
	insertQuestionQuery = std::regex_replace(insertQuestionQuery, std::regex(":question"), escapeString(question));
	insertQuestionQuery = std::regex_replace(insertQuestionQuery, std::regex(":correct_ans"), escapeString(correctAns));
	insertQuestionQuery = std::regex_replace(insertQuestionQuery, std::regex(":ans2"), escapeString(ans2));
	insertQuestionQuery = std::regex_replace(insertQuestionQuery, std::regex(":ans3"), escapeString(ans3));
	insertQuestionQuery = std::regex_replace(insertQuestionQuery, std::regex(":ans4"), escapeString(ans4));
	getQuestionIdQuery = std::regex_replace(getQuestionIdQuery, std::regex(":question"), escapeString(question));

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

/**
 * Creates a new game in the database
 *
 * @return The id of the game
 */
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

/**
 * Ends game in the database
 *
 * @param gameId The id of the game
 * @return None
 */
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

/**
 * Submits an answer to the database
 *
 * @param gameId The id of the game
 * @param questionId The id of the question
 * @param username The username
 * @param answer The answer id
 * @param correctAns Is the ans was correct
 * @return None
 */
void Database::submitAnswer(unsigned int gameId, unsigned int questionId, std::string username, unsigned int answer, bool correctAns)
{
	int res = 0;
	std::string submitAnswerQuery("INSERT INTO answers(username, game_id, question_id, answer, correct_ans) VALUES(':username', :game_id, :question_id, :answer, :correct_ans)");

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
