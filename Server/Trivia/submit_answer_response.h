#pragma once

#include "json.hpp"
using nlohmann::json;

struct SubmitAnswerResponse {
	unsigned int status;
	unsigned int correctAnswerId;
};

inline void to_json(json& j, const SubmitAnswerResponse& res) {
	j = json{ {"status", res.status}, {"correctAnswerId", res.correctAnswerId} };
}

inline void from_json(const json& j, SubmitAnswerResponse& res) {
	j.at("status").get_to(res.status);
	j.at("correctAnswerId").get_to(res.correctAnswerId);
}