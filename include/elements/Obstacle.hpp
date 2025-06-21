#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include "Element.hpp"

class Obstacle: public Element {
private:
    bool _isTopPipe;


public:
    Obstacle(
        float x, 
        float y, 
        float width, 
        float height, 
        float screenWidth, 
        float screenHeight, 
        float speed, 
        ALLEGRO_BITMAP* bitmap, 
        bool isTopPipe
    );
    
    ~Obstacle();
    
    void draw() override;
    void update(double deltaTime) override; // Move o cano para esquerda;
    bool isOffScreen() const; // Verifica se o cano saiu da tela à esquerda
    float getIsTopPipe() const;
    void setScrollSpeed(float newSpeed);

};

#endif