#include "CollisionManager.hpp"
#include <iostream>

CollisionManager::CollisionManager(
        int gameType, 
        float screenWidth, 
        float screenHeight
    ) : AbstractManager(gameType, screenWidth, screenHeight) {
        std::cout << "CollisionManager created (inheriting from AbstractManager)." << std::endl;
    }

CollisionManager::~CollisionManager() {
    std::cout << "CollisionManager destroyed." << std::endl;
}
//Método AABB: "Axis-Aligned Bounding Box." 
bool CollisionManager::checkAABB(float x1, float y1, float w1, float h1,
                                 float x2, float y2, float w2, float h2) const {
    // Verifique se os retângulos se sobrepõem no eixo X
    bool overlapX = (x1 < x2 + w2) && (x1 + w1 > x2);
    // Verifique se os retângulos se sobrepõem no eixo Y
    bool overlapY = (y1 < y2 + h2) && (y1 + h1 > y2);

    return overlapX && overlapY;
}

bool CollisionManager::checkCollision(const Bird& bird, const ObstacleManager& obstacleManager, float groundYPosition) const {
    if (bird.getColliderY() + bird.getColliderHeight() >= groundYPosition) {
        std::cout << "Collision: Bird hit ground!" << std::endl;
        return true;
    }

    const auto& pipes = obstacleManager.getPipes();
    for (const auto& pipe : pipes) {
        if (!pipe) {
            continue;
        }

        if (checkAABB(bird.getColliderX(), bird.getColliderY(), bird.getColliderWidth(), bird.getColliderHeight(), pipe->getX(), pipe->getY(), pipe->getWidth(), pipe->getHeight())) {
            std::cout << "Collision: Bird hit a pipe!" << std::endl;
            return true;
        }
    }

    return false;
}