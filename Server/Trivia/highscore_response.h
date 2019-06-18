#pragma once
#include <vector>

#include "json.hpp"
using nlohmann::json;

#include "highscore.h"

struct HighscoreResponse {
    unsigned int status;
    std::vector<Highscore> highscores;
};

inline void to_json(json& j, const HighscoreResponse& res) {
    j = json{ {"status", res.status}, {"highscores", res.highscores} };
}

inline void from_json(const json& j, HighscoreResponse& res) {
    j.at("status").get_to(res.status);
    j.at("highscores").get_to(res.highscores);
}