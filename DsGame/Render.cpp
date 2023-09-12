#include "Render.h"
#include "Camera.h"
#include <SDL2/SDL_render.h>


Render::Render(SDL_Renderer* renderer, Camera& camera) 
    : renderer_(renderer)
    , camera_(camera)
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
