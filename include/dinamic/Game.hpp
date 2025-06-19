#ifndef GAME_HPP
#define GAME_HPP

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

// #include "Bird.hpp"
#include "ObstacleManager.hpp"
// #include "Ground.hpp"
// #include "Background.hpp"
// #include "CollisionManager.hpp"
// #include "ScoreManager.hpp"

#include <vector>
#include <memory> // For std::unique_ptr

enum GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

class Game {
private:
    GameState currentState;
    ALLEGRO_DISPLAY* display;
    ALLEGRO_EVENT_QUEUE* eventQueue;
    ALLEGRO_TIMER* timer;

    std::unique_ptr<ObstacleManager> _ObstacleManager;
    // std::unique_ptr<Bird> bird;
    // std::unique_ptr<Ground> ground1, ground2; // Two ground segments for continuous scroll
    // std::unique_ptr<Background> background;
    // CollisionManager collisionManager; // No unique_ptr, as it's simple and doesn't own resources
    // std::unique_ptr<ScoreManager> scoreManager; // Managed by Game

    // ALLEGRO_FONT* gameFont;
    ALLEGRO_BITMAP* _topPipeSprite;
    ALLEGRO_BITMAP* _bottomPipeSprite;

    //  birdSprite1, *birdSprite2, *groundSprite, *backgroundSprite;
    // std::vector<ALLEGRO_BITMAP*> birdAnimationFrames;

    float _screenWidth;
    float _screenHeight;
    float groundYPosition; // Y coordinate where the ground starts
    double lastFrameTime; // For calculating deltaTime

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
    void drawMenu();
    void drawGameOver();
};

#endif // GAME_HPP