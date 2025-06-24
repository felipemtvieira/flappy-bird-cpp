#ifndef BIRD_HPP
#define BIRD_HPP

#include "Element.hpp"
#include <vector>

class Bird : public Element {
private:
    float _gravity;
    float _jumpForce;
    std::vector<ALLEGRO_BITMAP*> _animationFrames;
    int _currentFrame;
    double _animationTimer;
    double _frameDuration;

    float _gracePeriodTimer; // Período de tempo em que o pássaro voa em linha reta antes de começar a cair
    float _initialFlightDuration;//ATENCAO

public:
    Bird(
        float x, 
        float y, 
        float width, 
        float height, 
        float baseGravity, 
        float baseJumpForce,
        const std::vector<ALLEGRO_BITMAP*>& animationFrames, // Animação do pássaro ainda não implementada
        float screenWidth, 
        float screenHeight, 
        float initialFlightDuration
    );
    ~Bird();

    void draw() override;
    void update(double deltaTime) override;
    void jump();

    float getVelocityY();
    void setVelocityY(float vel);

    void setGravity(float newGravity);
    void setJumpForce(float newJumpForce);
};

#endif