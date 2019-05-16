#include <memory>
#include <iostream>

#include "server.h"

int main()
{
    std::unique_ptr<Server> server(new Server());

    try {
	// Try to run the server
	server->run();
    }
    catch (...) {
	std::cout << "An an error occurred running the server" << std::endl;
    }

    return 0;
}