#include "request_handler_factory.h"

RequestHandlerFactory::RequestHandlerFactory(std::shared_ptr<IDatabase> database) : m_loginManager(new LoginManager(database)), m_roomManager(new RoomManager())
{
}

std::shared_ptr<LoginRequestHandler> RequestHandlerFactory::createLoginRequestHandler()
{
    return std::shared_ptr<LoginRequestHandler>(new LoginRequestHandler((std::shared_ptr<RequestHandlerFactory>)this, m_loginManager));
}

std::shared_ptr<MenuRequestHandler> RequestHandlerFactory::createMenuRequestHandler(LoggedUser & user)
{
    return std::shared_ptr<MenuRequestHandler>(new MenuRequestHandler((std::shared_ptr<LoggedUser>)&user, m_roomManager, (std::shared_ptr<RequestHandlerFactory>)this));
}

std::shared_ptr<LoginManager> RequestHandlerFactory::getLoginManager()
{
    return m_loginManager;
}
