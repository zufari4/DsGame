#pragma once

struct SDL_Renderer;
struct cpSpace;

namespace DsMap
{
    void draw(SDL_Renderer* render, cpSpace* space, float x, float y, float size, float padding);
    void resetPos();
}
