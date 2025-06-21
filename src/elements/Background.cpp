#include "../../include/elements/Background.hpp"

#include <iostream>

Background::Background(float x, float y, float width, float height, float screenWidth, float screenHeight, float baseScrollSpeed, ALLEGRO_BITMAP* bitmap)
    : Element(x, y, width, height, screenWidth, screenHeight, baseScrollSpeed, bitmap)
    {}

Background::~Background() {
    // No specific cleanup needed here for the _sprite, as it's managed externally.
}

void Background::draw() {
    if (this->_bitmap) {
        // Draw the scaled bitmap to cover the screen
        // We typically want the background to fill the screen, so its destination width/height
        // should match its own dimensions (_width, _height), which should be screen dimensions.
        float original_sprite_width = (float)al_get_bitmap_width(this->_bitmap);
        float original_sprite_height = (float)al_get_bitmap_height(this->_bitmap);

        al_draw_scaled_bitmap(
            this->_bitmap,                 // The bitmap to draw
            0, 0,                   // Source X, Y (entire original bitmap)
            original_sprite_width,
            original_sprite_height,
            _x, _y,                 // Destination X, Y (from Element's position)
            _width, _height,        // Destination width, height (from Element's dimensions)
            0
        );

        // If background needs to tile/loop, you might draw a second instance
        // directly to the right of the first if it's scrolling past.
        // This is a common pattern for backgrounds that scroll continuously.
        if (_x + _width < this->_width) { // If the first background doesn't cover the whole screen, draw a second
            al_draw_scaled_bitmap(
                this->_bitmap,
                0, 0,
                original_sprite_width,
                original_sprite_height,
                _x + _width, _y, // Draw second instance right after the first
                _width, _height,
                0
            );
        }

    } else {
        // Fallback: clear to a sky blue color if no sprite
        al_clear_to_color(al_map_rgb(135, 206, 235));
    }
}

void Background::update(double deltaTime) {
    if (_scrollSpeed != 0) {
        _x -= _scrollSpeed * deltaTime; // Use _x and _scrollSpeed
        // Handle wrapping for continuous scrolling
        // If the background moves completely off-screen to the left, reset its X position
        // assuming it's meant to be a single, large, scrolling background that "tiles" itself.
        // For a more robust tiling background, you might have two or more background objects.
        if (_x + _width < 0) {
            _x = 0; // Simple reset to 0 if it goes off. This works well for a static background that might parallax scroll slightly.
                     // For a seamless repeating background, you'd typically have two background objects, and
                     // when one moves off-screen, it gets reset to the right of the other.
                     // e.g., _x = _width; (if it's one image and you're drawing it twice)
        }
    }
}

// NEW SETTER Implementation
void Background::setScrollSpeed(float newSpeed) {
    _scrollSpeed = newSpeed;
}