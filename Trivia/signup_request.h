#pragma once
#include <string>

#include "json.hpp"
using nlohmann::json;

struct SignupRequest {
	std::string username;
	std::string password;
	std::string email;
};

void to_json(json& j, const SignupRequest& req) {
	j = json{ {"username", req.username}, {"password", req.password}, {"email", req.email} };
}

void from_json(json& j, SignupRequest& req) {
	j.at("username").get_to(req.username);
	j.at("password").get_to(req.password);
	j.at("email").get_to(req.email);
}