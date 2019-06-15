#pragma once
#include <memory>
#include <vector>

#include "request_handler_interface.h"

struct RequestResult {
	std::vector<char> response;
	std::shared_ptr<IRequestHandler> newHandler;
};