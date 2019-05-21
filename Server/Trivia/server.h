#pragma once

#include <memory>

#include "database_interface.h"
#include "request_handler_factory.h"
#include "communicator.h"

class Server
{
public:
    Server();

    void run();

private:
    std::shared_ptr<IDatabase> m_database;
    std::shared_ptr<RequestHandlerFactory> m_handlerFactory;
    std::shared_ptr<Communicator> m_communicator;
};

