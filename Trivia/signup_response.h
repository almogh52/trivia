#pragma once

#include "json.hpp"
using nlohmann::json;

struct SignupResponse {
	unsigned int status;
};

void to_json(json& j, const SignupResponse& res) {
	j = json{ {"status", res.status} };
}

void from_json(const json& j, SignupResponse& res) {
	j.at("status").get_to(res.status);
}