#pragma once
#include "request_handler_interface.h"

#include "room.h"
#include "logged_user.h"
#include "room_manager.h"

class RequestHandlerFactory;

class RoomAdminRequestHandler :
	public IRequestHandler
{
public:
	RoomAdminRequestHandler(const int& roomId, const LoggedUser& user, std::shared_ptr<RoomManager> roomManager, std::shared_ptr<RequestHandlerFactory> handlerFactory);

	virtual bool isRequestRelevant(const Request& req) const;
	virtual RequestResult handleRequest(const Request& req) const;

	virtual void disconnect() const;

private:
	RequestResult closeRoom(const Request& req) const;
	RequestResult getRoomState(const Request& req) const;

	int m_roomId;
	LoggedUser m_user;
	std::shared_ptr<RoomManager> m_roomManager;
	std::shared_ptr<RequestHandlerFactory> m_handlerFactory;
};

