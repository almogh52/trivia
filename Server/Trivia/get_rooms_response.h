#pragma once
#include <vector>

#include "json.hpp"
using nlohmann::json;

#include "room_data.h"

struct GetRoomsResponse {
    unsigned int status;
    std::vector<RoomData> rooms;
};

inline void to_json(json& j, const GetRoomsResponse& res) {
    j = json{ {"status", res.status}, {"rooms", res.rooms} };
}

inline void from_json(const json& j, GetRoomsResponse& res) {
    j.at("status").get_to(res.status);
    j.at("rooms").get_to(res.rooms);
}