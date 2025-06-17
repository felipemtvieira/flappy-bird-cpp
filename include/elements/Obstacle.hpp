#include "Element.hpp"

class Obstacle: public Element {
private:
    bool _isTopPipe;

    Obstacle(float x, float y, float width, float height, ALLEGRO_BITMAP* bitmap, bool isTopPipe) 
        : Element(x, y, width, height, bitmap), _isTopPipe(isTopPipe) {}

    float getIsTopPipe() const { return _isTopPipe; }
};