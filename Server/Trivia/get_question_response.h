#pragma once
#include <map>
#include <string>

#include "json.hpp"
using nlohmann::json;

struct GetQuestionResponse {
	unsigned int status;
	std::string question;
	std::map<unsigned int, std::string> answers;
};

inline void to_json(json& j, const GetQuestionResponse& res) {
	j = json{ {"status", res.status}, {"question", res.question}, {"answers", res.answers} };
}

inline void from_json(const json& j, GetQuestionResponse& res) {
	j.at("status").get_to(res.status);
	j.at("question").get_to(res.question);
	j.at("answers").get_to(res.answers);
}