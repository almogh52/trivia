#pragma once
#include <string>

#include "json.hpp"
using nlohmann::json;

struct RoomData {
    unsigned int id;
    std::string name;
    unsigned int maxPlayers;
    unsigned int timePerQuestion;
	unsigned int questionCount;
    bool isActive;
};

inline void to_json(json& j, const RoomData& roomData) {
    j = json{ {"id", roomData.id}, {"name", roomData.name}, {"maxPlayers", roomData.maxPlayers}, {"timePerQuestion", roomData.timePerQuestion}, {"questionCount", roomData.questionCount}, {"isActive", roomData.isActive} };
}

inline void from_json(const json& j, RoomData& roomData) {
    j.at("id").get_to(roomData.id);
    j.at("name").get_to(roomData.name);
    j.at("maxPlayers").get_to(roomData.maxPlayers);
    j.at("timePerQuestion").get_to(roomData.timePerQuestion);
	j.at("questionCount").get_to(roomData.questionCount);
    j.at("isActive").get_to(roomData.isActive);
}

