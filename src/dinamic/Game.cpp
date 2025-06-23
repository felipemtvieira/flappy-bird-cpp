#include "../../include/dinamic/Game.hpp"
#include "../../include/dinamic/GameError.hpp"
#include <iostream> // For debugging output

// Constructor
Game::Game(float SCREEN_WIDTH, float SCREEN_HEIGHT)
    : 
      AbstractManager(1, SCREEN_WIDTH, SCREEN_HEIGHT), 
      currentState(MENU),
      display(nullptr),
      eventQueue(nullptr),
      timer(nullptr),
      _ObstacleManager(nullptr),
      _Bird(nullptr),
      _ground1(nullptr),
      _Background(nullptr),
      _CollisionManager(1, SCREEN_WIDTH, SCREEN_HEIGHT),
    //   gameFont(nullptr),
      _topPipeSprite(nullptr), 
      _bottomPipeSprite(nullptr),
    //   _birdSprite3(nullptr),
      _groundSprite(nullptr), 
      _backgroundSprite(nullptr),
      _birdSprite1(nullptr), 
      _birdSprite2(nullptr),
      _gameFont(nullptr),
      _difficulty_scalar(1.0f),
      _lastFrameTime(0.0),
      _currentScore(0)
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
        throw AllegroInitError("Failed to initialize Allegro core."); // THROW EXCEPTION
    }
    if (!al_init_image_addon()) {
        throw AllegroInitError("Failed to initialize Allegro image addon."); // THROW EXCEPTION
    }
    if (!al_install_keyboard()) {
        throw AllegroInitError("Failed to install keyboard."); // THROW EXCEPTION
    }
    if (!al_init_font_addon()) {
        throw AllegroInitError("Failed to initialize font addon."); // THROW EXCEPTION
    }
    if (!al_init_ttf_addon()) {
        throw AllegroInitError("Failed to initialize ttf addon."); // THROW EXCEPTION
    }
    if (!al_init_primitives_addon()) {
        throw AllegroInitError("Failed to initialize primitives addon."); // THROW EXCEPTION
    }

    display = al_create_display(this->_screenWidth, _screenHeight);
    if (!display) {
        throw DisplayCreationError("Failed to create display."); // THROW EXCEPTION
    }

    timer = al_create_timer(1.0 / 60.0); // 60 FPS
    if (!timer) {
        al_destroy_display(display); // Cleanup before throwing
        throw TimerCreationError("Failed to create timer."); // THROW EXCEPTION
    }

    eventQueue = al_create_event_queue();
    if (!eventQueue) {
        al_destroy_timer(timer);    // Cleanup
        al_destroy_display(display); // Cleanup
        throw EventQueueCreationError("Failed to create event queue."); // THROW EXCEPTION
    }

    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_register_event_source(eventQueue, al_get_keyboard_event_source());

    if (!loadAssets()) {
        throw AssetLoadError("Unknown error during asset loading (loadAssets returned false without throwing).");
    }

    // Initialize game objects after assets are loaded
    this->createGameElements();
    
    // // ScoreManager
    // scoreManager = std::make_unique<ScoreManager>(gameFont);

    this->applyDifficultyScalar();
    al_start_timer(timer);
    _lastFrameTime = al_get_time(); // Initialize last frame time

    #ifdef DEBUG_BUILD // <--- NEW: Only set debugDraw if DEBUG_BUILD is defined
    _Bird->setDebugDraw(true);
    #endif

    return true;
}

void Game::createGameElements(){
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
        this->_screenWidth, this->_screenHeight, // Pass screen dimensions to Element
        _INITIAL_BIRD_FLIGHT_DURATION
    );

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
    if (event.keyboard.keycode == ALLEGRO_KEY_SPACE || event.keyboard.keycode == ALLEGRO_KEY_J) {
        if (currentState == MENU) { // If you re-introduce a menu
            std::cout << "[DEBUG] handleInput: Transitioning from MENU to PLAYING." << std::endl; // NEW
            currentState = PLAYING;
            resetGame();
        } else if (currentState == PLAYING) {
            _Bird->jump(); // Make bird jump
        } else if (currentState == GAME_OVER) { // If you re-introduce game over
            currentState = MENU; // Transition from GAME_OVER back to MENU
            resetGame(); // Reset game state and objects for next play
        }
    }
}

void Game::update(double deltaTime) {
    switch (currentState) {
        case MENU:
            break;
        case PLAYING:
            updatePlaying(deltaTime);
            break;
        case GAME_OVER:
            break;
    }
}

void Game::updatePlaying(double deltaTime) {
    // _Background->update(deltaTime);
    this->_ground1->update(deltaTime);
    this->_ObstacleManager->update(deltaTime);
    this->_Bird->update(deltaTime);

    // Check for collisions
    if (_CollisionManager.checkCollision(*_Bird, *_ObstacleManager, _groundYPosition)) {
        currentState = GAME_OVER; // Set game state to GAME_OVER on collision
        std::cout << "Game Over! Collision detected." << std::endl;
        // You might add sound effects, score display, etc. here
    }

    // NEW: Accurate Scoring Logic
    // Iterate through active pipes to check if the bird has passed them
    for (const auto& pipe : _ObstacleManager->getPipes()) {
        // Only consider the bottom pipe of a pair for scoring, and only if not already scored
        // Assuming pipes are added in pairs (top then bottom), so every second pipe is a bottom pipe for scoring.
        // Or, better, check _isTopPipe if you prefer.
        if (pipe && !pipe->getIsTopPipe() && !pipe->getHasBeenScored()) { // Ensure it's a bottom pipe and not scored
            // Check if the bird's X position has passed the right edge of the pipe
            // Bird's X + half its width is a good point to check for passing the pipe gap.
            if (_Bird->getX() > pipe->getX() + pipe->getWidth()) {
                _currentScore++; // Increment score
                pipe->setScored(true); // Mark this pipe as scored
                std::cout << "Score: " << _currentScore << std::endl;

                // Difficulty increase logic (now based on _currentScore)
                if (_currentScore > 0 && _currentScore % 3 == 0) { // Every 3 points, increase difficulty
                    static int last_difficulty_score_threshold = 0; // To prevent multiple increases for same score
                    if (_currentScore > last_difficulty_score_threshold) {
                        increaseDifficulty(0.1f);
                        last_difficulty_score_threshold = _currentScore;
                    }
                }
            }
        }
    }

    #ifdef DEBUG_BUILD // <--- NEW: Only set debugDraw if DEBUG_BUILD is defined
    for (const auto& pipe : _ObstacleManager->getPipes()) {
        if (pipe) {
            pipe->setDebugDraw(true);
        }
    }
    #endif // DEBUG_BUILD
}

void Game::draw() {
    al_clear_to_color(al_map_rgb(0, 0, 0)); // Clear screen to black

    switch (currentState) {
        case MENU:
            drawMenu();
            break;
        case PLAYING:
            drawPlaying();
            break;
        case GAME_OVER:
            drawPlaying(); // Draw game state one last time
            drawGameOver();
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
    if (_gameFont) {
        std::string scoreText = "Score: " + std::to_string(_currentScore);
        // Draw centered at top of screen
        float textWidth = al_get_text_width(_gameFont, scoreText.c_str());
        al_draw_text(_gameFont, al_map_rgb(255, 255, 255), // White text
                     (_screenWidth / 2) - (textWidth / 2), 20, // X: centered, Y: 20px from top
                     ALLEGRO_ALIGN_LEFT, scoreText.c_str());
    }
}

void Game::drawMenu() {
    this->_Background->draw(); // Draw background for consistency
    this->_ground1->draw(); // Draw ground

    if (_gameFont) {
        ALLEGRO_COLOR textColor = al_map_rgb(255, 255, 255); // White text

        // Game Title
        std::string title = "FLAPPY BIRD CLONE";
        float titleWidth = al_get_text_width(_gameFont, title.c_str());
        al_draw_text(_gameFont, textColor,
                     (_screenWidth / 2) - (titleWidth / 2), _screenHeight * 0.10,
                     ALLEGRO_ALIGN_LEFT, title.c_str());

        // "Press SPACE to Play" instruction
        std::string instruction = "Press SPACE to Play";
        float instructionWidth = al_get_text_width(_gameFont, instruction.c_str());
        al_draw_text(_gameFont, textColor,
                     (_screenWidth / 2) - (instructionWidth / 2), _screenHeight * 0.25,
                     ALLEGRO_ALIGN_LEFT, instruction.c_str());

        // You could add your bird sprite animating here if you wish, or a static title screen image
    }
}

void Game::drawGameOver() {
    this->_Background->draw(); 
    this->_ObstacleManager->draw(); 
    this->_ground1->draw(); 
    this->_Bird->draw(); 

    if (_gameFont) {
        ALLEGRO_COLOR textColor = al_map_rgb(255, 0, 0); // Red text for Game Over

        // "Game Over!" text
        std::string gameOverText = "GAME OVER!";
        float gameOverWidth = al_get_text_width(_gameFont, gameOverText.c_str());
        al_draw_text(_gameFont, textColor,
                     (_screenWidth / 2) - (gameOverWidth / 2), _screenHeight * 0.10,
                     ALLEGRO_ALIGN_LEFT, gameOverText.c_str());

        // Display final score
        ALLEGRO_COLOR scoreColor = al_map_rgb(255, 255, 255); // White text for score
        std::string finalScoreText = "Score: " + std::to_string(_currentScore);
        float finalScoreWidth = al_get_text_width(_gameFont, finalScoreText.c_str());
        al_draw_text(_gameFont, scoreColor,
                     (_screenWidth / 2) - (finalScoreWidth / 2), _screenHeight * 0.25,
                     ALLEGRO_ALIGN_LEFT, finalScoreText.c_str());

        // "Press SPACE to restart/return to menu" instruction
        std::string restartInstruction = "Press SPACE to restart";
        float restartWidth = al_get_text_width(_gameFont, restartInstruction.c_str());
        al_draw_text(_gameFont, scoreColor, // Re-using scoreColor for instruction
                     (_screenWidth / 2) - (restartWidth / 2), _screenHeight * 0.9,
                     ALLEGRO_ALIGN_LEFT, restartInstruction.c_str());
    }
}

bool Game::loadAssets() {
    // Load Pipe Sprites
    _topPipeSprite = al_load_bitmap("assets/top_pipe.png");
    _bottomPipeSprite = al_load_bitmap("assets/bottom_pipe.png");
    if (!_topPipeSprite || !_bottomPipeSprite) {
        throw AssetLoadError("Failed to load pipe sprites (top_pipe.png or bottom_pipe.png). Check 'assets/' directory."); // THROW EXCEPTION
    }

    // Load Ground Sprite
    this->_groundSprite = al_load_bitmap("assets/ground.png");
    if (!this->_groundSprite) {
        throw AssetLoadError("Failed to load ground sprite (ground.png). Check 'assets/' directory."); // THROW EXCEPTION
    }


    // Load Background Sprite
    this->_backgroundSprite = al_load_bitmap("assets/background.png");
    if (!this->_backgroundSprite) {
        throw AssetLoadError("Failed to load background sprite (background.png). Check 'assets/' directory."); // THROW EXCEPTION
    }

    this->_gameFont = al_load_font("assets/PixelifySansFont.ttf", 36, 0); // Load font, size 36
    if (!_gameFont) {
        throw AssetLoadError("Failed to load font (arial.ttf). Check 'assets/' directory.");
    }

    // NEW: Load Bird Sprites
    this->_birdSprite1 = al_load_bitmap("assets/bird_1.png"); // Assuming you have these
    this->_birdSprite2 = al_load_bitmap("assets/bird_1.png");
    if (!this->_birdSprite1 || !this->_birdSprite2) {
        throw AssetLoadError("Failed to load bird sprites (bird_1.png or bird_2.png). Check 'assets/' directory."); // THROW EXCEPTION
    } else {
        this->_birdAnimationFrames.push_back(this->_birdSprite1);
        this->_birdAnimationFrames.push_back(this->_birdSprite2);
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
    if (this->_gameFont) { // NEW: Destroy the font
        al_destroy_font(this->_gameFont);
    }

    // Clear vector after destroying bitmaps
    // birdAnimationFrames.clear();
}

void Game::resetGame() {
    this->_difficulty_scalar = 1.0f;
    this->_currentScore = 0;

    // CALL THE NEW HELPER FUNCTION HERE
    this->createGameElements(); // Replaces duplicated object recreation code

    this->applyDifficultyScalar();
    this->_lastFrameTime = al_get_time();
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

int Game::getCurrentScore(){ return this->_currentScore; };