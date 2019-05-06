#pragma once
#include <string>

#include "json.hpp"
using nlohmann::json;

struct LoginRequest {
	std::string username;
	std::string password;
};

void to_json(json& j, const LoginRequest& req) {
	j = json{ {"username", req.username}, {"password", req.password} };
}

void from_json(json& j, LoginRequest& req) {
	j.at("username").get_to(req.username);
	j.at("password").get_to(req.password);
}