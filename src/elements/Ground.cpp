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

        // Draw the first instance of the ground bitmap
        al_draw_scaled_bitmap(
            _bitmap,                    // The bitmap to draw
            0, 0,                       // Source X, Y (entire original bitmap)
            _width,
            _height,
            _x, _y,                     // Destination X, Y (from Element's position)
            800.0, 120.0,               // Destination width, height (from Element's dimensions)
            0
        );

        // To create continuous scrolling, draw a second instance of the ground
        // directly to the right of the first one.
        // This is crucial if a single ground image doesn't cover the entire screen width.
        al_draw_scaled_bitmap(
            _bitmap,
            0, 0,
            _width,
            _height,
            _x + _width + 20.0, _y, // Draw second instance right after the first
            800.0, 120.0, 
            0
        );

    } else {
        // Fallback: draw a colored rectangle if no bitmap is loaded
        al_draw_filled_rectangle(_x, _y, _x + _width, _y + _height, al_map_rgb(100, 50, 0)); // Brown ground
    }
}

void Ground::update(double deltaTime) {
    _x -= _speed * deltaTime; // Use _x and _speed inherited from Element

    // Handle continuous scrolling:
    // If the first ground segment moves completely off-screen to the left,
    // reset its position to be immediately after the second segment.
    if (_x + _width < 0) {
        // This repositions it to the right of the second (currently visible) ground segment.
        // If you only had one ground object that covered the whole screen and you wanted
        // to loop it by wrapping, you might do _x = _x + _width; or _x = _screenWidth;
        // But with two segments, resetting one to the right of the other maintains the continuous effect.
        _x = _screenWidth; // Reset to the right edge of the screen for seamless looping
                          // (assuming _width is screenWidth, so _x + _width would be 2*screenWidth when reset)
    }
}

// Set the inherited _speed member
void Ground::setScrollSpeed(float newSpeed) {
    _speed = newSpeed; // Update the inherited speed
}