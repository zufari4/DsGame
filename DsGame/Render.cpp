#include "Render.h"
#include "Camera.h"
#include <SDL2/SDL_render.h>


Render::Render(SDL_Renderer* renderer, Camera& camera, uint8_t clearColorR, uint8_t clearColorG, uint8_t clearColorB)
    : renderer_(renderer)
    , camera_(camera)
    , clearColorR_(clearColorR)
    , clearColorG_(clearColorG)
    , clearColorB_(clearColorB)
{
}

void Render::setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    SDL_SetRenderDrawColor(renderer_, r, g, b, a);
}

void Render::drawLine(float x1, float y1, float x2, float y2) 
{
    SDL_RenderDrawLineF(renderer_,
        camera_.worldToScreenX(x1), camera_.worldToScreenY(y1),
        camera_.worldToScreenX(x2), camera_.worldToScreenY(y2)
    );
}

void Render::drawPoint(float x, float y)
{
    SDL_RenderDrawPoint(renderer_, camera_.worldToScreenX(x), camera_.worldToScreenY(y));
}

SDL_Renderer* Render::getRender()
{
    return renderer_;
}

void Render::clear()
{
    SDL_SetRenderDrawColor(renderer_, clearColorR_, clearColorG_, clearColorB_, 255);
    SDL_RenderClear(renderer_);
}

void Render::present()
{
    SDL_RenderPresent(renderer_);
}
