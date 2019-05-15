#pragma once

#include "login_manager.h"
#include "login_request_handler.h"

class RequestHandlerFactory
{
public:
    RequestHandlerFactory(std::shared_ptr<LoginManager> loginManager);

    std::shared_ptr<LoginRequestHandler> createLoginRequestHandler();

private:
    std::shared_ptr<LoginManager> m_loginManager;
};

