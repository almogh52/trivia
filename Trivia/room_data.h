#pragma once
#include <string>

#include "json.hpp"
using nlohmann::json;

struct RoomData {
    unsigned int id;
    std::string name;
    unsigned int maxPlayers;
    unsigned int timePerQuestion;
    bool isActive;
};

void to_json(json& j, const RoomData& roomData) {
    j = json{ {"id", roomData.id}, {"name", roomData.name}, {"maxPlayers", roomData.maxPlayers}, {"timePerQuestion", roomData.timePerQuestion}, {"isActive", roomData.isActive} };
}

void from_json(const json& j, RoomData& roomData) {
    j.at("id").get_to(roomData.id);
    j.at("name").get_to(roomData.name);
    j.at("maxPlayers").get_to(roomData.maxPlayers);
    j.at("timePerQuestion").get_to(roomData.timePerQuestion);
    j.at("isActive").get_to(roomData.isActive);
}

