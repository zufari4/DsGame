#pragma once
#include <inttypes.h>


class Camera;
struct SDL_Renderer;


class Render 
{
public:
    Render(SDL_Renderer* renderer, Camera& camera);
    void setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void drawLine(float x1, float y1, float x2, float y2);
private:
    SDL_Renderer* renderer_;
    const Camera& camera_;
};
