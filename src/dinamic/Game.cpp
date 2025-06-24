#include "../../include/dinamic/Game.hpp"
#include "../../include/dinamic/GameError.hpp"
#include <iostream>

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
      _topPipeSprite(nullptr), 
      _bottomPipeSprite(nullptr),
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
        throw AllegroInitError("Failed to initialize Allegro core."); 
    }
    if (!al_init_image_addon()) {
        throw AllegroInitError("Failed to initialize Allegro image addon."); 
    }
    if (!al_install_keyboard()) {
        throw AllegroInitError("Failed to install keyboard."); 
    }
    if (!al_init_font_addon()) {
        throw AllegroInitError("Failed to initialize font addon."); 
    }
    if (!al_init_ttf_addon()) {
        throw AllegroInitError("Failed to initialize ttf addon."); 
    }
    if (!al_init_primitives_addon()) {
        throw AllegroInitError("Failed to initialize primitives addon."); 
    }

    display = al_create_display(this->_screenWidth, _screenHeight);
    if (!display) {
        throw DisplayCreationError("Failed to create display."); 
    }

    timer = al_create_timer(1.0 / 60.0); // 60 FPS
    if (!timer) {
        al_destroy_display(display);
        throw TimerCreationError("Failed to create timer.");
    }

    eventQueue = al_create_event_queue();
    if (!eventQueue) {
        al_destroy_timer(timer);   
        al_destroy_display(display);
        throw EventQueueCreationError("Failed to create event queue.");
    }

    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_register_event_source(eventQueue, al_get_keyboard_event_source());

    if (!loadAssets()) {
        throw AssetLoadError("Unknown error during asset loading (loadAssets returned false without throwing).");
    }

    this->createGameElements();

    this->applyDifficultyScalar();
    al_start_timer(timer);
    _lastFrameTime = al_get_time(); 

    #ifdef DEBUG_BUILD 
    _Bird->setDebugDraw(true);
    #endif

    return true;
}

void Game::createGameElements(){
    this->_ObstacleManager = 
        std::make_unique<ObstacleManager>(
            this->_BASE_PIPE_SPAWN_INTERVAL, 
            this->_BASE_PIPE_SCROLL_SPEED, 
            this->_MIN_PIPE_GAP, 
            this->_MAX_PIPE_GAP, 
            this->_PIPE_WIDTH, 
            this->_PIPE_HEIGHT, 
            this->_screenWidth,
            this->_screenHeight,
            this->_topPipeSprite,
            this->_bottomPipeSprite
        );

    float backgroundY = -1 * (this->_screenHeight - _groundYPosition) + 5.0; //Cálculo necessário para o bitmap do fundo "encontrar" com o bitmap do chão corretamente

    this->_Background = 
        std::make_unique<Background>(
            0,              // _x
            backgroundY,    // _y
            this->_screenWidth,
            this->_screenHeight,
            this->_screenWidth,
            this->_screenHeight,
            50.0f,
            this->_backgroundSprite
        );

    float original_bitmap_width = (float)al_get_bitmap_width(_groundSprite);
    float original_bitmap_height = (float)al_get_bitmap_height(_groundSprite);


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
        if (currentState == MENU) { 
            currentState = PLAYING;
            resetGame();
        } else if (currentState == PLAYING) {
            _Bird->jump(); 
        } else if (currentState == GAME_OVER) { 
            currentState = MENU; 
            resetGame(); 
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
    this->_ground1->update(deltaTime);
    this->_ObstacleManager->update(deltaTime);
    this->_Bird->update(deltaTime);

    if (_CollisionManager.checkCollision(*_Bird, *_ObstacleManager, _groundYPosition)) {
        currentState = GAME_OVER; 
        std::cout << "Game Over! Collision detected." << std::endl;        
    }

    for (const auto& pipe : _ObstacleManager->getPipes()) {
        if (pipe && !pipe->getIsTopPipe() && !pipe->getHasBeenScored()) { 
            if (_Bird->getX() > pipe->getX() + pipe->getWidth()) {
                _currentScore++; 
                pipe->setScored(true); 
                std::cout << "Score: " << _currentScore << std::endl;
                if (_currentScore > 0 && _currentScore % 3 == 0) { 
                    static int last_difficulty_score_threshold = 0; 
                    if (_currentScore > last_difficulty_score_threshold) {
                        increaseDifficulty(0.1f);
                        last_difficulty_score_threshold = _currentScore;
                    }
                }
            }
        }
    }

    #ifdef DEBUG_BUILD 
    for (const auto& pipe : _ObstacleManager->getPipes()) {
        if (pipe) {
            pipe->setDebugDraw(true);
        }
    }
    #endif 
}

void Game::draw() {
    al_clear_to_color(al_map_rgb(0, 0, 0)); // Limpa a tela

    switch (currentState) {
        case MENU:
            drawMenu();
            break;
        case PLAYING:
            drawPlaying();
            break;
        case GAME_OVER:
            drawGameOver();
            break;
    }

    al_flip_display();
}

void Game::drawPlaying() {
    this->_Background->draw();
    this->_ObstacleManager->draw();
    this->_ground1->draw();
    this->_Bird->draw();
    
    if (_gameFont) {
        std::string scoreText = "Score: " + std::to_string(_currentScore);        
        float textWidth = al_get_text_width(_gameFont, scoreText.c_str());
        al_draw_text(_gameFont, al_map_rgb(255, 255, 255), 
                     (_screenWidth / 2) - (textWidth / 2), 20, 
                     ALLEGRO_ALIGN_LEFT, scoreText.c_str());
    }
}

void Game::drawMenu() {
    this->_Background->draw(); 
    this->_ground1->draw(); 

    if (_gameFont) {
        ALLEGRO_COLOR textColor = al_map_rgb(255, 255, 255); 
        std::string title = "FLAPPY BIRD CLONE";
        float titleWidth = al_get_text_width(_gameFont, title.c_str());
        al_draw_text(_gameFont, textColor,
                     (_screenWidth / 2) - (titleWidth / 2), _screenHeight * 0.10,
                     ALLEGRO_ALIGN_LEFT, title.c_str());
        
        std::string instruction = "Press SPACE to Play";
        float instructionWidth = al_get_text_width(_gameFont, instruction.c_str());
        al_draw_text(_gameFont, textColor,
                     (_screenWidth / 2) - (instructionWidth / 2), _screenHeight * 0.25,
                     ALLEGRO_ALIGN_LEFT, instruction.c_str());
    }
}

void Game::drawGameOver() {
    this->_Background->draw(); 
    this->_ObstacleManager->draw(); 
    this->_ground1->draw(); 
    this->_Bird->draw(); 

    if (_gameFont) {
        ALLEGRO_COLOR textColor = al_map_rgb(255, 0, 0); 
        std::string gameOverText = "GAME OVER!";
        float gameOverWidth = al_get_text_width(_gameFont, gameOverText.c_str());
        al_draw_text(_gameFont, textColor,
                     (_screenWidth / 2) - (gameOverWidth / 2), _screenHeight * 0.10,
                     ALLEGRO_ALIGN_LEFT, gameOverText.c_str());
        
        ALLEGRO_COLOR scoreColor = al_map_rgb(255, 255, 255); 
        std::string finalScoreText = "Score: " + std::to_string(_currentScore);
        float finalScoreWidth = al_get_text_width(_gameFont, finalScoreText.c_str());
        al_draw_text(_gameFont, scoreColor,
                     (_screenWidth / 2) - (finalScoreWidth / 2), _screenHeight * 0.25,
                     ALLEGRO_ALIGN_LEFT, finalScoreText.c_str());
        
        std::string restartInstruction = "Press SPACE to restart";
        float restartWidth = al_get_text_width(_gameFont, restartInstruction.c_str());
        al_draw_text(_gameFont, scoreColor, 
                     (_screenWidth / 2) - (restartWidth / 2), _screenHeight * 0.9,
                     ALLEGRO_ALIGN_LEFT, restartInstruction.c_str());
    }
}

bool Game::loadAssets() {
    // Load Pipe Sprites
    _topPipeSprite = al_load_bitmap("assets/top_pipe.png");
    _bottomPipeSprite = al_load_bitmap("assets/bottom_pipe.png");
    if (!_topPipeSprite || !_bottomPipeSprite) {
        throw AssetLoadError("Failed to load pipe sprites (top_pipe.png or bottom_pipe.png). Check 'assets/' directory."); 
    }

    // Load Ground Sprite
    this->_groundSprite = al_load_bitmap("assets/ground.png");
    if (!this->_groundSprite) {
        throw AssetLoadError("Failed to load ground sprite (ground.png). Check 'assets/' directory."); 
    }


    // Load Background Sprite
    this->_backgroundSprite = al_load_bitmap("assets/background.png");
    if (!this->_backgroundSprite) {
        throw AssetLoadError("Failed to load background sprite (background.png). Check 'assets/' directory."); 
    }

    this->_gameFont = al_load_font("assets/PixelifySansFont.ttf", 36, 0); // Load font, size 36
    if (!_gameFont) {
        throw AssetLoadError("Failed to load font (arial.ttf). Check 'assets/' directory.");
    }

    // Load Bird Sprites
    this->_birdSprite1 = al_load_bitmap("assets/bird_1.png"); 
    this->_birdSprite2 = al_load_bitmap("assets/bird_1.png"); //Para futura implementação da animação
    if (!this->_birdSprite1 || !this->_birdSprite2) {
        throw AssetLoadError("Failed to load bird sprites (bird_1.png or bird_2.png). Check 'assets/' directory."); 
    } else {
        this->_birdAnimationFrames.push_back(this->_birdSprite1);
        this->_birdAnimationFrames.push_back(this->_birdSprite2);
    }


    return true;
}

void Game::destroyAssets() {
    if (this->_birdSprite1) al_destroy_bitmap(this->_birdSprite1);
    if (this->_birdSprite2) al_destroy_bitmap(this->_birdSprite2);
    
    if (this->_topPipeSprite) al_destroy_bitmap(this->_topPipeSprite);
    if (this->_bottomPipeSprite) al_destroy_bitmap(this->_bottomPipeSprite);
    if (this->_groundSprite) al_destroy_bitmap(this->_groundSprite);
    if (this->_backgroundSprite) al_destroy_bitmap(this->_backgroundSprite);
    if (this->_gameFont) { 
        al_destroy_font(this->_gameFont);
    }
}

void Game::resetGame() {
    this->_difficulty_scalar = 1.0f;
    this->_currentScore = 0;
    
    this->createGameElements(); 

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


void Game::increaseDifficulty(float increment) {
    _difficulty_scalar += increment;    

    std::cout << "Difficulty increased! New scalar: " << _difficulty_scalar << std::endl;
    applyDifficultyScalar(); 
}

int Game::getCurrentScore(){ return this->_currentScore; };