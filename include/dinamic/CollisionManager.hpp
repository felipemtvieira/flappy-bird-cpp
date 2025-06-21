#ifndef COLLISION_MANAGER_HPP
#define COLLISION_MANAGER_HPP

#include "AbstractManager.hpp" // NEW: Inherit from AbstractManager
#include "Bird.hpp"
#include "ObstacleManager.hpp"

// CollisionManager now inherits from AbstractManager
class CollisionManager : public AbstractManager {
public:
    // Constructor now takes AbstractManager's parameters
    CollisionManager(int gameType, float screenWidth, float screenHeight);
    ~CollisionManager();

    // Checks for collisions between the bird and all relevant game elements
    bool checkCollision(const Bird& bird, const ObstacleManager& obstacleManager, float groundYPosition) const; // Added const

private:
    // Helper function to check AABB collision between two rectangles
    bool checkAABB(float x1, float y1, float w1, float h1,
                   float x2, float y2, float w2, float h2) const;
};

#endif // COLLISION_MANAGER_HPP