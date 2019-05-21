#pragma once

#include "json.hpp"
using nlohmann::json;

struct LogoutResponse {
    unsigned int status;
};

void to_json(json& j, const LogoutResponse& res) {
    j = json{ {"status", res.status} };
}

void from_json(const json& j, LogoutResponse& res) {
    j.at("status").get_to(res.status);
}