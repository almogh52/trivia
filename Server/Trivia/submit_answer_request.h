#pragma once
#include <string>

#include "json.hpp"
using nlohmann::json;

struct SubmitAnswerRequest {
	unsigned int answerId;
};

inline void to_json(json& j, const SubmitAnswerRequest& req) {
	j = json{ {"answerId", req.answerId} };
}

inline void from_json(const json& j, SubmitAnswerRequest& req) {
	j.at("answerId").get_to(req.answerId);
}