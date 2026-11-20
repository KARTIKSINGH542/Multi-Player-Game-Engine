#include "Rules.h"

#include <cstdlib>
#include <stdexcept>

Ruleset::Ruleset(int minGuess, int maxGuess, int pointsPerWin)
    : min_(minGuess), max_(maxGuess), points_(pointsPerWin) {
    if (min_ >= max_) throw std::invalid_argument("minGuess must be less than maxGuess");
    if (points_ < 1) throw std::invalid_argument("pointsPerWin must be at least 1");
}

int Ruleset::minGuess() const { return min_; }
int Ruleset::maxGuess() const { return max_; }
int Ruleset::pointsPerWin() const { return points_; }

std::string ExactMatchRule::name() const { return "Exact match"; }
std::string ExactMatchRule::describe() const {
    return "Exact match: guess exactly the Guesser's number (" + std::to_string(minGuess()) +
           "-" + std::to_string(maxGuess()) + ") to win " + std::to_string(pointsPerWin()) +
           " point(s).";
}
bool ExactMatchRule::isWin(int playerGuess, int target) const { return playerGuess == target; }

ProximityRule::ProximityRule(int minGuess, int maxGuess, int pointsPerWin, int tolerance)
    : Ruleset(minGuess, maxGuess, pointsPerWin), tolerance_(tolerance) {
    if (tolerance_ < 1) throw std::invalid_argument("tolerance must be at least 1");
}
std::string ProximityRule::name() const { return "Proximity"; }
std::string ProximityRule::describe() const {
    return "Proximity: guess within +/-" + std::to_string(tolerance_) +
           " of the Guesser's number (" + std::to_string(minGuess()) + "-" +
           std::to_string(maxGuess()) + ") to win " + std::to_string(pointsPerWin()) +
           " point(s).";
}
bool ProximityRule::isWin(int playerGuess, int target) const {
    return std::abs(playerGuess - target) <= tolerance_;
}
