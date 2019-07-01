#include "game_request_handler.h"

#include "json_response_packet_serializer.hpp"
#include "json_request_packet_deserializer.hpp"
#include "request_handler_factory.h"

#include "get_question_response.h"
#include "response_status.h"

GameRequestHandler::GameRequestHandler(unsigned int gameId, const LoggedUser& user, std::shared_ptr<RequestHandlerFactory> handlerFactory, std::shared_ptr<GameManager> gameManager)
	: m_game(gameId), m_user(user), m_handlerFactory(handlerFactory), m_gameManager(gameManager)
{
}

bool GameRequestHandler::isRequestRelevant(const Request & req) const
{
	return req.id == GET_QUESTION_REQUEST || req.id == SUBMIT_ANSWER_REQUEST || req.id == GET_GAME_RESULTS_REQUEST || req.id == LEAVE_GAME_REQUEST;
}

RequestResult GameRequestHandler::handleRequest(const Request & req) const
{
	RequestResult res;

	switch (req.id)
	{
	case GET_QUESTION_REQUEST:
		res = getQuestion(req);
	}

	return res;
}

RequestResult GameRequestHandler::getQuestion(const Request & req) const
{
	RequestResult res;

	GetQuestionResponse getQuestionResponse;

	Question question;
	std::vector<std::string> answers;

	try {
		// Try to get the question for the user
		question = m_gameManager->getQuestionForUser(m_game, m_user);

		// Get answers
		answers = question.getPossibleAnswers();

		// Set the question
		getQuestionResponse.question = question.getQuestion();
		
		// For each answer insert it to the map using the index of the answer
		for (auto iterator = answers.begin(); iterator != answers.end(); iterator++)
		{
			getQuestionResponse.answers[iterator - answers.begin()] = *iterator;
		}

		getQuestionResponse.status = SUCCESS;
	}
	catch (...) {
		getQuestionResponse.status = ERROR;
	}

	// Serialize the response
	res.newHandler = nullptr;
	res.response = JsonResponsePacketSerializer::SerializePacket(getQuestionResponse);

	return res;
}
