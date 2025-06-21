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
        _colliderOffsetY(0), // Default to full size
        _colliderWidth(width),
        _colliderHeight(height)
    {}

float Element::getX() const { return _x; }
float Element::getY() const { return _y; }
float Element::getWidth() const { return _width; }
float Element::getHeight() const { return _height; }

void Element::setColliderDimensions(float offsetX, float offsetY, float width, float height) {
    _colliderOffsetX = offsetX;
    _colliderOffsetY = offsetY;
    _colliderWidth = width;
    _colliderHeight = height;
}