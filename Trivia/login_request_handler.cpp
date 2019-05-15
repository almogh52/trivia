#include "login_request_handler.h"

#include <vector>

#include "login_request.h"
#include "login_response.h"
#include "response_status.h"
#include "json_request_packet_deserializer.hpp"
#include "json_response_packet_serializer.hpp"

LoginRequestHandler::LoginRequestHandler(std::shared_ptr<LoginManager> loginManager) : m_loginManager(loginManager)
{
}

RequestResult LoginRequestHandler::login(Request & req)
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
