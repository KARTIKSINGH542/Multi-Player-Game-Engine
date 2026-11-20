#pragma once
#include <cstddef>
#include <iosfwd>
#include <stdexcept>
#include <string>
#include <vector>

#include "Participant.h"
#include "Rules.h"

// Thrown when the input stream ends (e.g. Ctrl+D) while a value is still needed.
class InputClosed : public std::runtime_error {
public:
    InputClosed() : std::runtime_error("input stream closed") {}
};

struct RoundResult {
    int target = 0;                    // the Guesser's number
    std::vector<std::size_t> winners;  // indices into the players vector
};

// The Umpire is the single place where input is validated and a round is judged.
//  - askInteger / collectGuess : validate everything typed by a user
//  - evaluateRound             : pure logic, decides who wins (no I/O -> easy to test)
//  - announceRound             : prints the outcome
class Umpire {
public:
    Umpire(std::istream& in, std::ostream& out);

    // Re-prompts until the user enters a whole number in [lo, hi].
    int askInteger(const std::string& prompt, int lo, int hi);

    // Collects one validated guess for a participant according to the ruleset.
    void collectGuess(Participant& who, const Ruleset& rules);

    RoundResult evaluateRound(const Guesser& guesser, const std::vector<Player>& players,
                              const Ruleset& rules) const;

    void announceRound(int roundNumber, const RoundResult& result,
                       const std::vector<Player>& players) const;

private:
    std::istream& in_;
    std::ostream& out_;
};
