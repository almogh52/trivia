#pragma once
#include <string>

#include "json.hpp"
using nlohmann::json;

struct PlayerResults
{
	std::string username;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int averageAnswerTime;
};

inline void to_json(json& j, const PlayerResults& res) {
	j = json{ {"username", res.username}, {"correctAnswerCount", res.correctAnswerCount}, {"wrongAnswerCount", res.wrongAnswerCount}, {"averageAnswerTime", res.averageAnswerTime} };
}

inline void from_json(const json& j, PlayerResults& res) {
	j.at("username").get_to(res.username);
	j.at("correctAnswerCount").get_to(res.correctAnswerCount);
	j.at("wrongAnswerCount").get_to(res.wrongAnswerCount);
	j.at("averageAnswerTime").get_to(res.averageAnswerTime);
}