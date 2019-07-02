#include "server.h"

#include <iostream>

#include "database.h"

Server::Server() :  m_database(new Database()), 
					m_handlerFactory(std::make_shared<RequestHandlerFactory>(m_database)), 
					m_communicator(new Communicator(m_handlerFactory))
{
}

void Server::run()
{
	std::cout << "Trivia Server version 4.0.0" << std::endl;
	std::cout << "Programmers: Almog Hamdani & Sagiv Avizrat" << std::endl;
	std::cout << std::endl;

    // Initialize database
    m_database->initDatabase();

    // Bind and start listening and handle requests
    m_communicator->bindAndListen();
    m_communicator->handleRequests();

	// Close the database
	m_database->closeDatabase();
}
