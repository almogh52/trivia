#include "request_handler_factory.h"

RequestHandlerFactory::RequestHandlerFactory(std::shared_ptr<IDatabase> database) 
	: m_loginManager(new LoginManager(database)), m_roomManager(new RoomManager()), m_highscoreTable(new HighscoreTable(database)), m_gameManager(new GameManager(database))
{
}

std::shared_ptr<LoginRequestHandler> RequestHandlerFactory::createLoginRequestHandler()
{
    return std::shared_ptr<LoginRequestHandler>(new LoginRequestHandler(shared_from_this(), m_loginManager));
}

std::shared_ptr<MenuRequestHandler> RequestHandlerFactory::createMenuRequestHandler(const LoggedUser & user)
{
    return std::shared_ptr<MenuRequestHandler>(new MenuRequestHandler(user, m_roomManager, m_highscoreTable, shared_from_this()));
}

std::shared_ptr<RoomMemberRequestHandler> RequestHandlerFactory::createRoomMemberRequestHandler(const LoggedUser & user, const int roomId)
{
	return std::shared_ptr<RoomMemberRequestHandler>(new RoomMemberRequestHandler(roomId, user, m_roomManager, shared_from_this()));
}

std::shared_ptr<GameRequestHandler> RequestHandlerFactory::createGameRequestHandler(const LoggedUser & user, const unsigned int gameId)
{
	return std::shared_ptr<GameRequestHandler>(new GameRequestHandler(gameId, user, shared_from_this(), m_gameManager));
}

std::shared_ptr<RoomAdminRequestHandler> RequestHandlerFactory::createRoomAdminRequestHandler(const LoggedUser & user, const int roomId)
{
	return std::shared_ptr<RoomAdminRequestHandler>(new RoomAdminRequestHandler(roomId, user, m_roomManager, shared_from_this()));
}

std::shared_ptr<LoginManager> RequestHandlerFactory::getLoginManager()
{
    return m_loginManager;
}

std::shared_ptr<GameManager> RequestHandlerFactory::getGameManager()
{
	return m_gameManager;
}

std::shared_ptr<RequestHandlerFactory> RequestHandlerFactory::getPtr()
{
    return shared_from_this();
}
