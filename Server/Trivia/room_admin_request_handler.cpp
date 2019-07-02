#include "room_admin_request_handler.h"

#include "json_response_packet_serializer.hpp"

#include "close_room_response.h"
#include "get_room_state_response.h"
#include "response_status.h"
#include "start_game_response.h"

#include "request_handler_factory.h"

#include <iostream>

RoomAdminRequestHandler::RoomAdminRequestHandler(const int & roomId, const LoggedUser & user, std::shared_ptr<RoomManager> roomManager, std::shared_ptr<RequestHandlerFactory> handlerFactory)
	: m_roomId(roomId), m_user(user), m_roomManager(roomManager), m_handlerFactory(handlerFactory)
{
}

bool RoomAdminRequestHandler::isRequestRelevant(const Request & req) const
{
	return req.id == CLOSE_ROOM_REQUEST || req.id == START_GAME_REQUEST || req.id == GET_ROOM_STATE_REQUEST;
}

RequestResult RoomAdminRequestHandler::handleRequest(const Request & req) const
{
	RequestResult res;

	switch (req.id)
	{
	case CLOSE_ROOM_REQUEST:
		res = closeRoom(req);
		break;

	case GET_ROOM_STATE_REQUEST:
		res = getRoomState(req);
		break;

	case START_GAME_REQUEST:
		res = startGame(req);
		break;
	}

	return res;
}

void RoomAdminRequestHandler::disconnect() const
{
	try {
		// Try to delete the room
		m_roomManager->deleteRoom(m_roomId);
	}
	catch (...) {}
}

RequestResult RoomAdminRequestHandler::closeRoom(const Request & req) const
{
	RequestResult res;

	CloseRoomResponse closeRoomResponse;

	try {
		// Try to delete the room
		m_roomManager->deleteRoom(m_roomId);
		closeRoomResponse.status = SUCCESS;
	}
	catch (...) {
		closeRoomResponse.status = ERROR;
	}

	// Set the new handler as the menu request handler and serialize the response
	res.newHandler = m_handlerFactory->createMenuRequestHandler(m_user);
	res.response = JsonResponsePacketSerializer::SerializePacket(closeRoomResponse);

	return res;
}

RequestResult RoomAdminRequestHandler::getRoomState(const Request & req) const
{
	RequestResult res;

	RoomData roomData;
	GetRoomStateResponse getRoomStateResponse;

	try {
		// Get the details of the room
		roomData = m_roomManager->getRoomData(m_roomId);

		// Set the details of the room
		getRoomStateResponse.answerTimeout = roomData.timePerQuestion;
		getRoomStateResponse.questionCount = roomData.questionCount;

		// Get the players of the room
		getRoomStateResponse.players = m_roomManager->getPlayersInRoom(m_roomId);

		// Check if the game in the room has begun
		getRoomStateResponse.hasGameBegun = !m_roomManager->getRoomState(m_roomId);
		getRoomStateResponse.status = SUCCESS;
	}
	catch (...) {
		getRoomStateResponse.status = ERROR;
	}

	// Serialize the response
	res.newHandler = nullptr;
	res.response = JsonResponsePacketSerializer::SerializePacket(getRoomStateResponse);

	return res;
}

RequestResult RoomAdminRequestHandler::startGame(const Request & req) const
{
	RequestResult res;

	unsigned int gameId = -1;
	RoomData roomData;
	std::vector<LoggedUser> players;

	StartGameResponse startGameResponse;

	try {
		// Get the details of the room
		roomData = m_roomManager->getRoomData(m_roomId);

		// Get the logged players in the room
		players = m_roomManager->getLoggedPlayersInRoom(m_roomId);

		// Create the game
		gameId = m_handlerFactory->getGameManager()->createGame(roomData, players);

		// Start the game
		m_roomManager->startGame(m_roomId, gameId);

		startGameResponse.status = SUCCESS;
	}
	catch (...) {
		startGameResponse.status = ERROR;
	}

	// Set the new handler as the game request handler and serialize the response
	res.newHandler = startGameResponse.status ? nullptr : m_handlerFactory->createGameRequestHandler(m_user, gameId);
	res.response = JsonResponsePacketSerializer::SerializePacket(startGameResponse);

	return res;
}
