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
    _isTopPipe(isTopPipe) {}

Obstacle::~Obstacle(){ 
    // std::cout << "Destruindo obstáculo" << std::endl; 
};

void Obstacle::draw(){
    if(_bitmap){
        float original_sprite_width = (float)al_get_bitmap_width(_bitmap);
        float original_sprite_height = (float)al_get_bitmap_height(_bitmap);

        al_draw_scaled_bitmap(
            _bitmap,                  // The bitmap to draw
            0, 0,                    // Source X, Y (start from top-left of the original bitmap)
            original_sprite_width,   // Source width (entire original bitmap width)
            original_sprite_height,  // Source height (entire original bitmap height)
            _x, _y,                    // Destination X, Y (where to draw on screen)
            _width, _height,           // Destination width, height (the desired size)
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
    _x -= _speed * deltaTime; 
}

bool Obstacle::isOffScreen() const {
    return _x + _width < 0; // Verifica se o cano saiu da tela à esquerda
}

float Obstacle::getIsTopPipe() const { return _isTopPipe; }

void Obstacle::setScrollSpeed(float newSpeed) {
    _speed = newSpeed;
}
