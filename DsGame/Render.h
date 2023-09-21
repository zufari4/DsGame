#pragma once
#include <inttypes.h>


class Camera;
struct SDL_Renderer;


class Render 
{
public:
    Render(SDL_Renderer* renderer, Camera& camera, uint8_t clearColorR, uint8_t clearColorG, uint8_t clearColorB);
    void setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void drawLine(float x1, float y1, float x2, float y2);
    void drawPoint(float x, float y);
    SDL_Renderer* getRender();
    void clear();
    void present();
private:
    SDL_Renderer* renderer_;
    const Camera& camera_;
    uint8_t clearColorR_;
    uint8_t clearColorG_;
    uint8_t clearColorB_;
};
