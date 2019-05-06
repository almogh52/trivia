#pragma once

#include "json.hpp"
using nlohmann::json;

struct LoginResponse {
	unsigned int status;
};

void to_json(json& j, const LoginResponse& res) {
	j = json{ {"status", res.status} };
}

void from_json(const json& j, LoginResponse& res) {
	j.at("status").get_to(res.status);
}