#include "Game.hpp"
#include <iostream>

int main() {
    const float SCREEN_WIDTH = 800;
    const float SCREEN_HEIGHT = 600;

    Game game(SCREEN_WIDTH, SCREEN_HEIGHT);

    if (!game.initialize()) {
        std::cerr << "Game initialization failed. Exiting." << std::endl;
        return 1;
    }

    game.run();

    return 0;
}