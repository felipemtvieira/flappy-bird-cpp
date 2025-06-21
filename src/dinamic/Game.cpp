#include "Game.hpp"
#include <iostream> // For debugging output

// Constructor
Game::Game(float SCREEN_WIDTH, float SCREEN_HEIGHT)
    : 
      AbstractManager(1, SCREEN_WIDTH, SCREEN_HEIGHT), 
      currentState(PLAYING),
      display(nullptr),
      eventQueue(nullptr),
      timer(nullptr),
    //   gameFont(nullptr),
    //   birdSprite1(nullptr), birdSprite2(nullptr),
      _ground1(nullptr), // Initialize ground unique_ptr
      _ground2(nullptr),
      _topPipeSprite(nullptr), 
      _bottomPipeSprite(nullptr),
      _groundSprite(nullptr), 
      _backgroundSprite(nullptr),
      _difficulty_scalar(1.0f),
      _lastFrameTime(0.0)
      {
        _groundYPosition = SCREEN_HEIGHT * 0.8f;
    }

Game::~Game() {
    destroyAssets();
    if (timer) al_destroy_timer(timer);
    if (eventQueue) al_destroy_event_queue(eventQueue);
    if (display) al_destroy_display(display);
}

bool Game::initialize() {
    if (!al_init()) {
        std::cerr << "Failed to initialize Allegro!" << std::endl;
        return false;
    }
    if (!al_init_image_addon()) {
        std::cerr << "Failed to initialize Allegro image addon!" << std::endl;
        return false;
    }
    if (!al_install_keyboard()) {
        std::cerr << "Failed to install keyboard!" << std::endl;
        return false;
    }
    if (!al_init_font_addon()) {
        std::cerr << "Failed to initialize font addon!" << std::endl;
        return false;
    }
    if (!al_init_ttf_addon()) {
        std::cerr << "Failed to initialize ttf addon!" << std::endl;
        return false;
    }
    if (!al_init_primitives_addon()) {
        std::cerr << "Failed to initialize primitives addon!" << std::endl;
        return false;
    }

    display = al_create_display(this->_screenWidth, _screenHeight);
    if (!display) {
        std::cerr << "Failed to create display!" << std::endl;
        return false;
    }

    timer = al_create_timer(1.0 / 60.0); // 60 FPS
    if (!timer) {
        std::cerr << "Failed to create timer!" << std::endl;
        al_destroy_display(display);
        return false;
    }

    eventQueue = al_create_event_queue();
    if (!eventQueue) {
        std::cerr << "Failed to create event queue!" << std::endl;
        al_destroy_timer(timer);
        al_destroy_display(display);
        return false;
    }

    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_register_event_source(eventQueue, al_get_keyboard_event_source());

    if (!loadAssets()) {
        std::cerr << "Failed to load game assets!" << std::endl;
        return false;
    }

    // Initialize game objects after assets are loaded
    // Bird parameters: x, y, width, height, gravity, jumpForce, animationFrames
    // bird = std::make_unique<Bird>(50, screenHeight / 2 - 20, 40, 30, 900.0f, 400.0f, birdAnimationFrames);
    
    this->_ObstacleManager = 
        std::make_unique<ObstacleManager>(
            this->_BASE_PIPE_SPAWN_INTERVAL, // spawnInterval
            this->_BASE_PIPE_SCROLL_SPEED, // speed
            this->_MIN_PIPE_GAP, // minGapY 
            this->_MAX_PIPE_GAP, // maxGapY
            this->_PIPE_WIDTH, // pipeWidth
            this->_screenWidth,
            this->_screenHeight,
            this->_topPipeSprite,
            this->_bottomPipeSprite
        );

    float backgroundY = -1 * (this->_screenHeight - _groundYPosition) + 5.0; //Cálculo necessário para o bitmap do fundo "encontrar" com o bitmap do chão corretamente
    // Background parameters: x, y, width, height, scrollSpeed, sprite
    this->_Background = 
        std::make_unique<Background>(
            0, // _x
            backgroundY, // _y
            this->_screenWidth,
            this->_screenHeight,
            this->_screenWidth,
            this->_screenHeight,
            50.0f,
            this->_backgroundSprite
        );

    // Use _bitmap from Element
    float original_bitmap_width = (float)al_get_bitmap_width(_groundSprite);
    float original_bitmap_height = (float)al_get_bitmap_height(_groundSprite);

    std::cout << original_bitmap_width << this->_screenWidth << std::endl;

    // Initialize Ground (UNCOMMENT THIS)
    this->_ground1 = std::make_unique<Ground>(
        0,                       // x (starts at left edge)
        _groundYPosition,        // y (starts at the ground line)
        original_bitmap_width,
        original_bitmap_height,
        _screenWidth,            // width (full screen width for tiling)
        _screenHeight,
        _BASE_PIPE_SCROLL_SPEED, // Base ground scroll speed
        _groundSprite            // The loaded ground sprite
    );

    this->_ground2 = std::make_unique<Ground>(
        _screenWidth,            // x (starts immediately to the right of the first ground)
        _groundYPosition,
        original_bitmap_width,
        original_bitmap_height,
        _screenWidth,
        _screenHeight,
        _BASE_PIPE_SCROLL_SPEED,
        _groundSprite
    );

    // // ScoreManager
    // scoreManager = std::make_unique<ScoreManager>(gameFont);

    this->applyDifficultyScalar();
    al_start_timer(timer);
    _lastFrameTime = al_get_time(); // Initialize last frame time

    return true;
}

void Game::run() {
    bool running = true;
    while (running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(eventQueue, &event);

        double currentTime = al_get_time();
        double deltaTime = currentTime - _lastFrameTime;
        _lastFrameTime = currentTime;

        if (event.type == ALLEGRO_EVENT_TIMER) {
            update(deltaTime);
            draw();
        } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            // handleInput(event);
        } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }
    }
}

void Game::update(double deltaTime) {
    switch (currentState) {
        case MENU:
            // Background might still scroll slowly
            // background->update(deltaTime);
            break;
        case PLAYING:
            updatePlaying(deltaTime);
            break;
        case GAME_OVER:
            // No updates needed, just drawing game over screen
            break;
    }
}

void Game::updatePlaying(double deltaTime) {
    // _Background->update(deltaTime);
    this->_ground1->update(deltaTime);
    this->_ground2->update(deltaTime);
    this->_ObstacleManager->update(deltaTime);
    // bird->update(deltaTime);

    // Ground continuous scrolling logic (UNCOMMENT AND FIX)
    // When a ground segment moves off-screen, reposition it to the right of the other
    // if (_ground1->getX() + _ground1->getWidth() < 0) {
    //     _ground1->setPosition(_ground2->getX() + _ground2->getWidth(), _groundYPosition);
    // }
    // if (_ground2->getX() + _ground2->getWidth() < 0) {
    //     _ground2->setPosition(_ground1->getX() + _ground1->getWidth(), _groundYPosition);
    // }

    // Check for collisions
    // if (collisionManager.checkCollision(*bird, *pipeManager, groundYPosition)) {
    //     currentState = GAME_OVER;
    //     std::cout << "Game Over! Score: " << scoreManager->getScore() << std::endl;
    // }

    // Check for scoring (bird passed a pipe)
    // This is a simple scoring mechanism. More robust solutions might track pipes
    // that have been "passed" by checking if their x-position goes past the bird.
    // for (const auto& pipe : _ObstacleManager->getPipes()) {
        // Only increment if it's a bottom pipe (or top, just pick one per pair)
        // and it hasn't been scored yet (needs a 'scored' flag in Pipe class)
        // For simplicity, let's assume pipes are unique by their pair's position
        // and score when bird passes the *right edge* of the gap
        // if (pipe->getX() + pipe->getWidth() < bird->getX() && pipe->getX() + pipe->getWidth() > bird->getX() - 10 && !pipe->isTopPipe) {
        //     // Need a way to ensure we only score once per pipe pair.
        //     // A simple approach is to add a `bool scored` flag to the Pipe class,
        //     // set it to true after scoring, and only increment if `!scored`.
        //     // For now, this is a placeholder.
        //     // scoreManager->incrementScore();
        // // }
    // }

    // A better scoring mechanism: check if the bird has passed the mid-point of a pipe pair
    // static std::vector<bool> scoredPipes; // To keep track of which pipe pairs have been scored
    // if (_ObstacleManager->getPipes().size() / 2 > scoredPipes.size()) {
    //     scoredPipes.resize(_ObstacleManager->getPipes().size() / 2, false);
    // }

    // for (size_t i = 0; i < pipeManager->getPipes().size(); i += 2) {
    //     // Check the bottom pipe of the pair (assuming pairs are always Top then Bottom)
    //     const auto& bottomPipe = pipeManager->getPipes()[i+1];
    //     if (bird->getX() > bottomPipe->getX() + bottomPipe->getWidth() && !scoredPipes[i/2]) {
    //         scoreManager->incrementScore();
    //         scoredPipes[i/2] = true;
    //     }
    // }
    // Clean up scoredPipes when pipes are removed from pipeManager (TODO: more robust sync)
    // Or, better, integrate scoring directly into PipeManager
}

void Game::draw() {
    al_clear_to_color(al_map_rgb(0, 0, 0)); // Clear screen to black
    // std::cout << "Game.cpp Game::draw() " << this->_screenWidth << std::endl;

    switch (currentState) {
        case MENU:
            // drawMenu();
            break;
        case PLAYING:
            drawPlaying();
            break;
        case GAME_OVER:
            // drawPlaying(); // Draw game state one last time
            // drawGameOver();
            break;
    }

    al_flip_display(); // Swap buffers
}

void Game::drawPlaying() {
    this->_Background->draw();
    this->_ObstacleManager->draw();
    this->_ground1->draw();
    this->_ground2->draw();
    // bird->draw();
    // scoreManager->draw();
}

bool Game::loadAssets() {
    // Load Bird Sprites
    // birdSprite1 = al_load_bitmap("assets/bird_frame_1.png");
    // birdSprite2 = al_load_bitmap("assets/bird_frame_2.png");
    // if (!birdSprite1 || !birdSprite2) {
    //     std::cerr << "Failed to load bird sprites!" << std::endl;
    //     return false;
    // }
    // birdAnimationFrames.push_back(birdSprite1);
    // birdAnimationFrames.push_back(birdSprite2);

    // Load Pipe Sprites (assuming you have images for these)
    _topPipeSprite = al_load_bitmap("assets/top_pipe.png");
    _bottomPipeSprite = al_load_bitmap("assets/bottom_pipe.png");
    if (!_topPipeSprite || !_bottomPipeSprite) {
        std::cerr << "Failed to load pipe sprites! Using fallback rectangles." << std::endl;
        // Not returning false, just warning, so fallback rectangles are used
        _topPipeSprite = nullptr;
        _bottomPipeSprite = nullptr;
        std::cout << "LoadAssets" << std::endl;
    }

    // Load Ground Sprite
    this->_groundSprite = al_load_bitmap("assets/ground.png");
    if (!this->_groundSprite) {
        std::cerr << "Failed to load ground sprite! Using fallback rectangle." << std::endl;
        this->_groundSprite = nullptr;
    } else {
        // Adjust groundYPosition based on ground sprite's actual height if loaded
        // groundYPosition = screenHeight - al_get_bitmap_height(groundSprite);
    }


    // Load Background Sprite
    this->_backgroundSprite = al_load_bitmap("assets/background.png");
    if (!this->_backgroundSprite) {
        std::cerr << "Failed to load background sprite! Using fallback color." << std::endl;
        this->_backgroundSprite = nullptr;
    }

    // // Load Font
    // gameFont = al_load_font("assets/arial.ttf", 36, 0); // Assuming Arial font exists
    // if (!gameFont) {
    //     std::cerr << "Failed to load font! Make sure 'arial.ttf' is in 'assets/'." << std::endl;
    //     gameFont = al_create_builtin_font(); // Fallback to built-in font
    //     if (!gameFont) {
    //         std::cerr << "Failed to create built-in font!" << std::endl;
    //         return false;
    //     }
    // }

    if (!this->_topPipeSprite || !this->_bottomPipeSprite || !this->_backgroundSprite || !this->_groundSprite) { // Added _groundSprite check
        std::cerr << "One or more essential assets failed to load. Returning false." << std::endl;
        return false;
    }

    return true;
}

void Game::destroyAssets() {
    // if (birdSprite1) al_destroy_bitmap(birdSprite1);
    // if (birdSprite2) al_destroy_bitmap(birdSprite2);
    if (this->_topPipeSprite) al_destroy_bitmap(this->_topPipeSprite);
    if (this->_bottomPipeSprite) al_destroy_bitmap(this->_bottomPipeSprite);
    if (this->_groundSprite) al_destroy_bitmap(this->_groundSprite);
    // if (backgroundSprite) al_destroy_bitmap(backgroundSprite);
    // if (gameFont) al_destroy_font(gameFont);

    // Clear vector after destroying bitmaps
    // birdAnimationFrames.clear();
}

void Game::resetGame() {
    // Reset bird position and velocity
    // bird->setPosition(50, screenHeight / 2 - 20);
    // bird->setVelocityY(0.0f);

    // Clear and reset pipes
    this->_ObstacleManager = 
        std::make_unique<ObstacleManager>(
            this->_BASE_PIPE_SPAWN_INTERVAL, // spawnInterval
            this->_BASE_PIPE_SCROLL_SPEED, // speed
            this->_MIN_PIPE_GAP, // minGapY 
            this->_MAX_PIPE_GAP, // maxGapY
            this->_PIPE_WIDTH, // pipeWidth
            this->_screenWidth,
            this->_screenHeight,
            this->_topPipeSprite,
            this->_bottomPipeSprite
        );

    // Reset ground positions (if they moved off-screen)
    // ground1->setPosition(0, groundYPosition);
    // ground2->setPosition(screenWidth, groundYPosition);

    // scoreManager->resetScore();
    _lastFrameTime = al_get_time(); // Reset timer to prevent large deltaTime on restart
}

void Game::applyDifficultyScalar() {
    this->_ObstacleManager->setScrollSpeed(_BASE_PIPE_SCROLL_SPEED * _difficulty_scalar);
    this->_ObstacleManager->setSpawnInterval(_BASE_PIPE_SPAWN_INTERVAL / _difficulty_scalar); // Spawn faster
    this->_ground1->setScrollSpeed(_BASE_PIPE_SCROLL_SPEED * _difficulty_scalar); // UNCOMMENT THIS
    this->_ground2->setScrollSpeed(_BASE_PIPE_SCROLL_SPEED * _difficulty_scalar); // UNCOMMENT THIS

}

// Increases the difficulty scalar and reapplies it
void Game::increaseDifficulty(float increment) {
    _difficulty_scalar += increment;
    // Optional: Clamp difficulty_scalar to a maximum value
    // if (_difficulty_scalar > 3.0f) _difficulty_scalar = 3.0f; // Example max

    std::cout << "Difficulty increased! New scalar: " << _difficulty_scalar << std::endl;
    applyDifficultyScalar(); // Reapply the new scalar to all objects
}