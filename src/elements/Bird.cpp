#include "../../include/elements/Bird.hpp"
#include <allegro5/allegro_primitives.h>

#include <iostream>

Bird::Bird(
        float x, 
        float y, 
        float width, 
        float height, 
        float baseGravity, 
        float baseJumpForce,
        const std::vector<ALLEGRO_BITMAP*>& animationFrames,
        float screenWidth, 
        float screenHeight,
        float initialFlightDuration
    ) : Element(
        x, 
        y, 
        width, 
        height, 
        screenWidth, 
        screenHeight, 
        0.0f, 
        animationFrames.empty() ? nullptr : animationFrames[0]),
        _gravity(baseGravity), 
        _jumpForce(baseJumpForce), 
        _animationFrames(animationFrames), 
        _currentFrame(0), 
        _animationTimer(0.0), 
        _frameDuration(0.1),
        _gracePeriodTimer(0.0f),
        _initialFlightDuration(initialFlightDuration)  
        {
            float collider_visual_percent_width = 0.8f; // e.g., 80% do tamanho original
            float collider_visual_percent_height = 0.8f; 

            float actual_collider_width = width * collider_visual_percent_width;
            float actual_collider_height = height * collider_visual_percent_height;

            float offset_x = (width - actual_collider_width) / 2.0f;    // Alinhamento do retângulo de colisão ao centro do pássaro
            float offset_y = (height - actual_collider_height) / 2.0f;

            setColliderDimensions(offset_x, offset_y, actual_collider_width, actual_collider_height);
        }

Bird::~Bird() {}

void Bird::draw() {
    if (_bitmap) {
        float original_bitmap_width = (float)al_get_bitmap_width(_bitmap);
        float original_bitmap_height = (float)al_get_bitmap_height(_bitmap);

        al_draw_scaled_bitmap(
            this->_bitmap,                 
            0, 0,                   
            original_bitmap_width,
            original_bitmap_height,
            this->_x, this->_y,                 
            this->_width, this->_height,        
            0
        );
    }

    #ifdef DEBUG_BUILD
    if (_debugDraw) {
        al_draw_rectangle(getColliderX(), getColliderY(),
                          getColliderX() + getColliderWidth(), getColliderY() + getColliderHeight(),
                          al_map_rgb(255, 0, 255), 2);
    }
    #endif
}

void Bird::update(double deltaTime) {
    if (this->_gracePeriodTimer < this->_initialFlightDuration) {
        this->_gracePeriodTimer += deltaTime;
    } else {
        this->_speed += this->_gravity * deltaTime; 
    }
    this->_y += this->_speed * deltaTime; 

    
    if (this->_y < 0) {
        this->_y = 0;
        this->_speed = 0;
    }
    
    this->_animationTimer += deltaTime;
    if (this->_animationTimer >= this->_frameDuration) {
        this->_currentFrame = (this->_currentFrame + 1) % this->_animationFrames.size();
        this->_bitmap = _animationFrames[this->_currentFrame]; 
        this->_animationTimer -= this->_frameDuration;
    }
}

void Bird::jump() {
    this->_speed = -this->_jumpForce; 
    this->_gracePeriodTimer = this->_initialFlightDuration;
}

void Bird::setGravity(float newGravity) {
    this->_gravity = newGravity;
}

void Bird::setJumpForce(float newJumpForce) {
    this->_jumpForce = newJumpForce;
}

float Bird::getVelocityY() { return this->_speed; }
void Bird::setVelocityY(float vel) { this->_speed = vel; }