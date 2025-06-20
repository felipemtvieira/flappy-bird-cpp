#include "Element.hpp"

Element::Element(float x, float y, float width, float height, float speed, ALLEGRO_BITMAP* bitmap = nullptr) 
    : _x(x), _y(y), _width(width), _height(height), _speed(speed), _bitmap(bitmap) {}

float Element::getX() const { return _x; }
float Element::getY() const { return _y; }
float Element::getWidth() const { return _width; }
float Element::getHeight() const { return _height; }