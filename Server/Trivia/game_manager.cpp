#include "game_manager.h"

#include <iostream>
#include <sstream>
#include "HTTPRequest.hpp"

#include "exception.h"

GameManager::GameManager(std::shared_ptr<IDatabase> database) : m_database(database)
{
}

unsigned int GameManager::createGame(Room & room)
{
	return 0;
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
