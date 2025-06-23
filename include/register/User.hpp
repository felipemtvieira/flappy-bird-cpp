#ifndef USER_HPP
#define USER_HPP

#include <string> // For std::string

// Simple struct to hold user data
struct User {
    std::string name;
    std::string nickname;
    int score; // Current score for a single game session (will be reset)
    int gamesPlayed; // NEW: How many games this user has played
    int bestScore; 

    // Default constructor for convenience
    User() : name(""), nickname(""), score(0), gamesPlayed(0), bestScore(0) {}

    // Parameterized constructor
    User(const std::string& name, const std::string& nickname, int score = 0, int gamesPlayed = 0, int bestScore = 0)
        : name(name), nickname(nickname), score(score), gamesPlayed(gamesPlayed), bestScore(bestScore) {}
};

#endif // USER_HPP