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

    #ifdef DEBUG_BUILD // <--- NEW: Only compile this block if DEBUG_BUILD is defined
    if (_debugDraw) { // Still use the _debugDraw flag to allow runtime control
        al_draw_rectangle(getColliderX(), getColliderY(),
                          getColliderX() + getColliderWidth(), getColliderY() + getColliderHeight(),
                          al_map_rgb(255, 0, 255), 2); // Magenta outline for collider
    }
    #endif // DEBUG_BUILD
}

void Bird::update(double deltaTime) {
    if (this->_gracePeriodTimer < this->_initialFlightDuration) {
        this->_gracePeriodTimer += deltaTime;
        // Optionally, make it drift slightly up or down during grace period if not perfectly straight
        // For truly "straight", do nothing with _speed or _y here.
        // If you want it to move *slightly* forward or have a tiny vertical drift, you can add it here.
        // For now, it means _speed remains 0.0 unless jump() is called.
    } else {
        // Grace period over, apply normal physics
        this->_speed += this->_gravity * deltaTime; // Apply gravity to _speed
    }
    this->_y += this->_speed * deltaTime; // Update Element's _y position

    // Simple boundary check (e.g., prevent bird from going too high or falling too low)
    if (this->_y < 0) {
        this->_y = 0;
        this->_speed = 0;
    }
    // Bird should fall to the ground, so no upper bound on Y here related to groundYPosition in Game.
    // Collision with ground is handled by CollisionManager in Game.

    // Animation update
    this->_animationTimer += deltaTime;
    if (this->_animationTimer >= this->_frameDuration) {
        this->_currentFrame = (this->_currentFrame + 1) % this->_animationFrames.size();
        this->_bitmap = _animationFrames[this->_currentFrame]; // Update Element's _bitmap for current frame
        this->_animationTimer -= this->_frameDuration;
    }
}

void Bird::jump() {
    this->_speed = -this->_jumpForce; // <-- Apply jump force to _speed (velocity)
    this->_gracePeriodTimer = this->_initialFlightDuration;
}

// Setters remain the same, as they already manipulate _gravity and _jumpForce.
void Bird::setGravity(float newGravity) {
    this->_gravity = newGravity;
}

void Bird::setJumpForce(float newJumpForce) {
    this->_jumpForce = newJumpForce;
}

float Bird::getVelocityY() { return this->_speed; }
void Bird::setVelocityY(float vel) { this->_speed = vel; }