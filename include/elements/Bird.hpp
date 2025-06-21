#ifndef BIRD_HPP
#define BIRD_HPP

#include "Element.hpp"
#include <vector>

class Bird : public Element {
private:
    // float _velocityY; // <-- REMOVE THIS MEMBER
    float _gravity;
    float _jumpForce;
    std::vector<ALLEGRO_BITMAP*> _animationFrames;
    int _currentFrame;
    double _animationTimer;
    double _frameDuration;

public:
    Bird(
        float x, 
        float y, 
        float width, 
        float height, 
        float baseGravity, 
        float baseJumpForce,
        const std::vector<ALLEGRO_BITMAP*>& animationFrames,
        float screenWidth, 
        float screenHeight
    );
    ~Bird();

    void draw() override;
    void update(double deltaTime) override;
    void jump();

    // The getVelocityY() and setVelocityY() now operate on the inherited _speed
    float getVelocityY() const { return _speed; } // <-- RETURN _speed
    void setVelocityY(float vel) { _speed = vel; } // <-- SET _speed

    void setGravity(float newGravity);
    void setJumpForce(float newJumpForce);
};

#endif // BIRD_HPP