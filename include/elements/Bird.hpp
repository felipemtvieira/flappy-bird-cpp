// #ifndef BIRD_HPP
// #define BIRD_HPP

// #include "VisualElement.hpp"
// #include <vector>

// class Bird : public VisualElement {
// private:
//     float velocityY;
//     float gravity;   // Base gravity
//     float jumpForce; // Base jump force
//     std::vector<ALLEGRO_BITMAP*> animationFrames;
//     int currentFrame;
//     double animationTimer;
//     double frameDuration;

// public:
//     // Constructor now takes base gravity and jumpForce
//     Bird(float x, float y, float width, float height, float baseGravity, float baseJumpForce, const std::vector<ALLEGRO_BITMAP*>& frames);
//     ~Bird();

//     void draw() override;
//     void update(double deltaTime) override;
//     void jump();

//     float getVelocityY() const;
//     void setVelocityY(float vel);

//     // NEW SETTERS
//     void setGravity(float newGravity);
//     void setJumpForce(float newJumpForce);
// };

// #endif // BIRD_HPP