#ifndef GAME_HPP
#define GAME_HPP

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include "AbstractManager.hpp" 
#include "Bird.hpp"
#include "ObstacleManager.hpp"
#include "Ground.hpp"
#include "Background.hpp"
#include "CollisionManager.hpp"

#include <vector>
#include <memory>

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
    std::unique_ptr<Ground> _ground1;
    std::unique_ptr<Background> _Background;
    CollisionManager _CollisionManager; // Sem unique_ptr, pois é simples e não possui recursos

    // Sprites 
    ALLEGRO_BITMAP* _topPipeSprite;
    ALLEGRO_BITMAP* _bottomPipeSprite;
    ALLEGRO_BITMAP* _groundSprite;
    ALLEGRO_BITMAP* _backgroundSprite;
    ALLEGRO_BITMAP* _birdSprite1;
    ALLEGRO_BITMAP* _birdSprite2; 
    std::vector<ALLEGRO_BITMAP*> _birdAnimationFrames;

    ALLEGRO_FONT* _gameFont;

    float _groundYPosition; // Coordenada Y onde o solo começa
    float _difficulty_scalar; // Multiplicador de dificuldade para acelerar os elementos visuais proporcionalmente
    double _lastFrameTime; // Para calcular deltaTime
    int _currentScore; 
    
    // Constantes gerais do jogo
    static constexpr float _BASE_PIPE_SCROLL_SPEED = 150.0f;
    // const float _BASE_GROUND_SCROLL_SPEED = 150.0f; // Pode ser utilizado valor diferente para criar efeito de visual de paralaxe (ainda não implementado)
    static constexpr float _BASE_PIPE_SPAWN_INTERVAL = 2.0f; 
    static constexpr float _PIPE_WIDTH = 120.0f; 
    static constexpr float _PIPE_HEIGHT = 575.0f; 
    static constexpr float _MAX_PIPE_GAP = 150.0f;
    static constexpr float _MIN_PIPE_GAP = 150.0f;

    // Constantes relacionadas ao pássado
    static constexpr float _BASE_GRAVITY = 900.0f;
    static constexpr float _BASE_JUMP_FORCE = 300.0f;
    static constexpr float _BIRD_WIDTH = 60.0f; // Tamanho do retângulo de colisão (um pouco menor do que o aspecto visual do pássaro)
    static constexpr float _BIRD_HEIGHT = 50.0f; // Tamanho do retângulo de colisão
    static constexpr float _INITIAL_BIRD_FLIGHT_DURATION = 3.0f;


public:
    Game(float width, float height);
    ~Game();

    bool initialize();
    void run();

private: 
    void handleInput(ALLEGRO_EVENT& event);
    void update(double deltaTime);
    void draw();
    void resetGame();
    bool loadAssets(); 
    void destroyAssets(); 

    
    void updatePlaying(double deltaTime);
    void drawPlaying();
    void drawMenu();
    void drawGameOver();
    void applyDifficultyScalar(); 
    void increaseDifficulty(float increment);
    void createGameElements(); 

public:
    int getCurrentScore();
};

#endif