#include "ObstacleManager.hpp"
#include <iostream>

ObstacleManager::ObstacleManager(
    float spawnInterval, 
    float pipeScrollSpeed, 
    float minPipeGap, 
    float maxPipeGap, 
    float pipeWidth, 
    float screenWidth, 
    float screenHeight, 
    ALLEGRO_BITMAP* topPipeSprite, 
    ALLEGRO_BITMAP* bottomPipeSprite
)
    :
    DinamicManager(1), 
    _spawnTimer(0.0f), 
    _spawnInterval(spawnInterval), 
    _pipeScrollSpeed(pipeScrollSpeed), 
    _minPipeGap(minPipeGap), 
    _maxPipeGap(maxPipeGap), 
    _pipeWidth(pipeWidth), 
    _screenWidth(screenWidth), 
    _screenHeight(screenHeight), 
    _topPipeSprite(topPipeSprite), 
    _bottomPipeSprite(bottomPipeSprite) 
    {}

ObstacleManager::~ObstacleManager(){}

void ObstacleManager::update(double deltaTime) {
    _spawnTimer += deltaTime;
    if (_spawnTimer >= _spawnInterval) {
        spawnPipes();
        _spawnTimer = 0.0f;
    }

    for (auto it = pipes.begin(); it != pipes.end(); ) {
        (*it)->update(deltaTime);
        if ((*it)->isOffScreen()) {
            it = pipes.erase(it); // Erase returns iterator to next element
        } else {
            ++it;
        }
    }
}

void ObstacleManager::draw() {
    for (const auto& pipe : pipes) {
        pipe->draw();
    }
}

void ObstacleManager::spawnPipes() {
    float n = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    float topPipeY = -300 * n;

    // float maxGapTop = _screenHeight - (_screenHeight * 0.2f) - _maxPipeGap; // Assuming ground takes ~20% of screen
    // float minGapTop = _screenHeight * 0.1f; // Minimum Y for the top of the gap (e.g., 10% from top)

    // float gapTopY = minGapTop + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (maxGapTop - minGapTop));
    // float currentGapHeight = _minPipeGap + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (_maxPipeGap - _minPipeGap));

    std::cout << n << std::endl;

    // Top pipe: extends from the top of the screen down to `gapTopY`
    // Its actual Y position will be `gapTopY - screenHeight` since the image starts at Y=0
    pipes.push_back(
        std::make_unique<Obstacle>(
            _screenWidth, 
            topPipeY,
            _pipeWidth, 
            550, 
            _pipeScrollSpeed, 
            _topPipeSprite, 
            true
        )
    );

    // Bottom pipe: extends from `gapTopY + currentGapHeight` to the bottom of the screen
    // pipes.push_back(
    //     std::make_unique<Obstacle>(
    //         _screenWidth, 
    //         gapTopY + currentGapHeight, 
    //         _pipeWidth, 
    //         _screenHeight, 
    //         _pipeScrollSpeed, 
    //         _bottomPipeSprite, 
    //         false
    //     )
    // );
}
