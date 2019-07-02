#pragma once
#include <vector>

#include "json.hpp"
using nlohmann::json;

#include "player_results.h"

struct GetGameResultsResponse {
	unsigned int status;
	std::vector<PlayerResults> results;
};

inline void to_json(json& j, const GetGameResultsResponse& res) {
	j = json{ {"status", res.status}, {"results", res.results} };
}

inline void from_json(const json& j, GetGameResultsResponse& res) {
	j.at("status").get_to(res.status);
	j.at("results").get_to(res.results);
}