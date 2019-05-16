#include <memory>
#include <iostream>

#include "server.h"
#include "WSAInitializer.h"

int main()
{
    std::unique_ptr<Server> server(new Server());
    std::unique_ptr<WSAInitializer> wsaInitializer(new WSAInitializer());

    try {
	// Try to run the server
	server->run();
    }
    catch (std::exception& ex) {
	std::cout << ex.what() << std::endl;
    }
    catch (...) {
	std::cout << "An an unknown error occurred running the server" << std::endl;
    }

    return 0;
}