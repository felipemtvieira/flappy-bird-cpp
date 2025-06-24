#include "../../include/elements/Ground.hpp"
#include <allegro5/allegro_primitives.h>

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
            this->_bitmap,                          
            0, 0,                                   
            this->_width,
            this->_height,
            this->_x, this->_y,                     
            this->_screenWidth,                     
            this->_screenHeight - this->_y,         
            0
        );

        al_draw_scaled_bitmap(
            this->_bitmap,
            0, 0,
            this->_width,
            this->_height,
            this->_x + this->_screenWidth - 2.0, this->_y,      
            this->_screenWidth,                                 
            this->_screenHeight - this->_y,
            0
        );

    }
}

void Ground::update(double deltaTime) {
    this->_x -= this->_speed * deltaTime; 

    if (this->_x + this->_screenWidth <= 0) {
        this->_x = 0;
    }
}

void Ground::setScrollSpeed(float newSpeed) {
    this->_speed = newSpeed; 
}