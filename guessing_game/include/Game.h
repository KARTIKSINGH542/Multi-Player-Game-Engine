#pragma once
#include <iosfwd>
#include <memory>
#include <vector>

#include "Participant.h"
#include "Rules.h"
#include "Scoreboard.h"
#include "Umpire.h"

// Runs a multi-round game. Three separate kinds of state live in three places:
//   * per-entity state  -> Guesser / Player objects (their current guess)
//   * game-level state  -> Scoreboard and the round counter held by Game
//   * rule configuration -> the Ruleset object
class Game {
public:
    Game(Umpire& umpire, std::ostream& out, std::unique_ptr<Ruleset> rules,
         std::size_t numPlayers, int rounds);

    void run();

    const Scoreboard& scoreboard() const;

private:
    void playRound();
    void printFinalResult() const;

    Umpire& umpire_;
    std::ostream& out_;
    std::unique_ptr<Ruleset> rules_;
    Guesser guesser_;
    std::vector<Player> players_;
    Scoreboard board_;
    int totalRounds_;
    int currentRound_ = 0;
};
