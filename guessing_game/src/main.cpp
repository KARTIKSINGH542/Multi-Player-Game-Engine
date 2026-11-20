// Multi-player number-guessing game (C++17).
//   Guesser  : picks a number each round.
//   Players  : any number of players each guess a number.
//   Umpire   : validates all input and decides who wins each round.
// Build: see README.md.
#include <iostream>
#include <memory>
#include "Game.h"
#include "Rules.h"
#include "Umpire.h"

int main() {
    Umpire umpire(std::cin, std::cout);
    try {
        std::cout << "=== Setup ===\n";
        const int players = umpire.askInteger("Number of players", 1, 50);
        const int rounds = umpire.askInteger("Number of rounds", 1, 100);
        const int ruleChoice =
            umpire.askInteger("Ruleset (1 = exact match, 2 = within tolerance)", 1, 2);
        const int lo = umpire.askInteger("Smallest allowed number", -1000000, 999999);
        const int hi = umpire.askInteger("Largest allowed number", lo + 1, 1000000);
        const int points = umpire.askInteger("Points per win", 1, 100);

        std::unique_ptr<Ruleset> rules;
        if (ruleChoice == 1) {
            rules = std::make_unique<ExactMatchRule>(lo, hi, points);
        } else {
            const int tol = umpire.askInteger("Tolerance (+/-)", 1, hi - lo);
            rules = std::make_unique<ProximityRule>(lo, hi, points, tol);
        }

        Game game(umpire, std::cout, std::move(rules), static_cast<std::size_t>(players), rounds);
        game.run();
    } catch (const InputClosed&) {
        std::cerr << "\nInput ended before the game finished. Exiting.\n";
        return 1;
    }
    return 0;
}
