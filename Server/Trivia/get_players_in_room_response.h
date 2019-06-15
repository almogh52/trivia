#pragma once
#include <vector>

#include "json.hpp"
using nlohmann::json;

#include "room_data.h"

struct GetPlayersInRoomResponse {
	int status;
    std::vector<std::string> players;
};

inline void to_json(json& j, const GetPlayersInRoomResponse& res) {
    j = json{ {"status", res.status}, {"players", res.players} };
}

inline void from_json(const json& j, GetPlayersInRoomResponse& res) {
	j.at("status").get_to(res.status);
    j.at("players").get_to(res.players);
}