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
        _jumpForce(baseJumpForce), // Store base values for gravity and jumpForce
        _animationFrames(animationFrames), 
        _currentFrame(0), 
        _animationTimer(0.0), 
        _frameDuration(0.1),
        _gracePeriodTimer(0.0f),
        _initialFlightDuration(initialFlightDuration)  
        {
            float collider_visual_percent_width = 0.8f; // e.g., 80% of visual width
            float collider_visual_percent_height = 0.8f; // e.g., 80% of visual height

            float actual_collider_width = width * collider_visual_percent_width;
            float actual_collider_height = height * collider_visual_percent_height;

            float offset_x = (width - actual_collider_width) / 2.0f; // Center horizontally
            float offset_y = (height - actual_collider_height) / 2.0f; // Center vertically

            setColliderDimensions(offset_x, offset_y, actual_collider_width, actual_collider_height);
        }

Bird::~Bird() {}

void Bird::draw() {
    // Use _bitmap from Element (which should be updated by animation logic)
    if (_bitmap) {
        float original_bitmap_width = (float)al_get_bitmap_width(_bitmap);
        float original_bitmap_height = (float)al_get_bitmap_height(_bitmap);

        al_draw_scaled_bitmap(
            _bitmap,                 // The current animation frame bitmap
            0, 0,                   // Source X, Y (entire original bitmap)
            original_bitmap_width,
            original_bitmap_height,
            _x, _y,                 // Destination X, Y (from Element's _x, _y)
            _width, _height,        // Destination width, height (from Element's _width, _height)
            0
        );
    } else {
        // Fallback: draw a yellow rectangle if no bitmap is loaded or animation frames are empty
        al_draw_filled_rectangle(_x, _y, _x + _width, _y + _height, al_map_rgb(255, 255, 0)); // Yellow bird
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
    if (_gracePeriodTimer < _initialFlightDuration) {
        _gracePeriodTimer += deltaTime;
        // Optionally, make it drift slightly up or down during grace period if not perfectly straight
        // For truly "straight", do nothing with _speed or _y here.
        // If you want it to move *slightly* forward or have a tiny vertical drift, you can add it here.
        // For now, it means _speed remains 0.0 unless jump() is called.
    } else {
        // Grace period over, apply normal physics
        _speed += _gravity * deltaTime; // Apply gravity to _speed
    }
    _y += _speed * deltaTime; // Update Element's _y position

    // Simple boundary check (e.g., prevent bird from going too high or falling too low)
    if (_y < 0) {
        _y = 0;
        _speed = 0;
    }
    // Bird should fall to the ground, so no upper bound on Y here related to groundYPosition in Game.
    // Collision with ground is handled by CollisionManager in Game.

    // Animation update
    _animationTimer += deltaTime;
    if (_animationTimer >= _frameDuration) {
        _currentFrame = (_currentFrame + 1) % _animationFrames.size();
        _bitmap = _animationFrames[_currentFrame]; // Update Element's _bitmap for current frame
        _animationTimer -= _frameDuration;
    }
}

void Bird::jump() {
    _speed = -_jumpForce; // <-- Apply jump force to _speed (velocity)
    _gracePeriodTimer = _initialFlightDuration;
}

// Setters remain the same, as they already manipulate _gravity and _jumpForce.
void Bird::setGravity(float newGravity) {
    _gravity = newGravity;
}

void Bird::setJumpForce(float newJumpForce) {
    _jumpForce = newJumpForce;
}