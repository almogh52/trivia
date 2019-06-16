#pragma once
#include "request.h"

struct RequestResult;
class IRequestHandler {
public:
	virtual ~IRequestHandler() {}

	virtual bool isRequestRelevant(const Request& req) const = 0;
	virtual RequestResult handleRequest(const Request& req) const = 0;

	virtual void disconnect() const = 0;
};