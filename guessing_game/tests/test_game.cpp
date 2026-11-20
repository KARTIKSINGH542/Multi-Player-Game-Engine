// Minimal self-checking tests (no framework). Build and run: see README.md.
#include <cassert>
#include <iostream>
#include <sstream>

#include "Game.h"
#include "Participant.h"
#include "Rules.h"
#include "Scoreboard.h"
#include "Umpire.h"

static void test_participant_state() {
    Player p("P");
    assert(!p.hasGuessed());
    bool threw = false;
    try { p.guess(); } catch (const std::logic_error&) { threw = true; }
    assert(threw);
    p.setGuess(7);
    assert(p.guess() == 7);
    p.resetGuess();
    assert(!p.hasGuessed());
}

static void test_exact_rule_all_winner_combinations() {
    std::istringstream in; std::ostringstream out;
    Umpire u(in, out);
    ExactMatchRule rule(1, 10, 1);
    Guesser g; g.setGuess(5);
    std::vector<Player> ps{Player("a"), Player("b"), Player("c")};
    // Every subset of players matching the Guesser must be reported exactly.
    for (int mask = 0; mask < 8; ++mask) {
        for (int i = 0; i < 3; ++i) ps[i].setGuess((mask >> i) & 1 ? 5 : 6);
        auto r = u.evaluateRound(g, ps, rule);
        std::size_t expected = 0;
        for (int i = 0; i < 3; ++i) expected += (mask >> i) & 1;
        assert(r.winners.size() == expected);
        for (auto idx : r.winners) assert(ps[idx].guess() == 5);
    }
}

static void test_scales_to_many_players() {
    std::istringstream in; std::ostringstream out;
    Umpire u(in, out);
    ExactMatchRule rule(1, 100, 1);
    Guesser g; g.setGuess(42);
    std::vector<Player> ps;
    for (int i = 0; i < 25; ++i) { ps.emplace_back("p" + std::to_string(i)); ps.back().setGuess(i % 2 ? 42 : 1); }
    assert(u.evaluateRound(g, ps, rule).winners.size() == 12);
}

static void test_proximity_rule() {
    ProximityRule rule(1, 100, 2, 3);
    assert(rule.isWin(50, 52));
    assert(rule.isWin(50, 47));
    assert(!rule.isWin(50, 54));
    assert(rule.pointsPerWin() == 2);
}

static void test_input_validation() {
    std::istringstream in("abc\n\n12x\n500\n-3\n 7 \n");
    std::ostringstream out;
    Umpire u(in, out);
    assert(u.askInteger("n", 1, 10) == 7);  // rejects text, empty, trailing junk, out of range
    bool closed = false;
    try { u.askInteger("n", 1, 10); } catch (const InputClosed&) { closed = true; }
    assert(closed);
}

static void test_bad_ruleset_arguments() {
    bool threw = false;
    try { ExactMatchRule r(10, 5, 1); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);
}

static void test_scoreboard() {
    Scoreboard b({"a", "b", "c"});
    assert(b.leaders().empty());
    b.award(1, 2); b.award(2, 2); b.award(0, 1);
    auto l = b.leaders();
    assert(l.size() == 2 && l[0] == "b" && l[1] == "c");
    assert(b.ranked().front().points == 2);
}

static void test_full_game() {
    // 2 players, 2 rounds, exact rule 1..10, 1 point.
    // Round 1: guesser 5, P1=5, P2=3   -> P1 scores
    // Round 2: guesser 8, P1=2, P2=8   -> P2 scores   => tie 1-1
    std::istringstream in("5\n5\n3\n8\n2\n8\n");
    std::ostringstream out;
    Umpire u(in, out);
    Game g(u, out, std::make_unique<ExactMatchRule>(1, 10, 1), 2, 2);
    g.run();
    assert(g.scoreboard().standings()[0].points == 1);
    assert(g.scoreboard().standings()[1].points == 1);
    assert(out.str().find("Tie between") != std::string::npos);
}

int main() {
    test_participant_state();
    test_exact_rule_all_winner_combinations();
    test_scales_to_many_players();
    test_proximity_rule();
    test_input_validation();
    test_bad_ruleset_arguments();
    test_scoreboard();
    test_full_game();
    std::cout << "All tests passed.\n";
    return 0;
}
