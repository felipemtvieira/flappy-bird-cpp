#ifndef GROUND_HPP
#define GROUND_HPP

#include "Element.hpp"

class Ground : public Element {
private:

public:
    Ground(
        float x, 
        float y, 
        float width, 
        float height, 
        float screenWidth, 
        float screenHeight, 
        float baseScrollSpeed, 
        ALLEGRO_BITMAP* bitmap = nullptr    
    );

    ~Ground();

    void draw() override;
    void update(double deltaTime) override;

    void setScrollSpeed(float newSpeed);
};

#endif