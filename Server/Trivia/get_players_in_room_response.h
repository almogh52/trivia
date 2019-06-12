#pragma once
#include <vector>

#include "json.hpp"
using nlohmann::json;

#include "room_data.h"

struct GetPlayersInRoomResponse {
    std::vector<std::string> rooms;
};

inline void to_json(json& j, const GetPlayersInRoomResponse& res) {
    j = json{ {"rooms", res.rooms} };
}

inline void from_json(const json& j, GetPlayersInRoomResponse& res) {
    j.at("rooms").get_to(res.rooms);
}