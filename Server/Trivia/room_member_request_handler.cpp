#include "room_member_request_handler.h"

#include "get_room_state_response.h"
#include "request_result.h"
#include "response_status.h"
#include "json_response_packet_serializer.hpp"

RoomMemberRequestHandler::RoomMemberRequestHandler(const Room & room, const LoggedUser & user, std::shared_ptr<RoomManager> roomManager, std::shared_ptr<RequestHandlerFactory> handlerFactory)
	: m_room(room), m_user(user), m_roomManager(roomManager), m_handlerFactory(handlerFactory)
{
}

bool RoomMemberRequestHandler::isRequestRelevant(const Request & req) const
{
	return req.id == GET_ROOM_STATE_REQUEST || req.id == LEAVE_ROOM_REQUEST;
}

RequestResult RoomMemberRequestHandler::getRoomState(const Request & req) const
{
	RequestResult res;

	GetRoomStateResponse getRoomStateResponse;

	// Set the details of the room
	getRoomStateResponse.answerTimeout = m_room.getMetadata().timePerQuestion;
	getRoomStateResponse.questionCount = m_room.getMetadata().questionCount;

	try {
		// Get the players of the room
		getRoomStateResponse.players = m_roomManager->getPlayersInRoom(m_room.getMetadata().id);

		// Check if the game in the room has begun
		getRoomStateResponse.hasGameBegun = m_roomManager->getRoomState(m_room.getMetadata().id);
		getRoomStateResponse.status = SUCCESS;
	}
	catch (...) {
		getRoomStateResponse.status = ERROR;
	}

	// Set the new handler as the menu request handler and serialize the response
	res.newHandler = nullptr;
	res.response = JsonResponsePacketSerializer::SerializePacket(getRoomStateResponse);

	return res;
}