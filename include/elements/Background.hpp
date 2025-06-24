#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "Element.hpp"

class Background : public Element {
private:
    float _scrollSpeed; // Velocidade

public:
    Background(float x, float y, float width, float height, float screenWidth, float screenHeight, float baseScrollSpeed, ALLEGRO_BITMAP* bitmap = nullptr);
    ~Background();

    void draw() override;
    void update(double deltaTime) override;

    // Setter para efeito de paralaxe (Ainda não implementado)
    void setScrollSpeed(float newSpeed);
};

#endif