#pragma once
#include "request_handler_interface.h"

#include "logged_user.h"
#include "room_manager.h"
#include "login_manager.h"

class RequestHandlerFactory;

class MenuRequestHandler :
    public IRequestHandler
{
public:
    MenuRequestHandler(std::shared_ptr<LoggedUser> user, std::shared_ptr<RoomManager> roomManager, std::shared_ptr<LoginManager> loginManager, std::shared_ptr<RequestHandlerFactory> handlerFactory);
    
    virtual bool isRequestRelevant(const Request& req) const;
    virtual RequestResult handleRequest(const Request& req) const;

private:
    RequestResult logout(const Request& req) const;

    std::shared_ptr<LoggedUser> m_user;
    std::shared_ptr<RoomManager> m_roomManager;
    std::shared_ptr<LoginManager> m_loginManager;
    std::shared_ptr<RequestHandlerFactory> m_handlerFactory;
};

