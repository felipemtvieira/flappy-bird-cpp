#ifndef GAME_ERROR_HPP
#define GAME_ERROR_HPP

#include <stdexcept>
#include <string>   

// Base exception class for all game-related errors
class GameError : public std::runtime_error {
public:
    explicit GameError(const std::string& message)
        : std::runtime_error("Game Error: " + message) {}
};

// Specific exception for Allegro initialization failures
class AllegroInitError : public GameError {
public:
    explicit AllegroInitError(const std::string& message)
        : GameError("Allegro Initialization Failed - " + message) {}
};

// Specific exception for display creation failures
class DisplayCreationError : public GameError {
public:
    explicit DisplayCreationError(const std::string& message)
        : GameError("Display Creation Failed - " + message) {}
};

// Specific exception for timer creation failures
class TimerCreationError : public GameError {
public:
    explicit TimerCreationError(const std::string& message)
        : GameError("Timer Creation Failed - " + message) {}
};

// Specific exception for event queue creation failures
class EventQueueCreationError : public GameError {
public:
    explicit EventQueueCreationError(const std::string& message)
        : GameError("Event Queue Creation Failed - " + message) {}
};

// Specific exception for asset loading failures
class AssetLoadError : public GameError {
public:
    explicit AssetLoadError(const std::string& message)
        : GameError("Asset Loading Failed - " + message) {}
};

// Specific exception for file opening failures
class FileError : public GameError {
public:
    explicit FileError(const std::string& message)
        : GameError("Opening User Register File Failed - " + message) {}
};

#endif // GAME_ERROR_HPP