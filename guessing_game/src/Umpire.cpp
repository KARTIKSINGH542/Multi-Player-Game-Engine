#include "Umpire.h"

#include <charconv>
#include <iostream>

namespace {

std::string trim(const std::string& s) {
    const char* ws = " \t\r\n";
    const auto b = s.find_first_not_of(ws);
    if (b == std::string::npos) return "";
    const auto e = s.find_last_not_of(ws);
    return s.substr(b, e - b + 1);
}

// Accepts only a complete whole number such as "42" or "-7" (no trailing text).
bool parseInt(const std::string& text, int& value) {
    const std::string t = trim(text);
    if (t.empty()) return false;
    const char* first = t.data();
    const char* last = t.data() + t.size();
    auto [ptr, ec] = std::from_chars(first, last, value);
    return ec == std::errc() && ptr == last;
}

}  // namespace

Umpire::Umpire(std::istream& in, std::ostream& out) : in_(in), out_(out) {}

int Umpire::askInteger(const std::string& prompt, int lo, int hi) {
    std::string line;
    while (true) {
        out_ << prompt << " [" << lo << "-" << hi << "]: ";
        if (!std::getline(in_, line)) throw InputClosed();
        int value = 0;
        if (!parseInt(line, value)) {
            out_ << "  Invalid input. Please enter a whole number.\n";
            continue;
        }
        if (value < lo || value > hi) {
            out_ << "  Out of range. Please enter a number from " << lo << " to " << hi << ".\n";
            continue;
        }
        return value;
    }
}

void Umpire::collectGuess(Participant& who, const Ruleset& rules) {
    const std::string prompt = who.name() + ", enter your guess";
    who.setGuess(askInteger(prompt, rules.minGuess(), rules.maxGuess()));
}

RoundResult Umpire::evaluateRound(const Guesser& guesser, const std::vector<Player>& players,
                                  const Ruleset& rules) const {
    RoundResult result;
    result.target = guesser.guess();  // throws if the Guesser has not guessed
    for (std::size_t i = 0; i < players.size(); ++i) {
        if (rules.isWin(players[i].guess(), result.target)) result.winners.push_back(i);
    }
    return result;
}

void Umpire::announceRound(int roundNumber, const RoundResult& result,
                           const std::vector<Player>& players) const {
    out_ << "\n--- Round " << roundNumber << " result ---\n";
    out_ << "Guesser's number: " << result.target << "\n";
    for (const auto& p : players) out_ << "  " << p.name() << " guessed " << p.guess() << "\n";
    if (result.winners.empty()) {
        out_ << "No player won this round.\n";
        return;
    }
    out_ << "Winner(s): ";
    for (std::size_t k = 0; k < result.winners.size(); ++k) {
        if (k) out_ << ", ";
        out_ << players[result.winners[k]].name();
    }
    out_ << "\n";
}
