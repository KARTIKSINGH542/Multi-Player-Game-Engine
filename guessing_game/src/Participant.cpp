#include "Participant.h"

#include <stdexcept>
#include <utility>

Participant::Participant(std::string name) : name_(std::move(name)) {}

const std::string& Participant::name() const { return name_; }
bool Participant::hasGuessed() const { return guess_.has_value(); }

int Participant::guess() const {
    if (!guess_) throw std::logic_error(name_ + " has not guessed yet");
    return *guess_;
}

void Participant::setGuess(int value) { guess_ = value; }
void Participant::resetGuess() { guess_.reset(); }

Guesser::Guesser(std::string name) : Participant(std::move(name)) {}
std::string Guesser::role() const { return "Guesser"; }

Player::Player(std::string name) : Participant(std::move(name)) {}
std::string Player::role() const { return "Player"; }
