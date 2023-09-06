#pragma once

struct SDL_Renderer;
struct cpSpace;

namespace DsMap
{
    void drawDS(SDL_Renderer* render, cpSpace* space, float x, float y, float size, float padding);
    void drawSubscribe(SDL_Renderer* render, cpSpace* space, float x, float y, float size, float padding);
    void resetPos();
}
