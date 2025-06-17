#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <string.h>

class Player {
private:
    int points;
    std::string name;
    std::string nickname;

public:
    Player();

    Player(int val);

    Player(const Player& outroHeapInt);

    ~Player();
};

#endif 