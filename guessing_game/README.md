# Object-Oriented Multi-Player Number Guessing Game (C++17)

A console game with three kinds of entities:

* **Guesser**  - picks a number every round.
* **Player**   - any number of players (1-50) each guess a number.
* **Umpire**   - validates all input and decides which players matched.

The game runs several rounds, keeps a score for each player, and prints final standings.
<!-- for compile
g++ -std=c++17 -I..\include main.cpp Game.cpp Participant.cpp Umpire.cpp Rules.cpp Scoreboard.cpp -o game.exe
for run
.\game.exe -->
## Build and run

    # with g++ (no CMake needed)
    g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/*.cpp -o guessing_game
    ./guessing_game

    # run the tests
    g++ -std=c++17 -Iinclude src/Participant.cpp src/Rules.cpp src/Scoreboard.cpp \
        src/Umpire.cpp src/Game.cpp tests/test_game.cpp -o test_game && ./test_game

    # or with CMake
    cmake -S . -B build && cmake --build build && ctest --test-dir build

## Project layout

    include/Participant.h   Participant (abstract base), Guesser, Player
    include/Rules.h         Ruleset (abstract), ExactMatchRule, ProximityRule
    include/Scoreboard.h    multi-round scoring
    include/Umpire.h        input validation + round evaluation + announcements
    include/Game.h          round loop, ties to everything together
    src/*.cpp               implementations (main.cpp = setup menu)
    tests/test_game.cpp     self-checking tests

## Mapping to the resume bullets

| Resume bullet | Where it is implemented |
|---|---|
| Multi-entity interactive guessing game using core OOP principles and modular architecture | Separate headers/sources per module; inheritance (`Guesser`, `Player` derive from `Participant`); polymorphism (`role()`, `Ruleset::isWin()`); abstraction (abstract `Participant`, `Ruleset`) |
| Guesser, Player, Umpire classes enforcing strict data encapsulation and separate system states | All data members are `private`; access only via methods. Per-entity state = guess (in `Participant`); game state = `Scoreboard` + round counter (in `Game`); rule config = `Ruleset` |
| Centralized validation logic in Umpire to process inputs and evaluate win/loss | `Umpire::askInteger` / `collectGuess` validate every input (text, empty, trailing junk, out of range, end of input); `Umpire::evaluateRound` decides winners |
| Dynamic scaling of players, custom rulesets, multi-round scoring | `Game` takes any number of players; new rules = new `Ruleset` subclass (`ExactMatchRule`, `ProximityRule` included); `Scoreboard` accumulates points over rounds |

## Design notes (useful for interviews)

* `Umpire::evaluateRound` has **no I/O**, so it is unit-tested directly.
* `Umpire` takes `std::istream&` / `std::ostream&`, so tests feed it a `std::istringstream`.
* Input uses `std::getline` + `std::from_chars`; a bad entry never leaves `cin` in a failed state.
* The original 7-way `if/else` chain is replaced by one loop over players, so it works for any N.
* Adding a ruleset: derive from `Ruleset`, implement `name()`, `describe()`, `isWin()`; no other class changes.

## Known limits (be honest about these)

* Console only; all guesses are typed by a human (no AI/random players).
* Player names are fixed as "Player 1..N".
* Rules are chosen from a fixed menu in `main.cpp`.
