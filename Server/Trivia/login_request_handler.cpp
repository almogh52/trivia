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

LoginRequestHandler::LoginRequestHandler(std::shared_ptr<RequestHandlerFactory> handlerFactory, std::shared_ptr<LoginManager> loginManager) : m_handlerFactory(handlerFactory), m_loginManager(loginManager)
{
}

bool LoginRequestHandler::isRequestRelevant(const Request & req) const
{
    return req.id == LOGIN_REQUEST || req.id == SIGNUP_REQUEST;
}

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

RequestResult LoginRequestHandler::login(const Request & req) const
{
    LoginRequest loginReq = JsonRequestPacketDeserializer::DeserializePacket<LoginRequest>(req.buffer);
    LoginResponse loginRes;
    RequestResult res;

    // Try to login the user
    if (m_loginManager->login(loginReq.username, loginReq.password))
    {
	loginRes.status = SUCCESS;
    }
    else {
	loginRes.status = ERROR;
    }

    // Serialize the new packet and set the next handler
    res.newHandler = nullptr;
    res.response = JsonResponsePacketSerializer::SerializePacket(loginRes);

    return res;
}

RequestResult LoginRequestHandler::signup(const Request & req) const
{
    SignupRequest signupReq = JsonRequestPacketDeserializer::DeserializePacket<SignupRequest>(req.buffer);
    SignupResponse signupRes;
    RequestResult res;

    // Try to sign up the user
    if (m_loginManager->signup(signupReq.username, signupReq.password, signupReq.email))
    {
	signupRes.status = SUCCESS;
    }
    else {
	signupRes.status = ERROR;
    }

    // Serialize the new packet and set the next handler
    res.newHandler = nullptr;
    res.response = JsonResponsePacketSerializer::SerializePacket(signupRes);

    return res;
}
