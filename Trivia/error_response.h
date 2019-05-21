#pragma once
#include <string>

#include "json.hpp"
using nlohmann::json;

struct ErrorResponse {
    std::string message;
};

void to_json(json& j, const ErrorResponse& req) {
    j = json{ {"message", req.message} };
}

void from_json(const json& j, ErrorResponse& req) {
    j.at("message").get_to(req.message);
}