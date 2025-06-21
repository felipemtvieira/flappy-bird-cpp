#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include <allegro5/allegro.h>

class Element {
protected:
    float _x;
    float _y;
    float _width;
    float _height;
    float _screenWidth;
    float _screenHeight;
    float _speed;
    ALLEGRO_BITMAP* _bitmap;
    bool _debugDraw;
    
    float _colliderOffsetX; 
    float _colliderOffsetY; 
    float _colliderWidth;   
    float _colliderHeight; 

public:
    Element(
        float x, 
        float y, 
        float width, 
        float height, 
        float screenWidth, 
        float screenHeight, 
        float speed, 
        ALLEGRO_BITMAP* bitmap
    );

    virtual ~Element() {} //ATENCAO

    virtual void draw() = 0;

    virtual void update(double deltaTime) = 0;

    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;

    float getColliderX() const { return _x + _colliderOffsetX; }
    float getColliderY() const { return _y + _colliderOffsetY; }
    float getColliderWidth() const { return _colliderWidth; }
    float getColliderHeight() const { return _colliderHeight; }

    void setDebugDraw(bool debugDraw) { _debugDraw = debugDraw; }

    void setColliderDimensions(float offsetX, float offsetY, float width, float height);
};

#endif