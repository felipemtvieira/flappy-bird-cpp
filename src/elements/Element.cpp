#include "Element.hpp"

Element::Element(
        float x, 
        float y, 
        float width, 
        float height, 
        float screenWidth, 
        float screenHeight, 
        float speed, 
        ALLEGRO_BITMAP* bitmap = nullptr
    ) : _x(x), 
        _y(y), 
        _width(width), 
        _height(height), 
        _screenWidth(screenWidth),
        _screenHeight(screenHeight),
        _speed(speed), 
        _bitmap(bitmap), 
        _debugDraw(false),
        _colliderOffsetX(0), 
        _colliderOffsetY(0),
        _colliderWidth(width),
        _colliderHeight(height)
    {}

float Element::getX() const { return this->_x; }
float Element::getY() const { return this->_y; }
float Element::getWidth() const { return this->_width; }
float Element::getHeight() const { return this->_height; }
float Element::getColliderX() const { return this->_x + this->_colliderOffsetX; }
float Element::getColliderY() const { return this->_y + this->_colliderOffsetY; }
float Element::getColliderWidth() const { return this->_colliderWidth; }
float Element::getColliderHeight() const { return this->_colliderHeight; }

void Element::setColliderDimensions(float offsetX, float offsetY, float width, float height) {
    this->_colliderOffsetX = offsetX;
    this->_colliderOffsetY = offsetY;
    this->_colliderWidth = width;
    this->_colliderHeight = height;
}