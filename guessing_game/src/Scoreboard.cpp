#include "Scoreboard.h"

#include <algorithm>

Scoreboard::Scoreboard(const std::vector<std::string>& names) {
    for (const auto& n : names) standings_.push_back({n, 0});
}

void Scoreboard::award(std::size_t playerIndex, int points) {
    standings_.at(playerIndex).points += points;
}

const std::vector<Standing>& Scoreboard::standings() const { return standings_; }

std::vector<Standing> Scoreboard::ranked() const {
    std::vector<Standing> copy = standings_;
    std::stable_sort(copy.begin(), copy.end(),
                     [](const Standing& a, const Standing& b) { return a.points > b.points; });
    return copy;
}

std::vector<std::string> Scoreboard::leaders() const {
    std::vector<std::string> result;
    int best = 0;
    for (const auto& s : standings_) best = std::max(best, s.points);
    if (best == 0) return result;  // nobody scored
    for (const auto& s : standings_)
        if (s.points == best) result.push_back(s.name);
    return result;
}
