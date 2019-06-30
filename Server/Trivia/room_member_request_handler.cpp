#include "room_member_request_handler.h"

#include "get_room_state_response.h"
#include "leave_room_response.h"
#include "request_result.h"
#include "response_status.h"

#include "request_handler_factory.h"
#include "json_response_packet_serializer.hpp"

RoomMemberRequestHandler::RoomMemberRequestHandler(const int & roomId, const LoggedUser & user, std::shared_ptr<RoomManager> roomManager, std::shared_ptr<RequestHandlerFactory> handlerFactory)
	: m_roomId(roomId), m_user(user), m_roomManager(roomManager), m_handlerFactory(handlerFactory)
{
}

bool RoomMemberRequestHandler::isRequestRelevant(const Request & req) const
{
	return req.id == GET_ROOM_STATE_REQUEST || req.id == LEAVE_ROOM_REQUEST;
}

RequestResult RoomMemberRequestHandler::handleRequest(const Request & req) const
{
	RequestResult res;

	switch (req.id)
	{
	case GET_ROOM_STATE_REQUEST:
		res = getRoomState(req);
		break;

	case LEAVE_ROOM_REQUEST:
		res = leaveRoom(req);
		break;
	}

	return res;
}

void RoomMemberRequestHandler::disconnect() const
{
	// Leave the room
	m_roomManager->leaveRoom(m_user, m_roomId);
}

RequestResult RoomMemberRequestHandler::leaveRoom(const Request & req) const
{
	RequestResult res;

	LeaveRoomResponse leaveRoomResponse;

	// Try to leave the room
	if (m_roomManager->leaveRoom(m_user, m_roomId))
	{
		leaveRoomResponse.status = SUCCESS;
	}
	else {
		leaveRoomResponse.status = ERROR;
	}

	// Set the new handler as the menu request handler and serialize the response
	res.newHandler = m_handlerFactory->createMenuRequestHandler(m_user);
	res.response = JsonResponsePacketSerializer::SerializePacket(leaveRoomResponse);

	return res;
}

RequestResult RoomMemberRequestHandler::getRoomState(const Request & req) const
{
	RequestResult res;

	RoomData roomData;
	GetRoomStateResponse getRoomStateResponse = { 0 };
	
	try {
		// Get the room data of the room
		roomData = m_roomManager->getRoomData(m_roomId);

		// Set the details of the room
		getRoomStateResponse.answerTimeout = roomData.timePerQuestion;
		getRoomStateResponse.questionCount = roomData.questionCount;

		// Get the players of the room
		getRoomStateResponse.players = m_roomManager->getPlayersInRoom(m_roomId);

		// Check if the game in the room has begun
		getRoomStateResponse.hasGameBegun = m_roomManager->getRoomState(m_roomId);
		getRoomStateResponse.status = SUCCESS;
	}
	catch (...) {
		getRoomStateResponse.status = ERROR;
	}

	// Set the new handler as the menu request handler and serialize the response
	res.newHandler = getRoomStateResponse.status ? m_handlerFactory->createMenuRequestHandler(m_user) : nullptr;
	res.response = JsonResponsePacketSerializer::SerializePacket(getRoomStateResponse);

	return res;
}