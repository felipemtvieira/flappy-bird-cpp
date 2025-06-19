#ifndef DINAMIC_MANAGER_HPP
#define DINAMIC_MANAGER_HPP

#include <iostream>
#include <vector>
#include <memory>

class DinamicManager {
    private:
        int _gameType;
        // float _screenWidth;
        // float _screenHeight;

    public:
        DinamicManager(int gameType);
        ~DinamicManager();
};

#endif