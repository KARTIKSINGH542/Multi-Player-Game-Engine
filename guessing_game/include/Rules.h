#pragma once
#include <memory>
#include <string>

// A Ruleset decides what a valid guess is and when a player wins.
// New rulesets are added by deriving from Ruleset; Umpire and Game never change.
class Ruleset {
public:
    Ruleset(int minGuess, int maxGuess, int pointsPerWin);
    virtual ~Ruleset() = default;

    int minGuess() const;
    int maxGuess() const;
    int pointsPerWin() const;

    virtual std::string name() const = 0;
    virtual std::string describe() const = 0;
    virtual bool isWin(int playerGuess, int target) const = 0;

private:
    int min_;
    int max_;
    int points_;
};

// Win only by guessing exactly the Guesser's number (the original problem).
class ExactMatchRule : public Ruleset {
public:
    using Ruleset::Ruleset;
    std::string name() const override;
    std::string describe() const override;
    bool isWin(int playerGuess, int target) const override;
};

// Win by guessing within +/- tolerance of the Guesser's number.
class ProximityRule : public Ruleset {
public:
    ProximityRule(int minGuess, int maxGuess, int pointsPerWin, int tolerance);
    std::string name() const override;
    std::string describe() const override;
    bool isWin(int playerGuess, int target) const override;

private:
    int tolerance_;
};
