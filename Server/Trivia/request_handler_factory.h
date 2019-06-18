#pragma once

#include "login_request_handler.h"
#include "menu_request_handler.h"
#include "room_admin_request_handler.h"

#include "database_interface.h"

#include "highscore_table.h"
#include "room_manager.h"
#include "login_manager.h"

#include <memory>

class RequestHandlerFactory : public std::enable_shared_from_this<RequestHandlerFactory>
{
public:
    RequestHandlerFactory(std::shared_ptr<IDatabase> database);

    std::shared_ptr<LoginRequestHandler> createLoginRequestHandler();
    std::shared_ptr<MenuRequestHandler> createMenuRequestHandler(const LoggedUser& user);
	std::shared_ptr<RoomAdminRequestHandler> createRoomAdminRequestHandler(const LoggedUser& user, const Room& room);
    
    std::shared_ptr<LoginManager> getLoginManager();

    std::shared_ptr<RequestHandlerFactory> getPtr();
private:
	std::shared_ptr<HighscoreTable> m_highscoreTable;
    std::shared_ptr<LoginManager> m_loginManager;
    std::shared_ptr<RoomManager> m_roomManager;
};

