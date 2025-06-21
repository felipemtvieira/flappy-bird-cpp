#include "CollisionManager.hpp"
#include <iostream> // For debugging output

// CollisionManager constructor, calling AbstractManager's constructor
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

// Helper function for Axis-Aligned Bounding Box collision detection
bool CollisionManager::checkAABB(float x1, float y1, float w1, float h1,
                                 float x2, float y2, float w2, float h2) const {
    // Check if the rectangles overlap on the X axis
    bool overlapX = (x1 < x2 + w2) && (x1 + w1 > x2);
    // Check if the rectangles overlap on the Y axis
    bool overlapY = (y1 < y2 + h2) && (y1 + h1 > y2);

    return overlapX && overlapY;
}

bool CollisionManager::checkCollision(const Bird& bird, const ObstacleManager& obstacleManager, float groundYPosition) const { // Added const
    // 1. Collision with Ground
    // Bird's bottom edge (_y + _height) vs. ground's top edge (groundYPosition)
    if (bird.getColliderY() + bird.getColliderHeight() >= groundYPosition) {
        std::cout << "Collision: Bird hit ground!" << std::endl;
        return true;
    }

    // 2. Collision with Pipes
    const auto& pipes = obstacleManager.getPipes();
    for (const auto& pipe : pipes) {
        if (!pipe) {
            continue;
        }

        // Check AABB collision between bird and current pipe
        if (checkAABB(bird.getColliderX(), bird.getColliderY(), bird.getColliderWidth(), bird.getColliderHeight(), pipe->getX(), pipe->getY(), pipe->getWidth(), pipe->getHeight())) {
            std::cout << "Collision: Bird hit a pipe!" << std::endl;
            return true;
        }
    }

    // 3. Collision with Ceiling (Optional: if bird flies too high)
    // If you add this, and it depends on _screenHeight, you can now access _screenHeight directly.
    if (bird.getColliderY() < 0) { // Bird's top edge goes above screen
        std::cout << "Collision: Bird hit ceiling!" << std::endl;
        return true;
    }

    return false; // No collision detected
}