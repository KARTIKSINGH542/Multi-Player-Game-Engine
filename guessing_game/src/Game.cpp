#include "Game.h"
#include <iostream>
#include <stdexcept>
#include <string>

namespace {
std::vector<Player> makePlayers(std::size_t n) {
    if (n < 1) throw std::invalid_argument("a game needs at least one player");
    std::vector<Player> players;
    for (std::size_t i = 1; i <= n; ++i) players.emplace_back("Player " + std::to_string(i));
    return players;
}
std::vector<std::string> namesOf(const std::vector<Player>& players) {
    std::vector<std::string> names;
    for (const auto& p : players) names.push_back(p.name());
    return names;
}
}  // namespace

Game::Game(Umpire& umpire, std::ostream& out, std::unique_ptr<Ruleset> rules,
           std::size_t numPlayers, int rounds)
    : umpire_(umpire),
      out_(out),
      rules_(std::move(rules)),
      guesser_(),
      players_(makePlayers(numPlayers)),
      board_(namesOf(players_)),
      totalRounds_(rounds) {
    if (!rules_) throw std::invalid_argument("a ruleset is required");
    if (totalRounds_ < 1) throw std::invalid_argument("rounds must be at least 1");
}

const Scoreboard& Game::scoreboard() const { return board_; }

void Game::run() {
    out_ << "\n=========== Number Guessing Game ===========\n";
    out_ << "Players: " << players_.size() << "   Rounds: " << totalRounds_ << "\n";
    out_ << "Ruleset: " << rules_->describe() << "\n";
    for (currentRound_ = 1; currentRound_ <= totalRounds_; ++currentRound_) playRound();
    printFinalResult();
}

void Game::playRound() {
    out_ << "\n>>> Round " << currentRound_ << " of " << totalRounds_ << "\n";
    guesser_.resetGuess();
    for (auto& p : players_) p.resetGuess();

    umpire_.collectGuess(guesser_, *rules_);
    for (auto& p : players_) umpire_.collectGuess(p, *rules_);

    const RoundResult result = umpire_.evaluateRound(guesser_, players_, *rules_);
    for (std::size_t idx : result.winners) board_.award(idx, rules_->pointsPerWin());
    umpire_.announceRound(currentRound_, result, players_);

    out_ << "Scores so far:";
    for (const auto& s : board_.standings()) out_ << "  " << s.name << "=" << s.points;
    out_ << "\n";
}

void Game::printFinalResult() const {
    out_ << "\n=========== Final Standings ===========\n";
    int rank = 1;
    for (const auto& s : board_.ranked())
        out_ << rank++ << ". " << s.name << " - " << s.points << " point(s)\n";

    const auto leaders = board_.leaders();
    if (leaders.empty()) {
        out_ << "Nobody scored in this game.\n";
    } else if (leaders.size() == 1) {
        out_ << "Overall winner: " << leaders.front() << "\n";
    } else {
        out_ << "Tie between: ";
        for (std::size_t i = 0; i < leaders.size(); ++i) out_ << (i ? ", " : "") << leaders[i];
        out_ << "\n";
    }
    out_ << "=======================================\n";
}
