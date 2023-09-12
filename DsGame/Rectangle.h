#pragma once

#include "Entity.h"

struct cpBody;
struct cpShape;
struct cpSpace;
class  Render;


class Rectangle: public Entity
{
public:
    Rectangle(cpSpace* space, Render& render, float x, float y, float w, float h);
    void draw() override;
    void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;
    void setPos(float x, float y) override;
    void setAngle(float a) override;
    void resetVelocity() override;

private:
    cpBody*  body_;
    cpShape* shape_;
    Render&  render_;
    uint8_t r_;
    uint8_t g_;
    uint8_t b_;
    uint8_t a_;
};