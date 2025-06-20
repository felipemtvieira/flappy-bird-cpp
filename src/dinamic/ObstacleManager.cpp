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
    _deletedPipes(0),
    _topPipeSprite(topPipeSprite), 
    _bottomPipeSprite(bottomPipeSprite) 
    {}

ObstacleManager::~ObstacleManager(){}

void ObstacleManager::update(double deltaTime) {
    this->_spawnTimer += deltaTime;

    if (this->_spawnTimer >= this->_spawnInterval) {
        spawnPipes();
        this->_spawnTimer = 0.0f;
    }

    for (auto it = pipes.begin(); it != pipes.end(); ) {
        (*it)->update(deltaTime);
        if ((*it)->isOffScreen()) {
            it = pipes.erase(it);
            this->_deletedPipes += 1;
        } else {
            ++it;
        }
    }

    // if(this->_deletedPipes % 2 == 0){
    //     this->_pipeScrollSpeed *= 1.2;
    //     // this->_spawnInterval *= 0.8;
    // }
}

void ObstacleManager::draw() {
    for (const auto& pipe : pipes) {
        pipe->draw();
    }
}

void ObstacleManager::spawnPipes() {
    float n = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    float topPipeY = -300 * n;
    float gap;
    float pipeX = this->_screenWidth - 100;

    if(this->_maxPipeGap == this->_minPipeGap){
        gap == this->_maxPipeGap;
    }

    float bottomPipeY = gap + topPipeY + this->_pipeWidth;

    // float maxGapTop = _screenHeight - (_screenHeight * 0.2f) - _maxPipeGap; // Assuming ground takes ~20% of screen
    // float minGapTop = _screenHeight * 0.1f; // Minimum Y for the top of the gap (e.g., 10% from top)

    // float gapTopY = minGapTop + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (maxGapTop - minGapTop));
    // float currentGapHeight = _minPipeGap + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (_maxPipeGap - _minPipeGap));


    // Top pipe: extends from the top of the screen down to `gapTopY`
    // Its actual Y position will be `gapTopY - screenHeight` since the image starts at Y=0
    pipes.push_back(
        std::make_unique<Obstacle>(
            pipeX,                      // _x
            topPipeY,                   // _y
            this->_pipeWidth,
            550,                        // _height
            this->_pipeScrollSpeed, 
            this->_topPipeSprite, 
            true                        // _isTopPipe
        )
    );

    // Bottom pipe: extends from `gapTopY + currentGapHeight` to the bottom of the screen
    pipes.push_back(
        std::make_unique<Obstacle>(
            pipeX,                      // _x
            bottomPipeY,                // _y
            this->_pipeWidth, 
            this->_screenHeight,        // _height
            this->_pipeScrollSpeed, 
            this->_bottomPipeSprite, 
            false                       // _isTopPipe
        )
    );
}
