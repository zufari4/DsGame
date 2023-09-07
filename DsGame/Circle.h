#pragma once

#include "Entity.h"
#include <vector>

struct cpBody;
struct cpShape;
struct cpSpace;
struct SDL_Renderer;


class Cirle : public Entity
{
public:
    Cirle(cpSpace* space, SDL_Renderer* screen, float x, float y, float r);
    void draw() override;
    void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;
    void setPos(float x, float y) override;
    void setAngle(float a) override;
    void resetVelocity() override;
    void setCamera(const Camera* camera) override;

private:
    void updateDrawShape(float radiusX, float radiusY);

    cpBody* body_;
    cpShape* shape_;
    SDL_Renderer* screen_;
    uint8_t r_;
    uint8_t g_;
    uint8_t b_;
    uint8_t a_;
    std::vector<float> xShape_;
    std::vector<float> yShape_;
    const Camera* camera_;
};
