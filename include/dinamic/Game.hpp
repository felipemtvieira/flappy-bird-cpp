#ifndef GAME_HPP
#define GAME_HPP

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include "AbstractManager.hpp" // Inherit from AbstractManager
#include "Bird.hpp"
#include "ObstacleManager.hpp"
#include "Ground.hpp"
#include "Background.hpp"
#include "CollisionManager.hpp"
// #include "ScoreManager.hpp"

#include <vector>
#include <memory> // For std::unique_ptr

enum GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

class Game : public AbstractManager {
private:
    GameState currentState;
    ALLEGRO_DISPLAY* display;
    ALLEGRO_EVENT_QUEUE* eventQueue;
    ALLEGRO_TIMER* timer;

    std::unique_ptr<ObstacleManager> _ObstacleManager;
    std::unique_ptr<Bird> _Bird;
    std::unique_ptr<Ground> _ground1; // Two ground segments for continuous scroll
    std::unique_ptr<Background> _Background;
    CollisionManager _CollisionManager; // No unique_ptr, as it's simple and doesn't own resources
    // std::unique_ptr<ScoreManager> scoreManager; // Managed by Game

    // ALLEGRO_FONT* gameFont;
    ALLEGRO_BITMAP* _topPipeSprite;
    ALLEGRO_BITMAP* _bottomPipeSprite;
    ALLEGRO_BITMAP* _groundSprite;
    ALLEGRO_BITMAP* _backgroundSprite;
    ALLEGRO_BITMAP* _birdSprite1;
    ALLEGRO_BITMAP* _birdSprite2; 
    std::vector<ALLEGRO_BITMAP*> _birdAnimationFrames;

    float _groundYPosition; // Y coordinate where the ground starts
    float _difficulty_scalar; // Multiplicador de dificuldade para acelerar os elementos visuais proporcionalmente
    double _lastFrameTime; // For calculating deltaTime

    int _last_difficulty_score_threshold; // To prevent rapid difficulty increase on same score

    // Base values for game properties
    static constexpr float _BASE_PIPE_SCROLL_SPEED = 150.0f;
    // const float _BASE_GROUND_SCROLL_SPEED = 150.0f;
    static constexpr float _BASE_PIPE_SPAWN_INTERVAL = 2.0f; // Seconds
    static constexpr float _PIPE_WIDTH = 120.0f; // Keep this consistent
    static constexpr float _PIPE_HEIGHT = 575.0f; // Keep this consistent
    static constexpr float _MAX_PIPE_GAP = 150.0f;
    static constexpr float _MIN_PIPE_GAP = 150.0f;

    // NEW BIRD-RELATED CONSTANTS
    static constexpr float _BASE_GRAVITY = 900.0f;
    static constexpr float _BASE_JUMP_FORCE = 300.0f;
    static constexpr float _BIRD_WIDTH = 60.0f; // Logical width
    static constexpr float _BIRD_HEIGHT = 50.0f; // Logical height
    static constexpr float _INITIAL_BIRD_FLIGHT_DURATION = 10.0f;


public:
    Game(float width, float height);
    ~Game();

    bool initialize();
    void run();

private: // Helper methods, typically private
    void handleInput(ALLEGRO_EVENT& event);
    void update(double deltaTime);
    void draw();
    void resetGame();
    bool loadAssets(); // Helper to load all bitmaps and fonts
    void destroyAssets(); // Helper to destroy all loaded assets

    // Helper methods for different game states
    void updatePlaying(double deltaTime);
    void drawPlaying();
    // void drawMenu();
    // void drawGameOver();
    void applyDifficultyScalar(); // Applies the current difficulty_scalar to game objects
    void increaseDifficulty(float increment);
};

#endif // GAME_HPP