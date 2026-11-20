#pragma once
#include <cstddef>
#include <string>
#include <vector>

struct Standing {
    std::string name;
    int points;
};

// Keeps the running multi-round score. Separate from Player so that
// game-level state (points) never leaks into per-entity state (a guess).
class Scoreboard {
public:
    explicit Scoreboard(const std::vector<std::string>& names);

    void award(std::size_t playerIndex, int points);
    const std::vector<Standing>& standings() const;   // in player order
    std::vector<Standing> ranked() const;             // highest score first
    std::vector<std::string> leaders() const;         // everyone on the top score (> 0)

private:
    std::vector<Standing> standings_;
};
