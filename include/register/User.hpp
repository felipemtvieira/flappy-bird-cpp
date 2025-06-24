#ifndef USER_HPP
#define USER_HPP

#include <string>

// Struct para dados dos usuários
struct User {
    std::string name;
    std::string nickname;
    int score;
    int gamesPlayed;
    int bestScore; 

    User() : name(""), nickname(""), score(0), gamesPlayed(0), bestScore(0) {}

    User(const std::string& name, const std::string& nickname, int score = 0, int gamesPlayed = 0, int bestScore = 0)
        : name(name), nickname(nickname), score(score), gamesPlayed(gamesPlayed), bestScore(bestScore) {}
};

#endif