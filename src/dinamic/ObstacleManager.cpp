#include "ObstacleManager.hpp"
#include <iostream>

ObstacleManager::ObstacleManager(
    float spawnInterval, 
    float pipeScrollSpeed, 
    float minPipeGap, 
    float maxPipeGap, 
    float pipeWidth, 
    float pipeHeight,
    float screenWidth, 
    float screenHeight, 
    ALLEGRO_BITMAP* topPipeSprite, 
    ALLEGRO_BITMAP* bottomPipeSprite
)
    :
    AbstractManager(1, screenWidth, screenHeight), 
    _spawnTimer(0.0f), 
    _spawnInterval(spawnInterval), 
    _pipeScrollSpeed(pipeScrollSpeed), 
    _minPipeGap(minPipeGap), 
    _maxPipeGap(maxPipeGap), 
    _pipeWidth(pipeWidth),
    _pipeHeight(pipeHeight),
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

    for (auto it = this->_pipes.begin(); it != this->_pipes.end(); ) {
        (*it)->update(deltaTime);
        if ((*it)->isOffScreen()) {
            it = this->_pipes.erase(it);
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
    for (const auto& pipe : this->_pipes) {
        pipe->draw();
    }
}

void ObstacleManager::spawnPipes() {
    float groundYPosition = this->_screenHeight * 0.8;
    float maxGapTopYAllowed = groundYPosition - _maxPipeGap - 20;
    float minGapTopYAllowed = 50.0f;

    if (maxGapTopYAllowed < minGapTopYAllowed) {
        maxGapTopYAllowed = minGapTopYAllowed;
    }

    float gapTopY = minGapTopYAllowed + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (maxGapTopYAllowed - minGapTopYAllowed));
    float currentGapHeight = _minPipeGap + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (_maxPipeGap - _minPipeGap));

    float topPipeY = 0.0f;
    float topPipeHeight = gapTopY;

    float bottomPipeY = gapTopY + currentGapHeight;
    float bottomPipeHeight = groundYPosition - bottomPipeY; // Correctly calculated height

    topPipeHeight = std::max(0.0f, topPipeHeight);
    bottomPipeHeight = std::max(0.0f, bottomPipeHeight);

    this->_pipes.push_back(
        std::make_unique<Obstacle>(
            _screenWidth,        // _x
            topPipeY,            // _y
            this->_pipeWidth,    // _width
            topPipeHeight,       // <-- CORRECT: Pass the CALCULATED height
            this->_screenWidth,     // screenWidth for Element
            this->_screenHeight,    // screenHeight for Element
            this->_pipeScrollSpeed, // speed
            this->_topPipeSprite,   // bitmap
            true // _isTopPipe
        )
    );

    this->_pipes.push_back(
        std::make_unique<Obstacle>(
            _screenWidth,         // _x
            bottomPipeY,          // _y
            this->_pipeWidth,
            bottomPipeHeight,     // <-- CORRECT: Pass the CALCULATED height
            this->_screenWidth,      // screenWidth for Element
            this->_screenHeight,     // screenHeight for Element
            this->_pipeScrollSpeed, // speed
            this->_bottomPipeSprite, // bitmap
            false // _isTopPipe
        )
    );
}

const std::vector<std::unique_ptr<Obstacle>>& ObstacleManager::getPipes() const {
    return this->_pipes; // <-- Return the actual _pipes member variable
}

void ObstacleManager::setScrollSpeed(float newSpeed) {
    _pipeScrollSpeed = newSpeed;
    for (const auto& pipe : this->_pipes) {
        pipe->setScrollSpeed(newSpeed);
    }
}

void ObstacleManager::setSpawnInterval(float newInterval) {
    _spawnInterval = newInterval;
}