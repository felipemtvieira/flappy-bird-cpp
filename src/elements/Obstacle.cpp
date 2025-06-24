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

Obstacle::~Obstacle(){};

void Obstacle::draw(){
    if(this->_bitmap){
        float original_sprite_width = (float)al_get_bitmap_width(this->_bitmap);
        float original_sprite_height = (float)al_get_bitmap_height(this->_bitmap);

        al_draw_scaled_bitmap(
            this->_bitmap,                  
            0, 0,                           
            original_sprite_width,          
            original_sprite_height,         
            this->_x, this->_y,             
            this->_width, this->_height,    
            0                               
        );
    };
    
    #ifdef DEBUG_BUILD 
    if (_debugDraw) {
        al_draw_rectangle(_x, _y, _x + _width, _y + _height, al_map_rgb(255, 0, 255), 2); // Retângulo Magenta
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

bool Obstacle::getHasBeenScored() const { return this->_hasBeenScored; }

void Obstacle::setScored(bool scored) { this->_hasBeenScored = scored; }

void Obstacle::setScrollSpeed(float newSpeed) {
    this->_speed = newSpeed;
}
