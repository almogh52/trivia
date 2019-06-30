#pragma once
#include <string>

#include "json.hpp"
using nlohmann::json;

struct CreateRoomRequest {
	unsigned int answerId;
};

inline void to_json(json& j, const CreateRoomRequest& req) {
	j = json{ {"answerId", req.answerId} };
}

inline void from_json(const json& j, CreateRoomRequest& req) {
	j.at("answerId").get_to(req.answerId);
}