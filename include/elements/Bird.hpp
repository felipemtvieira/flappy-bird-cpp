#include "Element.hpp"

class Bird : public Element{
private:
    float _jumpForce, _verticalVelocity;

    Bird(float x, float y, float width, float height, ALLEGRO_BITMAP* bitmap, float jumpForce,float verticalVelocity) 
        : Element(x, y, width, height, bitmap), _jumpForce(jumpForce), _verticalVelocity(verticalVelocity) {}


    float getJumpForce() const { return _jumpForce; }
    float getVerticalVelocity() const { return _verticalVelocity; }
};