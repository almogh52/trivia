#pragma once
#include <string>

#include "json.hpp"
using nlohmann::json;

struct GetPlayersInRoomRequest {
    unsigned int roomId;
};

void to_json(json& j, const GetPlayersInRoomRequest& req) {
    j = json{ {"roomId", req.roomId} };
}

void from_json(const json& j, GetPlayersInRoomRequest& req) {
    j.at("roomId").get_to(req.roomId);
}