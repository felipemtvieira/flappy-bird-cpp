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
    : Element(x, y, width, height, screenWidth, screenHeight, baseScrollSpeed, bitmap) {
}

Ground::~Ground() {}

void Ground::draw() {
    if (this->_bitmap) {
        al_draw_scaled_bitmap(
            this->_bitmap,                    // The bitmap to draw
            0, 0,                       // Source X, Y (entire original bitmap)
            this->_width,
            this->_height,
            this->_x, this->_y,                     // Destination X, Y (from Element's position)
            this->_screenWidth,               // Destination width
            this->_screenHeight - this->_y,         // Altura final deve ser 20% da tela para não distorcer a imagem
            0
        );

        al_draw_scaled_bitmap(
            this->_bitmap,
            0, 0,
            this->_width,
            this->_height,
            this->_x + this->_screenWidth - 2.0, this->_y,     // Draw second instance right after the first
            this->_screenWidth,               // Destination width
            this->_screenHeight - this->_y,
            0
        );

    } else {
        // Fallback: draw a colored rectangle if no bitmap is loaded
        al_draw_filled_rectangle(this->_x, this->_y, this->_x + this->_width, this->_y + this->_height, al_map_rgb(100, 50, 0)); // Brown ground
    }
}

void Ground::update(double deltaTime) {
    this->_x -= this->_speed * deltaTime; // Use _x and _speed inherited from Element

    if (this->_x + this->_screenWidth <= 0) {
        this->_x = 0;
    }
}

// Set the inherited _speed member
void Ground::setScrollSpeed(float newSpeed) {
    this->_speed = newSpeed; // Update the inherited speed
}