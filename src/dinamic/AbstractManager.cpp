#include "../../include/dinamic/AbstractManager.hpp"

AbstractManager::AbstractManager(int gameType, float screenWidth, float screenHeight) : _gameType(gameType), _screenWidth(screenWidth), _screenHeight(screenHeight) {
    std::cout << "AbstractManager Constructor" << std::endl;

}

AbstractManager::~AbstractManager(){
    std::cout << "AbstractManager Destructor Called" << std::endl;
}