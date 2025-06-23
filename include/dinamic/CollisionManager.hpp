#ifndef COLLISION_MANAGER_HPP
#define COLLISION_MANAGER_HPP

#include "AbstractManager.hpp" 
#include "Bird.hpp"
#include "ObstacleManager.hpp"

class CollisionManager : public AbstractManager {
public:
    CollisionManager(int gameType, float screenWidth, float screenHeight);
    ~CollisionManager();

    // Verifica colisões entre o pássaro e todos os elementos relevantes do jogo
    bool checkCollision(const Bird& bird, const ObstacleManager& obstacleManager, float groundYPosition) const;

private:
    // Função auxiliar para verificar colisão AABB entre dois retângulos
    bool checkAABB(float x1, float y1, float w1, float h1,
                   float x2, float y2, float w2, float h2) const;
};

#endif