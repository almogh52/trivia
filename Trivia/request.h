#pragma once
#include <ctime>
#include <vector>

#include "request_id.h"

struct Request {
	RequestId id;
	time_t receivalTime;
	std::vector<char> buffer;
};