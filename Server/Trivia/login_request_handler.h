#pragma once
#include "request_handler_interface.h"

#include <memory>

#include "login_manager.h"
#include "request_result.h"

class RequestHandlerFactory;

class LoginRequestHandler :
    public IRequestHandler
{
public:
    LoginRequestHandler(std::shared_ptr<RequestHandlerFactory> handlerFactory, std::shared_ptr<LoginManager> loginManager);

    virtual bool isRequestRelevant(const Request& req) const;
    virtual RequestResult handleRequest(const Request& req) const;

	virtual void disconnect() const;

private:
    std::shared_ptr<LoginManager> m_loginManager;
    std::shared_ptr<RequestHandlerFactory> m_handlerFactory;

    RequestResult login(const Request& req) const;
    RequestResult signup(const Request& req) const;
};

