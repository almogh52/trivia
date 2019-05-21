#pragma once
#include <string>

#include "json.hpp"
using nlohmann::json;

struct JoinRoomRequest {
    unsigned int roomId;
};

void to_json(json& j, const JoinRoomRequest& req) {
    j = json{ {"roomId", req.roomId} };
}

void from_json(const json& j, JoinRoomRequest& req) {
    j.at("roomId").get_to(req.roomId);
}