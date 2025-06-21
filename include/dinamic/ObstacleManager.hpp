#ifndef OBSTACLE_MANAGER_HPP
#define OBSTACLE_MANAGER_HPP

#include "Obstacle.hpp"
#include "AbstractManager.hpp"
#include <iostream>
#include <vector>
#include <memory>

class ObstacleManager : public AbstractManager {
    private:
        std::vector<std::unique_ptr<Obstacle>> _pipes;
        float _spawnTimer;
        float _spawnInterval;
        float _pipeScrollSpeed;
        float _minPipeGap;
        float _maxPipeGap;
        float _pipeWidth;
        float _pipeHeight;
        int _deletedPipes;
        ALLEGRO_BITMAP* _topPipeSprite;
        ALLEGRO_BITMAP* _bottomPipeSprite;

    public:
        ObstacleManager(
            float spawnInterval, 
            float pipeScrollSpeed,
            float minPipeGap, 
            float maxPipeGap, 
            float pipeWidth, 
            float pipeHeight,
            float screenWidth, 
            float screenHeight, 
            ALLEGRO_BITMAP* topSprite, 
            ALLEGRO_BITMAP* bottomSprite
        );

        ~ObstacleManager();

        void update(double deltaTime);
        void draw();
        void setScrollSpeed(float newSpeed);
        void setSpawnInterval(float newInterval);
        const std::vector<std::unique_ptr<Obstacle>>&getPipes() const;

        
    private:
        void spawnPipes();
};

#endif