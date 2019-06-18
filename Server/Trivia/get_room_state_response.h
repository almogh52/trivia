#pragma once
#include <vector>

#include "json.hpp"
using nlohmann::json;

struct GetRoomStateResponse {
	unsigned int status;
	bool hasGameBegun;
	std::vector<std::string> players;
	unsigned int questionCount;
	unsigned int answerTimeout;
};

inline void to_json(json& j, const GetRoomStateResponse& res) {
	j = json{ {"status", res.status}, {"hasGameBegun", res.hasGameBegun}, {"players", res.players}, {"questionCount", res.questionCount}, {"answerTimeout", res.answerTimeout} };
}

inline void from_json(const json& j, GetRoomStateResponse& res) {
	j.at("status").get_to(res.status);
	j.at("hasGameBegun").get_to(res.hasGameBegun);
	j.at("players").get_to(res.players);
	j.at("questionCount").get_to(res.questionCount);
	j.at("answerTimeout").get_to(res.answerTimeout);
}