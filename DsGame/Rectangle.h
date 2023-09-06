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
private:
    cpBody*  body_;
    cpShape* shape_;
    SDL_Renderer* screen_;
};