#pragma once
#include <string>

#include "json.hpp"
using nlohmann::json;

struct CreateRoomRequest {
    std::string roomName;
    unsigned int maxPlayers;
    unsigned int questionCount;
    unsigned int answerTimeout;
};

inline void to_json(json& j, const CreateRoomRequest& req) {
    j = json{ {"roomId", req.roomName}, {"maxPlayers", req.maxPlayers}, {"questionCount", req.questionCount}, {"answerTimeout", req.answerTimeout} };
}

inline void from_json(const json& j, CreateRoomRequest& req) {
    j.at("roomName").get_to(req.roomName);
    j.at("maxPlayers").get_to(req.maxPlayers);
    j.at("questionCount").get_to(req.questionCount);
    j.at("answerTimeout").get_to(req.answerTimeout);
}