#pragma once
#include <string>

#include "json.hpp"
using nlohmann::json;

struct Highscore {
    std::string name;
    unsigned int score;
};

inline void to_json(json& j, const Highscore& highscore) {
    j = json{ {"name", highscore.name}, {"score", highscore.score} };
}

inline void from_json(const json& j, Highscore& highscore) {
    j.at("name").get_to(highscore.name);
    j.at("score").get_to(highscore.score);
}
