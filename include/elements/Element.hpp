#include <allegro5/allegro.h>

class Element{
protected:
    float _x, _y, _width, _height; //Posição e dimensão do objeto
    ALLEGRO_BITMAP* _bitmap;

public:
    Element(float x, float y, float width, float height, ALLEGRO_BITMAP* bitmap = nullptr) 
        : _x(x), _y(y), _width(width), _height(height), _bitmap(bitmap) {}

    virtual ~Element() {} //VER ISSO AQUI

    virtual void draw() = 0;

    virtual void update() = 0;

    float getX() const { return _x; }
    float getY() const { return _y; }
    float getWidth() const { return _width; }
    float getHeight() const { return _height; }
};