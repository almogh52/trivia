#include "game_request_handler.h"

#include "json_response_packet_serializer.hpp"
#include "json_request_packet_deserializer.hpp"
#include "request_handler_factory.h"

#include "get_question_response.h"
#include "submit_answer_request.h"
#include "submit_answer_response.h"
#include "leave_room_response.h"
#include "get_game_results_response.h"
#include "response_status.h"

std::time_t lastQuestionSendTime;

GameRequestHandler::GameRequestHandler(unsigned int gameId, const LoggedUser& user, std::shared_ptr<RequestHandlerFactory> handlerFactory, std::shared_ptr<GameManager> gameManager)
	: m_game(gameId), m_user(user), m_handlerFactory(handlerFactory), m_gameManager(gameManager)
{
}

/**
 * Checks if the request is relevant
 *
 * @param req The request
 * @return Is the request relevant
 */
bool GameRequestHandler::isRequestRelevant(const Request & req) const
{
	return req.id == GET_QUESTION_REQUEST || req.id == SUBMIT_ANSWER_REQUEST || req.id == GET_GAME_RESULTS_REQUEST || req.id == LEAVE_GAME_REQUEST;
}

/**
 * Handles a request
 *
 * @param req The request
 * @return The request result
 */
RequestResult GameRequestHandler::handleRequest(const Request & req) const
{
	RequestResult res;

	switch (req.id)
	{
	case GET_QUESTION_REQUEST:
		res = getQuestion(req);
		break;

	case SUBMIT_ANSWER_REQUEST:
		res = submitAnswer(req);
		break;

	case LEAVE_GAME_REQUEST:
		res = leaveGame(req);
		break;

	case GET_GAME_RESULTS_REQUEST:
		res = getGameResults(req);
		break;
	}

	return res;
}

/**
 * Disconnect handler
 *
 * @return None
 */
void GameRequestHandler::disconnect() const
{
	try {
		// Remove the player from the game
		m_gameManager->removePlayer(m_game, m_user);

		// Logout the user
		m_handlerFactory->getLoginManager()->logout(m_user);

		// Try to delete the game if no one is playing it
		if (m_gameManager->deleteGame(m_game))
		{
			// Try to delete the room of the game
			m_handlerFactory->getRoomManager()->deleteRoomWithGameId(m_game);
		}
	}
	catch (...) {}
}

/**
 * Get a question handler
 *
 * @param req The request
 * @return The request result
 */
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
			getQuestionResponse.answers[(unsigned int)(iterator - answers.begin())] = *iterator;
		}

		getQuestionResponse.status = SUCCESS;
	}
	catch (...) {
		getQuestionResponse.status = ERROR;
	}

	// Serialize the response
	res.newHandler = nullptr;
	res.response = JsonResponsePacketSerializer::SerializePacket(getQuestionResponse);

	// Set the time the last question was sent
	lastQuestionSendTime = req.receivalTime;

	return res;
}

/**
 * Submit an answer handler
 *
 * @param req The request
 * @return The request result
 */
RequestResult GameRequestHandler::submitAnswer(const Request & req) const
{
	RequestResult res;

	SubmitAnswerRequest submitAnswerRequest = JsonRequestPacketDeserializer::DeserializePacket<SubmitAnswerRequest>(req.buffer);
	SubmitAnswerResponse submitAnswerResponse;

	try {
		// Try to get the answer id for the current question of the user
		submitAnswerResponse.correctAnswerId = m_gameManager->getQuestionForUser(m_game, m_user).getCorrectAnswer();

		// Submit the answer to the game manager
		m_gameManager->submitAnswer(m_game, m_user, submitAnswerRequest.answerId, (unsigned int)(req.receivalTime - lastQuestionSendTime));

		submitAnswerResponse.status = SUCCESS;
	}
	catch (...) {
		submitAnswerResponse.status = ERROR;
	}

	// Serialize the response
	res.newHandler = nullptr;
	res.response = JsonResponsePacketSerializer::SerializePacket(submitAnswerResponse);

	return res;
}

/**
 * Leave game handler
 *
 * @param req The request
 * @return The request result
 */
RequestResult GameRequestHandler::leaveGame(const Request & req) const
{
	RequestResult res;

	LeaveRoomResponse leaveRoomResponse;

	try {
		// Try to remove the player
		m_gameManager->removePlayer(m_game, m_user);

		leaveRoomResponse.status = SUCCESS;
	}
	catch (...) {
		leaveRoomResponse.status = ERROR;
	}

	// Try to delete the game if no one is playing it
	try {
		// Try to delete the game if no one is playing it
		if (m_gameManager->deleteGame(m_game))
		{
			// Try to delete the room of the game
			m_handlerFactory->getRoomManager()->deleteRoomWithGameId(m_game);
		}
	} catch (...) {}
	

	// Serialize the response and set the next handler as the menu request handler
	res.newHandler = m_handlerFactory->createMenuRequestHandler(m_user);
	res.response = JsonResponsePacketSerializer::SerializePacket(leaveRoomResponse);

	return res;
}

/**
 * Get game results handler
 *
 * @param req The request
 * @return The request result
 */
RequestResult GameRequestHandler::getGameResults(const Request & req) const
{
	RequestResult res;

	GetGameResultsResponse getGameResultsResponse;

	try {
		// Try to get the results of the game
		getGameResultsResponse.results = m_gameManager->getPlayersResults(m_game);

		// Remove the player from the game
		m_gameManager->removePlayer(m_game, m_user);

		getGameResultsResponse.status = SUCCESS;
	}
	catch (...) {
		getGameResultsResponse.status = ERROR;
	}

	try {
		// Try to delete the game if no one is playing it
		if (m_gameManager->deleteGame(m_game))
		{
			// Try to delete the room of the game
			m_handlerFactory->getRoomManager()->deleteRoomWithGameId(m_game);
		}
	} catch (...) {}

	// Serialize the response and set the next handler as the menu request handler
	res.newHandler = getGameResultsResponse.status ? nullptr : m_handlerFactory->createMenuRequestHandler(m_user);
	res.response = JsonResponsePacketSerializer::SerializePacket(getGameResultsResponse);

	return res;
}
