#ifndef OBSTACLE_MANAGER_HPP
#define OBSTACLE_MANAGER_HPP

#include "Obstacle.hpp"
#include "DinamicManager.hpp"
#include <iostream>
#include <vector>
#include <memory>

class ObstacleManager : public DinamicManager {
    private:
        std::vector<std::unique_ptr<Obstacle>> pipes;
        float _spawnTimer;
        float _spawnInterval;
        float _pipeScrollSpeed;
        float _minPipeGap;
        float _maxPipeGap;
        float _pipeWidth;
        float _screenWidth;
        float _screenHeight;

        ALLEGRO_BITMAP* _topPipeSprite;
        ALLEGRO_BITMAP* _bottomPipeSprite;

    public:
        ObstacleManager(
            float spawnInterval, 
            float pipeScrollSpeed,
            float minPipeGap, 
            float maxPipeGap, 
            float pipeWidth, 
            float screenWidth, 
            float screenHeight, 
            ALLEGRO_BITMAP* topSprite, 
            ALLEGRO_BITMAP* bottomSprite
        );

        ~ObstacleManager();

        void update(double deltaTime);
        void draw();

    private:
        void spawnPipes();
};

#endif