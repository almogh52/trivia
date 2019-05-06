#pragma once
#include "request.h"
#include "request_result.h"

class IRequestHandler {
public:
	virtual ~IRequestHandler() {}

	virtual bool isRequestRelevant(const Request& req) const = 0;
	virtual RequestResult handleRequest(const Request& req) const = 0;
};