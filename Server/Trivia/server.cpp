#include "server.h"

#include "database.h"

Server::Server() :  m_database(new Database()), 
		    m_handlerFactory(new RequestHandlerFactory(m_database)), 
		    m_communicator(new Communicator(m_handlerFactory))
{
}

void Server::run()
{
    // Initialize database
    m_database->initDatabase();

    // Bind and start listening and handle requests
    m_communicator->bindAndListen();
    m_communicator->handleRequests();
}
