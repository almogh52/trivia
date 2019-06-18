#pragma once
#include <vector>

#include "json.hpp"
using nlohmann::json;

struct CreateRoomResponse {
	int status;
	int roomId;
};

inline void to_json(json& j, const CreateRoomResponse& res) {
	j = json{ {"status", res.status}, {"roomId", res.roomId} };
}

inline void from_json(const json& j, CreateRoomResponse& res) {
	j.at("status").get_to(res.status);
	j.at("roomId").get_to(res.roomId);
}