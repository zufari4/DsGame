#pragma once

#include "Entity.h"

struct cpBody;
struct cpShape;
struct cpSpace;
struct SDL_Renderer;


class Rectangle: public Entity
{
public:
    Rectangle(cpSpace* space, SDL_Renderer* screen, float x, float y, float w, float h);
    void draw() override;
    void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;
    void setPos(float x, float y) override;
    void setAngle(float a) override;
    void resetVelocity() override;
    void setCamera(const Camera* camera) override;
private:
    cpBody*  body_;
    cpShape* shape_;
    SDL_Renderer* screen_;
    uint8_t r_;
    uint8_t g_;
    uint8_t b_;
    uint8_t a_;
    const Camera* camera_;
};