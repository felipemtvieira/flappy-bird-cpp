#include "../../include/elements/Bird.hpp"
#include <allegro5/allegro_primitives.h>

Bird::Bird(
        float x, 
        float y, 
        float width, 
        float height, 
        float baseGravity, 
        float baseJumpForce,
        const std::vector<ALLEGRO_BITMAP*>& animationFrames,
        float screenWidth, 
        float screenHeight
    )   : Element(x, y, width, height, 0.0f, screenWidth, screenHeight, animationFrames.empty() ? nullptr : animationFrames[0]),
      _gravity(baseGravity), 
      _jumpForce(baseJumpForce), // Store base values for gravity and jumpForce
      _animationFrames(animationFrames), 
      _currentFrame(0), 
      _animationTimer(0.0), 
      _frameDuration(0.1) {
}

Bird::~Bird() {
    // ...
}

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
}

void Bird::update(double deltaTime) {
    // Apply gravity
    _speed += _gravity * deltaTime;
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
}

// Setters remain the same, as they already manipulate _gravity and _jumpForce.
void Bird::setGravity(float newGravity) {
    _gravity = newGravity;
}

void Bird::setJumpForce(float newJumpForce) {
    _jumpForce = newJumpForce;
}