#pragma once

#include "room_manager.h"
#include "login_manager.h"
#include "login_request_handler.h"
#include "database_interface.h"

class RequestHandlerFactory
{
public:
    RequestHandlerFactory(std::shared_ptr<IDatabase> database);

    std::shared_ptr<LoginRequestHandler> createLoginRequestHandler();

private:
    std::shared_ptr<LoginManager> m_loginManager;
    std::shared_ptr<RoomManager> m_roomManager;
};

