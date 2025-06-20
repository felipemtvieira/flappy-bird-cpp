#include "DinamicManager.hpp"

DinamicManager::DinamicManager(int gameType) : _gameType(gameType) {}

DinamicManager::~DinamicManager(){
    std::cout << "DinamicManager Destructor Called" << std::endl;
}