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
      _birdSprite1(nullptr), 
      _birdSprite2(nullptr),
    //   _birdSprite3(nullptr),
      _ground1(nullptr),
      _topPipeSprite(nullptr), 
      _bottomPipeSprite(nullptr),
      _groundSprite(nullptr), 
      _backgroundSprite(nullptr),
      _difficulty_scalar(1.0f),
      _lastFrameTime(0.0),
      _Bird(nullptr)
      {
        _groundYPosition = SCREEN_HEIGHT * 0.8f; // O chão deve ocupar 20% da tela
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
            this->_PIPE_HEIGHT, // pipeWidth
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

    _Bird = std::make_unique<Bird>(
        50,                             // x position
        _screenHeight / 2 - (_BIRD_HEIGHT / 2), // y position (roughly center vertically)
        _BIRD_WIDTH,                    // width
        _BIRD_HEIGHT,                   // height
        _BASE_GRAVITY,                  // base gravity
        _BASE_JUMP_FORCE,               // base jump force
        _birdAnimationFrames,           // vector of animation bitmaps
        this->_screenWidth, this->_screenHeight // Pass screen dimensions to Element
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
            handleInput(event);
        } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }
    }
}

void Game::handleInput(ALLEGRO_EVENT& event) {
    if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
        if (currentState == MENU) { // If you re-introduce a menu
            currentState = PLAYING;
            resetGame();
        } else if (currentState == PLAYING) {
            _Bird->jump(); // Make bird jump
        } else if (currentState == GAME_OVER) { // If you re-introduce game over
            currentState = PLAYING; // Or MENU
            resetGame();
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
    this->_ObstacleManager->update(deltaTime);
    this->_Bird->update(deltaTime);

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
    this->_Bird->draw();
    // scoreManager->draw();
}

bool Game::loadAssets() {
    // Load Pipe Sprites
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
    
// ALLEGRO_BITMAP* frame1_sub = al_create_sub_bitmap(master_sheet, x_offset_frame1, y_offset_frame1, 32, 24);
// ALLEGRO_BITMAP* frame2_sub = al_create_sub_bitmap(master_sheet, x_offset_frame2, y_offset_frame2, 32, 24);

    // NEW: Load Bird Sprites
    _birdSprite1 = al_load_bitmap("assets/bird_1.png"); // Assuming you have these
    _birdSprite2 = al_load_bitmap("assets/bird_1.png");
    if (!_birdSprite1 || !_birdSprite2) {
        std::cerr << "Failed to load bird sprites! Using fallback rectangle." << std::endl;
        _birdSprite1 = nullptr; _birdSprite2 = nullptr;
    } else {
        _birdAnimationFrames.push_back(_birdSprite1);
        _birdAnimationFrames.push_back(_birdSprite2);
    }

    if (!this->_topPipeSprite || !this->_bottomPipeSprite || !this->_backgroundSprite || !this->_groundSprite) { // Added _groundSprite check
        std::cerr << "One or more essential assets failed to load. Returning false." << std::endl;
        return false;
    }

    return true;
}

void Game::destroyAssets() {
    if (this->_birdSprite1) al_destroy_bitmap(this->_birdSprite1);
    if (this->_birdSprite2) al_destroy_bitmap(this->_birdSprite2);
    // if (this->_birdSprite3) al_destroy_bitmap(this->_birdSprite3);
    if (this->_topPipeSprite) al_destroy_bitmap(this->_topPipeSprite);
    if (this->_bottomPipeSprite) al_destroy_bitmap(this->_bottomPipeSprite);
    if (this->_groundSprite) al_destroy_bitmap(this->_groundSprite);
    if (this->_backgroundSprite) al_destroy_bitmap(this->_backgroundSprite);
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
            this->_PIPE_HEIGHT, // pipeWidth
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
    this->_ObstacleManager->setSpawnInterval(_BASE_PIPE_SPAWN_INTERVAL / _difficulty_scalar);
    this->_ground1->setScrollSpeed(_BASE_PIPE_SCROLL_SPEED * _difficulty_scalar);
    this->_Bird->setGravity(_BASE_GRAVITY * _difficulty_scalar);
    this->_Bird->setJumpForce(_BASE_JUMP_FORCE * _difficulty_scalar);

}

// Increases the difficulty scalar and reapplies it
void Game::increaseDifficulty(float increment) {
    _difficulty_scalar += increment;
    // Optional: Clamp difficulty_scalar to a maximum value
    // if (_difficulty_scalar > 3.0f) _difficulty_scalar = 3.0f; // Example max

    std::cout << "Difficulty increased! New scalar: " << _difficulty_scalar << std::endl;
    applyDifficultyScalar(); // Reapply the new scalar to all objects
}