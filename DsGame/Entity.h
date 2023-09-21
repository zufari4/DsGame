#pragma once

#include <inttypes.h>


class Camera;


class Entity
{
public:
    Entity() = default;
    virtual ~Entity() = default;
    virtual void draw() = 0;
    virtual void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) = 0;
    virtual void setPos(float x, float y) = 0;
    virtual void setAngle(float a) = 0;
    virtual void resetVelocity() = 0;
    virtual void setStatic() = 0;
    virtual void setMass(float m) = 0;
};