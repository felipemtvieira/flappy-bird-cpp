#ifndef GROUND_HPP
#define GROUND_HPP

#include "Element.hpp"

class Ground : public Element {
private:
    float _originalX; // To reset position for continuous scrolling

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

    ~Ground(); // Destructor (optional, as Element's destructor is virtual)

    void draw() override;
    void update(double deltaTime) override;

    // We can still expose a setter, which will internally update Element's _speed
    void setScrollSpeed(float newSpeed);
};

#endif // GROUND_HPP