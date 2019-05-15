#pragma once
#include "request_handler_interface.h"

#include <memory>

#include "login_manager.h"
#include "request_result.h"

class LoginRequestHandler :
    public IRequestHandler
{
public:
    LoginRequestHandler(std::shared_ptr<LoginManager> loginManager);

private:
    std::shared_ptr<LoginManager> m_loginManager;

    RequestResult login(Request& req);
};

