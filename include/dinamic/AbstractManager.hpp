#ifndef ABSTRACT_MANAGER_HPP
#define ABSTRACT_MANAGER_HPP

#include <iostream>
#include <vector>
#include <memory>

class AbstractManager {
    protected:
        int _gameType;
        float _screenWidth;
        float _screenHeight;

    public:
        AbstractManager(int gameType, float _screenWidth, float _screenHeight);
        ~AbstractManager();
};

#endif