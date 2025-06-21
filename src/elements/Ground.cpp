#include "../../include/elements/Ground.hpp" // Correct path to your header
#include <allegro5/allegro_primitives.h> // For al_draw_filled_rectangle

// Constructor implementation
Ground::Ground(
        float x, 
        float y, 
        float width, 
        float height, 
        float screenWidth, 
        float screenHeight,     
        float baseScrollSpeed, 
        ALLEGRO_BITMAP* bitmap
    )
    : Element(x, y, width, height, screenWidth, screenHeight, baseScrollSpeed, bitmap),
      _originalX(x) {
}

Ground::~Ground() {}

void Ground::draw() {
    if (_bitmap) {
        al_draw_scaled_bitmap(
            _bitmap,                    // The bitmap to draw
            0, 0,                       // Source X, Y (entire original bitmap)
            _width,
            _height,
            _x, _y,                     // Destination X, Y (from Element's position)
            _screenWidth,               // Destination width
            _screenHeight - _y,         // Altura final deve ser 20% da tela para não distorcer a imagem
            0
        );

        al_draw_scaled_bitmap(
            _bitmap,
            0, 0,
            _width,
            _height,
            _x + _screenWidth - 2.0, _y,     // Draw second instance right after the first
            _screenWidth,               // Destination width
            _screenHeight - _y,
            0
        );

    } else {
        // Fallback: draw a colored rectangle if no bitmap is loaded
        al_draw_filled_rectangle(_x, _y, _x + _width, _y + _height, al_map_rgb(100, 50, 0)); // Brown ground
    }
}

void Ground::update(double deltaTime) {
    _x -= _speed * deltaTime; // Use _x and _speed inherited from Element

    if (_x + _screenWidth <= 0) {
        _x = 0;
    }
}

// Set the inherited _speed member
void Ground::setScrollSpeed(float newSpeed) {
    _speed = newSpeed; // Update the inherited speed
}