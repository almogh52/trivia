#include "request_handler_factory.h"

RequestHandlerFactory::RequestHandlerFactory(std::shared_ptr<LoginManager> loginManager) : m_loginManager(loginManager)
{
}

std::shared_ptr<LoginRequestHandler> RequestHandlerFactory::createLoginRequestHandler()
{
    return std::shared_ptr<LoginRequestHandler>(new LoginRequestHandler(m_loginManager));
}
