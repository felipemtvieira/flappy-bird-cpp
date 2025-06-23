#include "../../include/elements/Background.hpp"

#include <iostream>

Background::Background(float x, float y, float width, float height, float screenWidth, float screenHeight, float baseScrollSpeed, ALLEGRO_BITMAP* bitmap)
    : Element(x, y, width, height, screenWidth, screenHeight, baseScrollSpeed, bitmap)
    {}

Background::~Background() {}

void Background::draw() {
    if (this->_bitmap) {
        float original_sprite_width = (float)al_get_bitmap_width(this->_bitmap);
        float original_sprite_height = (float)al_get_bitmap_height(this->_bitmap);

        al_draw_scaled_bitmap(
            this->_bitmap,          // Bitmap
            0, 0,                   // Source X, Y 
            original_sprite_width,
            original_sprite_height,
            this->_x, this->_y,                 // Destination X, Y 
            this->_width, this->_height,        // Destination width, height 
            0
        );

        if (this->_x + this->_width < this->_width) { 
            al_draw_scaled_bitmap(
                this->_bitmap,
                0, 0,
                original_sprite_width,
                original_sprite_height,
                this->_x + this->_width, this->_y,    // Segundo bitmap do chão após o primeiro
                this->_width, this->_height,
                0
            );
        }

    } else {
        al_clear_to_color(al_map_rgb(135, 206, 235));
    }
}

void Background::update(double deltaTime) {
    if (this->_scrollSpeed != 0) {
        this->_x -= this->_scrollSpeed * deltaTime; //Move o chão para a esque
        if (this->_x + this->_width < 0) { // Verifica se o chão saiu inteiramente da tela, se sim, o move novamente para a extremidade direita para rolagem contínua
            this->_x = 0;
        }
    }
}

void Background::setScrollSpeed(float newSpeed) {
    this->_scrollSpeed = newSpeed;
}