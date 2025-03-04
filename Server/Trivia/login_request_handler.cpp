#include "login_request_handler.h"

#include <iostream>
#include <vector>

#include "login_request.h"
#include "login_response.h"
#include "signup_request.h"
#include "signup_response.h"
#include "response_status.h"
#include "json_request_packet_deserializer.hpp"
#include "json_response_packet_serializer.hpp"
#include "request_handler_factory.h"

LoginRequestHandler::LoginRequestHandler(std::shared_ptr<RequestHandlerFactory> handlerFactory, std::shared_ptr<LoginManager> loginManager) : m_handlerFactory(handlerFactory), m_loginManager(loginManager)
{
}

/**
 * Checks if the request is relevant
 *
 * @param req The request
 * @return Is the request relevant
 */
bool LoginRequestHandler::isRequestRelevant(const Request & req) const
{
    return req.id == LOGIN_REQUEST || req.id == SIGNUP_REQUEST;
}

/**
 * Handles a request
 *
 * @param req The request
 * @return The request result
 */
RequestResult LoginRequestHandler::handleRequest(const Request & req) const
{
    RequestResult res;

    // Handle the request by it's id
    if (req.id == LOGIN_REQUEST)
    {
		res = login(req);
    }
    else if (req.id == SIGNUP_REQUEST) {
		res = signup(req);
    }

    return res;
}

/**
 * Disconnect handler
 *
 * @return None
 */
void LoginRequestHandler::disconnect() const
{
}

/**
 * Login handler
 *
 * @param req The request
 * @return The request result
 */
RequestResult LoginRequestHandler::login(const Request & req) const
{
    std::shared_ptr<LoggedUser> user;
    LoginRequest loginReq = JsonRequestPacketDeserializer::DeserializePacket<LoginRequest>(req.buffer);
    LoginResponse loginRes;
    RequestResult res;

    // Try to login the user
    if ((user = m_loginManager->login(loginReq.username, loginReq.password)) != nullptr)
    {
		loginRes.status = SUCCESS;
    }
    else {
		loginRes.status = ERROR;
    }

    // Serialize the new packet and set the next handler
    res.newHandler = loginRes.status == ERROR ? nullptr : m_handlerFactory->createMenuRequestHandler(*user);
    res.response = JsonResponsePacketSerializer::SerializePacket(loginRes);

    return res;
}

/**
 * Signup handler
 *
 * @param req The request
 * @return The request result
 */
RequestResult LoginRequestHandler::signup(const Request & req) const
{
    std::shared_ptr<LoggedUser> user;
    SignupRequest signupReq = JsonRequestPacketDeserializer::DeserializePacket<SignupRequest>(req.buffer);
    SignupResponse signupRes;
    RequestResult res;

    // Try to sign up the user
    if ((user = m_loginManager->signup(signupReq.username, signupReq.password, signupReq.email)) != nullptr)
    {
		signupRes.status = SUCCESS;
    }
    else {
		signupRes.status = ERROR;
    }

    // Serialize the new packet and set the next handler
    res.newHandler = signupRes.status == ERROR ? nullptr : m_handlerFactory->createMenuRequestHandler(*user);
    res.response = JsonResponsePacketSerializer::SerializePacket(signupRes);

    return res;
}
