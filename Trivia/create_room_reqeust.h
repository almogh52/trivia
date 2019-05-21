#pragma once
#include <string>

#include "json.hpp"
using nlohmann::json;

struct CreateRoomRequest {
    std::string roomName;
    unsigned int maxUsers;
    unsigned int questionCount;
    unsigned int answerTimeout;
};

void to_json(json& j, const CreateRoomRequest& req) {
    j = json{ {"roomId", req.roomName}, {"maxUsers", req.maxUsers}, {"questionCount", req.questionCount}, {"answerTimeout", req.answerTimeout} };
}

void from_json(const json& j, CreateRoomRequest& req) {
    j.at("roomName").get_to(req.roomName);
    j.at("maxUsers").get_to(req.maxUsers);
    j.at("questionCount").get_to(req.questionCount);
    j.at("answerTimeout").get_to(req.answerTimeout);
}