#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <map>

#include "request_handler_factory.h"
#include "request_handler_interface.h"

#define LISTEN_PORT 61110

class Communicator
{
public:
    Communicator(std::shared_ptr<RequestHandlerFactory> handlerFactory);
    ~Communicator();

    void bindAndListen();
    void handleRequests();

private:
    std::shared_ptr<RequestHandlerFactory> m_handleFactory;
    std::map <SOCKET, std::shared_ptr<IRequestHandler>> m_clients;

    SOCKET _socket;

    void startThreadForNewClient(SOCKET clientSocket);
    void clientHandler(SOCKET clientSocket);
};

