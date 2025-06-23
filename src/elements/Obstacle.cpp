#include "Obstacle.hpp"
#include <allegro5/allegro_primitives.h> 
#include <iostream>

Obstacle::Obstacle(
    float x,
    float y, 
    float width, 
    float height, 
    float screenWidth, 
    float screenHeight, 
    float speed, 
    ALLEGRO_BITMAP* bitmap, 
    bool isTopPipe
    ) : Element(x, y, width, height, screenWidth, screenHeight, speed, bitmap), 
    _isTopPipe(isTopPipe), _hasBeenScored(false) {}

Obstacle::~Obstacle(){ 
    // std::cout << "Destruindo obstáculo" << std::endl; 
};

void Obstacle::draw(){
    if(this->_bitmap){
        float original_sprite_width = (float)al_get_bitmap_width(this->_bitmap);
        float original_sprite_height = (float)al_get_bitmap_height(this->_bitmap);

        al_draw_scaled_bitmap(
            this->_bitmap,                  // The bitmap to draw
            0, 0,                    // Source X, Y (start from top-left of the original bitmap)
            original_sprite_width,   // Source width (entire original bitmap width)
            original_sprite_height,  // Source height (entire original bitmap height)
            this->_x, this->_y,                    // Destination X, Y (where to draw on screen)
            this->_width, this->_height,           // Destination width, height (the desired size)
            0                        // Flags (e.g., ALLEGRO_FLIP_HORIZONTAL)
        );
    };
    
    #ifdef DEBUG_BUILD 
    if (_debugDraw) {
        al_draw_rectangle(_x, _y, _x + _width, _y + _height, al_map_rgb(255, 0, 255), 2); // Magenta outline
    }
    #endif
}

void Obstacle::update(double deltaTime){ // Move o cano para esquerda;
    this->_x -= this->_speed * deltaTime; 
}

bool Obstacle::isOffScreen() const {
    return this->_x + this->_width < 0; // Verifica se o cano saiu da tela à esquerda
}

bool Obstacle::getIsTopPipe() const { return this->_isTopPipe; }

bool Obstacle::getHasBeenScored() const { return this->_hasBeenScored; } // NEW: Getter for _hasBeenScored

void Obstacle::setScored(bool scored) { this->_hasBeenScored = scored; } // NEW: Setter for _hasBeenScored

void Obstacle::setScrollSpeed(float newSpeed) {
    this->_speed = newSpeed;
}
