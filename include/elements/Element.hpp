#include <allegro5/allegro.h>

class Element {
protected:
    float _x;
    float _y;
    float _width;
    float _height;
    float _speed;
    ALLEGRO_BITMAP* _bitmap;

public:
    Element(float x, float y, float width, float height, float speed, ALLEGRO_BITMAP* bitmap = nullptr);

    virtual ~Element() {} //ATENCAO

    virtual void draw() = 0;

    virtual void update(double deltaTime) = 0;

    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;
};