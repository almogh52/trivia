#pragma once

#include "json.hpp"
using nlohmann::json;

struct StatusResponse {
    unsigned int status;
};

inline void to_json(json& j, const StatusResponse& res) {
    j = json{ {"status", res.status} };
}

inline void from_json(const json& j, StatusResponse& res) {
    j.at("status").get_to(res.status);
}