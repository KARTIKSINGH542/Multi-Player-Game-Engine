#pragma once
#include <optional>
#include <string>

// Base class for every entity that holds a guess.
// Encapsulation: the guess is private; it can only be changed through setGuess()
// and read through guess(). Each object owns its own, independent state.
class Participant {
public:
    explicit Participant(std::string name);
    virtual ~Participant() = default;

    const std::string& name() const;
    virtual std::string role() const = 0;  // polymorphic: "Guesser" / "Player"

    bool hasGuessed() const;
    int guess() const;            // throws std::logic_error if no guess was set
    void setGuess(int value);
    void resetGuess();

private:
    std::string name_;
    std::optional<int> guess_;
};

// The entity whose number every Player tries to match.
class Guesser : public Participant {
public:
    explicit Guesser(std::string name = "Guesser");
    std::string role() const override;
};

// A competing entity. Any number of Players can exist in a game.
class Player : public Participant {
public:
    explicit Player(std::string name);
    std::string role() const override;
};
