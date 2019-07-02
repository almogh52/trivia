#pragma once
#include <string>

#include "json.hpp"
using nlohmann::json;

struct LoginRequest {
	std::string username;
	std::string password;
};

inline void to_json(json& j, const LoginRequest& req) {
	j = json{ {"username", req.username}, {"password", req.password} };
}

inline void from_json(const json& j, LoginRequest& req) {
	j.at("username").get_to(req.username);
	j.at("password").get_to(req.password);
}