#include "request_handler_factory.h"

RequestHandlerFactory::RequestHandlerFactory(std::shared_ptr<IDatabase> database) : m_loginManager(new LoginManager(database)), m_roomManager(new RoomManager())
{
}

std::shared_ptr<LoginRequestHandler> RequestHandlerFactory::createLoginRequestHandler()
{
    return std::shared_ptr<LoginRequestHandler>(new LoginRequestHandler((std::shared_ptr<RequestHandlerFactory>)this, m_loginManager));
}
